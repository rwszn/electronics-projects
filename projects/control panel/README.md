# Arduino Control Panel

An Arduino-based control panel with a 0.96-inch OLED display, four push buttons, an LED, and a passive buzzer. The published version 1.0 sketch is written for the Arduino Uno R3.

## Current published version

**1.0**

## What it does

- Displays an OLED main menu and lets you navigate between options.
- Turns an LED on and off.
- Turns a passive buzzer on and off.
- Plays a fireworks animation with a short melody.
- Lets you adjust OLED brightness, animation speed, and buzzer volume.
- Provides button sound feedback.

## Which microcontroller can I use?

You do **not** have to use an Arduino Uno R3 for every version of this project. However, the published 1.0 sketch is specifically written for the Uno R3, so not every board is a drop-in replacement.

- **Arduino Uno R3:** the reference board; use the supplied code and pin connections.
- **Classic Arduino Nano with the ATmega328P:** the closest alternative. It has the same basic ATmega328P architecture and compatible D4–D9, A4/A5 I2C, and Timer1 buzzer arrangement. Check that it is the classic ATmega328P Nano, not a newer Nano model with a different processor.
- **Arduino Mega 2560:** can be adapted, but its Timer1 output pin mapping differs. The buzzer output and wiring need changing, and the sketch must be checked for compatibility.
- **ESP32, Raspberry Pi Pico, and other microcontrollers:** can be used for a port of the project, but the current sketch will not run unchanged. Its buzzer code directly accesses AVR Timer1 registers, which these boards do not share. The buzzer code, pin assignments, and possibly the display wiring must be adapted.

### What a different board needs

For a compatible adaptation, choose a board that provides:

1. Enough usable GPIO pins for four buttons, one LED, and one buzzer output.
2. I2C support for the OLED display, with SDA and SCL pins available.
3. A compatible 128 × 64 SSD1306 OLED library or an equivalent display driver.
4. A hardware timer or PWM/audio method that can generate the buzzer tones. The exact code depends on the microcontroller.
5. Logic levels and power connections that are safe for the exact OLED module and other components. A 3.3V board must not have 5V signals connected to its GPIO pins unless the board explicitly supports them; use suitable level shifting where needed.

If you use a different board, first identify its exact model and processor. Then update the pin table, wiring diagram, and buzzer implementation for that board. Do not assume the Uno wiring diagram or Uno sketch will work unchanged.

## Components required

These are the components used by this project only; they are not a personal inventory. The Uno R3 is the reference option; see the compatibility section above for alternatives.

- Arduino Uno R3 (or a compatible/adapted microcontroller board)
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
2. Connect the Arduino board by USB.
3. Select the correct board and port in Arduino IDE. For the supplied version 1.0 sketch, select Arduino Uno R3 (or a classic ATmega328P Nano if using that compatible alternative).
4. Install the required libraries if they are not already installed.
5. Upload the sketch.

## Controls

- **Power:** turns the OLED interface on or off.
- **UP / DOWN:** moves through menu options or changes a selected setting.
- **OK:** opens a menu item, confirms a setting, or exits the fireworks screen.

Settings are held in RAM and return to their defaults after the board is reset or loses power.

## Technical notes and limitations

- The OLED is initialised at I2C address `0x3C`.
- The sketch uses AVR Timer1 registers directly to generate buzzer tones on the Uno R3's D9 output. Other libraries or features that use Timer1 may conflict. This is why boards with a different processor need a code adaptation.
- The OLED VDD connection must match the voltage requirements of the exact display module.
- This published 1.0 sketch has not been independently compiled or hardware-tested as part of this public release. Check the wiring and library versions before powering the circuit.

## Version history

| Version | Notes |
|---|---|
| 1.0 | Initial published control panel sketch, wiring guide, and schematic. |
