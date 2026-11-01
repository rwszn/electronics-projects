# Arduino Control Panel

An Arduino Uno R3 control panel with a 0.96-inch OLED display, four push buttons, an LED, and a passive buzzer.

## Current published version

**1.0**

## What it does

- Displays an OLED main menu and lets you navigate between options.
- Turns an LED on and off.
- Turns a passive buzzer on and off.
- Plays a fireworks animation with a short melody.
- Lets you adjust OLED brightness, animation speed, and buzzer volume.
- Provides button sound feedback.

## Components required

These are the components used by this project only; they are not a personal inventory.

- Arduino Uno R3
- 0.96-inch 128 × 64 I2C OLED display (address 0x3C)
- Four momentary push buttons
- One LED
- One 220-ohm resistor for the LED
- One passive buzzer
- Breadboard and jumper wires for prototyping
- USB cable suitable for the Arduino board

## Wiring

| Component | Arduino connection |
|---|---|
| Power button | D7 to GND |
| UP button | D6 to GND |
| DOWN button | D5 to GND |
| OK button | D4 to GND |
| LED anode | D8 through a 220-ohm resistor |
| LED cathode | GND |
| Passive buzzer positive (+) | D9 |
| Passive buzzer negative (−) | GND |
| OLED SDA | A4 |
| OLED SCL/SCK | A5 |
| OLED VDD | 5V only if the specific display module supports 5V |
| OLED GND | GND |

The buttons use the Arduino's internal pull-up resistors (`INPUT_PULLUP`). Each button connects its input pin to GND when pressed.

**OLED voltage warning:** OLED modules are not all wired identically. Check the markings or datasheet for your exact module before connecting VDD. Use 5V only if the module supports it.

See the [wiring schematic](schematics/control-panel-wiring.svg).

## Software

- Arduino IDE
- Adafruit GFX Library
- Adafruit SSD1306 Library

Install the two Adafruit libraries through the Arduino IDE Library Manager.

## Uploading the code

1. Open [control-panel-1.0.ino](code/control-panel-1.0.ino) in Arduino IDE.
2. Connect the Arduino Uno R3 by USB.
3. Select the correct Arduino Uno board and port.
4. Install the required libraries if they are not already installed.
5. Upload the sketch.

## Controls

- **Power:** turns the OLED interface on or off.
- **UP / DOWN:** moves through menu options or changes a selected setting.
- **OK:** opens a menu item, confirms a setting, or exits the fireworks screen.

Settings are held in RAM and return to their defaults after the board is reset or loses power.

## Technical notes and limitations

- The OLED is initialised at I2C address `0x3C`.
- The sketch uses Timer1 directly to generate buzzer tones on D9. Other libraries or features that use Timer1 may conflict.
- The OLED VDD connection must match the voltage requirements of the exact display module.
- This published 1.0 sketch has not been independently compiled or hardware-tested as part of this public release. Check the wiring and library versions before powering the circuit.

## Version history

| Version | Notes |
|---|---|
| 1.0 | Initial published control panel sketch, wiring guide, and schematic. |
