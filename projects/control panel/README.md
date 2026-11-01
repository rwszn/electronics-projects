# Microcontroller Control Panel

A DIY control panel using a small OLED display, four push buttons, an LED, and a passive buzzer. **This project is designed to be adaptable to different microcontrollers**, rather than being limited to the Arduino Uno. The published sketch uses the Arduino framework, so each target board must support the required framework features and libraries.

## Current published version

**1.1 — portable Arduino-framework sketch**

## Features

- OLED menu with button navigation
- LED on/off control
- Passive buzzer on/off control and button feedback
- Fireworks animation with a short melody
- OLED brightness and animation-speed settings
- Settings reset to defaults when the board loses power or resets

## Microcontroller compatibility

Version 1.1 uses the Arduino framework's standard `tone()` and `noTone()` functions instead of directly accessing AVR timer registers. This removes the sketch's direct dependency on the Arduino Uno's AVR timer hardware and makes it easier to adapt to many Arduino-compatible boards.

**The project is designed to be adaptable to different microcontrollers, but it is not guaranteed to work unchanged on every board.** Compatibility depends on the board's Arduino core, pin mapping, I2C implementation, library support, and whether its core supports `tone()`.

Potential target boards include Arduino Uno/Nano-class boards, Arduino Mega, ESP32 boards, and Raspberry Pi Pico/RP2040 boards with suitable Arduino cores. Each board still needs its own wiring/pin review and may require small code changes. For boards without a compatible Arduino framework, the sketch must be ported to that platform's APIs.

### Important differences from version 1.0

- Replaced Uno/AVR-specific Timer1 register control with `tone()` / `noTone()`.
- Removed the volume percentage setting because standard `tone()` does not provide consistent buzzer-volume control across different boards.
- Kept the fireworks animation and melody.
- The pin assignments below remain the reference wiring for an Arduino Uno R3; adapt them for other boards.

## Components

- Microcontroller board supported by an Arduino framework (Uno R3 is the reference)
- 0.96-inch 128 × 64 I2C SSD1306 OLED display (commonly address `0x3C`)
- Four momentary push buttons
- One LED and one 220-ohm resistor
- One passive buzzer
- Breadboard and jumper wires
- USB cable suitable for the selected board

## Reference wiring — Arduino Uno R3

| Component | Uno connection |
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
| OLED VDD | 5V only if the exact module supports it |
| OLED GND | GND |

Buttons use `INPUT_PULLUP`; each button connects its input to GND when pressed.

**Voltage warning:** Check the exact OLED module's voltage requirements. Many 3.3V microcontroller boards have GPIO pins that are not 5V tolerant. Use appropriate level shifting or a suitable display module where required.

See the [wiring schematic](schematics/control-panel-wiring.svg). The schematic shows the Uno reference wiring, not a universal pinout.

## Software

- Arduino IDE or another compatible Arduino-framework environment
- Adafruit GFX Library
- Adafruit SSD1306 Library

Install the display libraries using the environment's library manager.

## Uploading

1. Open [control-panel-1.1-portable.ino](code/control-panel-1.1-portable.ino).
2. Select the exact board and port in your Arduino-compatible development environment.
3. Install the required display libraries.
4. Review and adapt the pin assignments for your board.
5. Confirm the OLED voltage and wiring before powering the circuit.
6. Upload the sketch.

## Controls

- **Power:** turns the OLED interface on or off.
- **UP / DOWN:** moves through menus or changes the selected setting.
- **OK:** opens a menu item, confirms a setting, or exits the fireworks screen.

## Technical notes and limitations

- The OLED is initialized at I2C address `0x3C`; change this if your module uses another address.
- The fireworks melody and button sounds use `tone()`. Board cores vary in their implementation and pin/timer restrictions; check the selected board's documentation if sound does not work.
- The brightness setting controls the SSD1306 display contrast, not physical backlight brightness.
- Settings are held in RAM and reset after power loss or board reset.
- Version 1.1 has not been independently compiled or hardware-tested as part of this public release. Verify the selected board, libraries, and wiring before use.

## Version history

| Version | Notes |
|---|---|
| 1.1 | Portable Arduino-framework version; uses `tone()` / `noTone()`, removes the cross-board-inconsistent volume setting, and retains fireworks. |
| 1.0 | Original Arduino Uno R3 sketch using AVR Timer1 registers for buzzer output. |
