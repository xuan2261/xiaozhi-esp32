# ESP32-2432S028 (Cheap Yellow Display - CYD)

## Board Overview

The ESP32-2432S028 (commonly known as "Cheap Yellow Display" or CYD) is a development board featuring:
- ESP32-WROOM-32 module (dual-core, 240 MHz)
- 2.8-inch TFT LCD display (320×240, ILI9341 driver)
- Resistive touchscreen (XPT2046 controller)
- RGB LED
- MicroSD card slot
- Light sensor (LDR)
- Speaker output
- Extended GPIO connectors

## Hardware Specifications

### Display (ILI9341)
- **Resolution**: 320×240 pixels
- **Interface**: SPI (HSPI)
- **Driver**: ILI9341
- **Backlight**: GPIO 21

| Pin Function | GPIO |
|--------------|------|
| MOSI | 13 |
| MISO | 12 |
| SCLK | 14 |
| CS | 15 |
| DC | 2 |
| RST | -1 (software) |
| Backlight | 21 |

### Touchscreen (XPT2046)
- **Interface**: SPI (VSPI)
- **Driver**: XPT2046

| Pin Function | GPIO |
|--------------|------|
| MOSI | 32 |
| MISO | 39 |
| CLK | 25 |
| CS | 33 |
| IRQ | 36 |

### RGB LED (Active Low)
| LED Color | GPIO |
|-----------|------|
| Red | 4 |
| Green | 16 |
| Blue | 17 |

### MicroSD Card
| Pin Function | GPIO |
|--------------|------|
| MISO | 19 |
| MOSI | 23 |
| SCK | 18 |
| CS | 5 |

### Other Peripherals
- **Light Sensor (LDR)**: GPIO 34 (ADC)
- **Speaker**: GPIO 26 (PWM)
- **Boot Button**: GPIO 0

### Extended GPIO Connectors
- **P3**: GND, GPIO 35, GPIO 22, GPIO 21
- **CN1**: GND, GPIO 22, GPIO 27, 3V3

## Important Notes

1. **No Audio Codec**: This board does not have a dedicated audio codec chip. Audio functionality uses a dummy codec for compatibility.

2. **GPIO 21 Shared**: GPIO 21 is used for LCD backlight, so it cannot be used for I2C SDA when the display is on.

3. **GPIO 27 Pull-up**: GPIO 27 has a 10K pull-up resistor, so `analogRead()` will return 4095 when not connected.

4. **TX/RX Limitation**: TX (GPIO 1) and RX (GPIO 3) are connected to the CH340 USB-to-serial converter and may not work reliably for external serial devices.

5. **RGB LED Active Low**: The RGB LEDs are active low, meaning LOW = ON and HIGH = OFF.

## Building Firmware

### Using idf.py
```bash
# Set target to ESP32
idf.py set-target esp32

# Configure board type
idf.py menuconfig
# Navigate to: Xiaozhi Assistant -> Board Type
# Select: ESP32-2432S028 (Cheap Yellow Display - CYD)

# Build
idf.py build

# Flash
idf.py flash
```

### Using release.py script
```bash
python scripts/release.py esp32-2432s028 --name esp32-2432s028
```

## References

- [Random Nerd Tutorials - CYD Pinout](https://randomnerdtutorials.com/esp32-cheap-yellow-display-cyd-pinout-esp32-2432s028r/)
- [macsbug - ESP32-2432S028](https://macsbug.wordpress.com/2022/08/17/esp32-2432s028/)

## Known Limitations

1. **No Audio Input/Output**: The board lacks audio codec hardware, so voice interaction features are not available.
2. **Limited RAM**: ESP32 (not S3) has only 320 KB RAM, which may limit some features compared to ESP32-S3 boards.
3. **Touch Calibration**: The resistive touchscreen may require calibration for accurate touch detection.

## Recommended Use Cases

- Visual display applications
- IoT dashboards
- Status monitors
- Simple GUI projects
- Learning platform for ESP32 and LVGL

