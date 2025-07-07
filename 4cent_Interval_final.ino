#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <math.h>

//Keypad
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'=', '+', '.', '0'},
  {'-', '3', '2', '1'},
  {'*', '6', '5', '4'},
  {'N', '9', '8', '7'}
};
byte rowPins[ROWS] = {23, 22, 21, 19};
byte colPins[COLS] = {18, 5, 17, 16};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

//LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Sound & Button
const int dacPin = 25;
const int incBtnPin = 32;
int baseToneIndex = 0;
const int maxTones = 600;
const int tonesPerPage = 15;

float baseFrequency = 440.0; //can be changed

const int sampleRate = 8000; 
const int maxVoices = 4;

struct Note {
  bool active = false;
  float frequency = 0.0;
  float phase = 0.0;
};

Note activeNotes[maxVoices];

const float stepRatio = pow(2.0, 1.0 / 600.0);
unsigned long lastSampleMicros = 0;
const unsigned long sampleInterval = 1000000 / sampleRate;
char pressedKeys[maxVoices];
int pressedCount = 0;

float getFrequency(int index) {
  return baseFrequency * pow(stepRatio, index);
}

int keyToIndex(char key) {
  switch(key) {
    case '=': return 0;
    case '+': return 1;
    case '.': return 2;
    case '0': return 3;
    case '-': return 4;
    case '3': return 5;
    case '2': return 6;
    case '1': return 7;
    case '*': return 8;
    case '6': return 9;
    case '5': return 10;
    case '4': return 11;
    case 'N': return 12;
    case '9': return 13;
    case '8': return 14;
    case '7': return 15;
    default: return -1;
  }
}

void updateLCDBaseTone() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("BaseTone:");
  float freq = getFrequency(baseToneIndex);
  lcd.setCursor(0, 1);
  lcd.print(freq, 2); lcd.print(" Hz");
}

void setup() {
  Serial.begin(115200);
  Wire.begin(26, 27);
  lcd.init(); lcd.backlight();
  pinMode(dacPin, OUTPUT);
  pinMode(incBtnPin, INPUT_PULLUP);

  lcd.setCursor(0, 0);
  lcd.print("Synthesizer Ready");
  delay(1000);
  lcd.clear();
  updateLCDBaseTone();
  lastSampleMicros = micros();
}

void loop() {
  static bool lastBtnState = HIGH;
  bool btnState = digitalRead(incBtnPin);
  if (lastBtnState == HIGH && btnState == LOW) {
    baseToneIndex += tonesPerPage;
    if (baseToneIndex >= maxTones) baseToneIndex = 0;
    updateLCDBaseTone();
    delay(200); 
  }
  lastBtnState = btnState;

  keypad.getKeys(); 

  for (int i = 0; i < maxVoices; i++) activeNotes[i].active = false;
  pressedCount = 0;

  for (int i = 0; i < LIST_MAX; i++) {
    if (keypad.key[i].kstate == PRESSED || keypad.key[i].kstate == HOLD) {
      char k = keypad.key[i].kchar;
      int idx = keyToIndex(k);
      if (idx >= 0 && idx < tonesPerPage && pressedCount < maxVoices) {
        pressedKeys[pressedCount] = k;
        pressedCount++;
      }
    }
  }

  for (int i = 0; i < pressedCount; i++) {
    int idx = keyToIndex(pressedKeys[i]);
    activeNotes[i].active = true;
    activeNotes[i].frequency = getFrequency(baseToneIndex + idx);
  }

  unsigned long now = micros();
  while (now - lastSampleMicros >= sampleInterval) {
    lastSampleMicros += sampleInterval;

    float sample = 0;

    if (pressedCount > 0) {
      for (int i = 0; i < maxVoices; i++) {
        if (activeNotes[i].active) {
          activeNotes[i].phase += 2.0 * PI * activeNotes[i].frequency / sampleRate;
          if (activeNotes[i].phase > 2.0 * PI) activeNotes[i].phase -= 2.0 * PI;
          sample += sin(activeNotes[i].phase);
        }
      }
      sample /= pressedCount; 
    }

    int dacValue = (int)((sample + 1.0) * 127.5);
    dacWrite(dacPin, dacValue);

    now = micros();
  }
}
