# ESP32 Synthesizer

A lightweight, polyphonic synthesizer powered by an ESP32. It uses a 4x4 matrix keypad for note input, a 16x2 I2C LCD for frequency display, and the ESP32’s internal DAC for real-time wave generation.

---

## Features

* **Real-time audio output** using the ESP32's built-in DAC
* **16-key matrix keypad** for triggering musical notes
* **16x2 LCD screen** for showing current base frequency
* **Polyphonic synthesis** — supports up to 4 simultaneous notes
* **Adjustable base frequency** using a dedicated push button
* **3.5mm audio jack** output for headphones or speakers
---

## Usage

### Prerequisites

* ESP32 development board
* Required libraries:

  * `Keypad`
  * `LiquidCrystal_I2C`
* 3.5mm jack breakout (for connecting audio output)
* Matrix keypad
* A push-button along with the necessary resistor
* **Wiring:**

| Component      | ESP32 Pin        |
| -------------- | ---------------- |
| Keypad Rows    | 23, 22, 21, 19   |
| Keypad Columns | 18, 5, 17, 16    |
| DAC Output     | 25               |
| LCD I2C        | SDA: 26, SCL: 27 |
| Inc. Button    | 32               |

---

### Getting Started

1. **Upload the Code**:
   Use Arduino IDE or PlatformIO to flash the sketch to your ESP32.

2. **Connect Audio**:
   Plug a 3.5mm audio jack into GPIO 25 for DAC output to headphones or speakers.

3. **Play Notes**:
   Press any key on the keypad to play notes. Hold multiple keys for chords.

4. **Adjust Base Frequency**:
   Use the button connected to pin 32 to shift the base tone up by 15 semitones.

5. **Check the LCD**:
   The screen will show the current base frequency in Hz.

---

## Tone Mapping Logic

Each keypad key maps to a tone index (0–15) relative to a **base frequency**, which can be incremented in blocks of 15.

Frequency is calculated using:

```cpp
frequency = baseFrequency * pow(2.0, index / 600.0);
```

The result is a finely divided pitch scale that allows smooth transitions between frequencies.

---

## Inspiration

This project was born from a desire to blend music, code, and microcontrollers into something playful and interactive. It’s a great way to learn about digital audio synthesis, keypad input handling, and real-time control on embedded systems.


---
## Collaborators

Built with love and creativity by:

* [Sina Noorizadeh](https://github.com/SinaNoorizadeh)
* [Rojin Rohani](https://github.com/Rojin-rohani)
* [Saleh Molazade](https://github.com/MKmasterg)

---

## License

MIT License — free to use, modify, and share.
