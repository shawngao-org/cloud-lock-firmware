# Cloud Lock Firmware
***
This is an IoT door lock firmware based on STM32 MCU.

## Description
***
Currently the firmware is developed based on STM32F103RCT6.

## Getting Started
***
### Hardware
| Emoji              | Description             |
|--------------------|-------------------------|
| :heavy_check_mark: | The driver is available |
| :x:                | The driver is under development |

+ :heavy_check_mark: STM32F103RCT6 or other development boards
+ :heavy_check_mark: 0.96 inch OLED display (4Pin: SDA, SCL, VCC +3.3V, GND)
+ :heavy_check_mark: TTP229 Matrix keypad Module (PH2.0 to 4Pin DuPont line: SCL, SDO, VCC +5V, GND)
+ :heavy_check_mark: RFID-RC522 (8Pin: SDA, SCK, MOSI, MISO, IRO, GND, RST, VCC +3.3V)
+ :x: ESP-8266(ESP-12F)

### Software
+ CLion or other IDE
+ STM32CubeMX
+ gcc-arm-none-eabi
+ CMake

## License
***
This project is licensed under the ***MIT*** License - see the LICENSE.md file for details.
