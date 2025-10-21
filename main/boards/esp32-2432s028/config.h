#ifndef _BOARD_CONFIG_H_
#define _BOARD_CONFIG_H_

#include <driver/gpio.h>

// Audio configuration (using dummy codec - no physical audio hardware)
#define AUDIO_INPUT_SAMPLE_RATE  16000
#define AUDIO_OUTPUT_SAMPLE_RATE 16000

// Display LCD (ILI9341) - SPI Interface (HSPI)
#define DISPLAY_WIDTH   320
#define DISPLAY_HEIGHT  240
#define DISPLAY_MOSI GPIO_NUM_13
#define DISPLAY_MISO GPIO_NUM_12
#define DISPLAY_SCLK GPIO_NUM_14
#define DISPLAY_CS GPIO_NUM_15
#define DISPLAY_DC GPIO_NUM_2
#define DISPLAY_RST -1  // Software reset
#define DISPLAY_BACKLIGHT_PIN GPIO_NUM_21
#define DISPLAY_BACKLIGHT_OUTPUT_INVERT false

// Display orientation
#define DISPLAY_SWAP_XY  false
#define DISPLAY_MIRROR_X false
#define DISPLAY_MIRROR_Y false
#define DISPLAY_OFFSET_X 0
#define DISPLAY_OFFSET_Y 0

// Touch Controller (XPT2046) - SPI Interface (VSPI)
#define TOUCH_MOSI GPIO_NUM_32
#define TOUCH_MISO GPIO_NUM_39
#define TOUCH_SCLK GPIO_NUM_25
#define TOUCH_CS GPIO_NUM_33
#define TOUCH_IRQ GPIO_NUM_36

// RGB LED (Active Low)
#define LED_RED GPIO_NUM_4
#define LED_GREEN GPIO_NUM_16
#define LED_BLUE GPIO_NUM_17
#define BUILTIN_LED_GPIO LED_RED  // Use red LED as default

// Buttons
#define BOOT_BUTTON_GPIO GPIO_NUM_0

// Optional peripherals
#define SD_MISO GPIO_NUM_19
#define SD_MOSI GPIO_NUM_23
#define SD_SCLK GPIO_NUM_18
#define SD_CS GPIO_NUM_5

#define LDR_GPIO GPIO_NUM_34  // Light sensor (ADC)
#define SPEAKER_GPIO GPIO_NUM_26  // PWM speaker output

// Extended GPIO connectors
// P3: GND, GPIO35, GPIO22, GPIO21
// CN1: GND, GPIO22, GPIO27, 3V3
#define EXT_GPIO_P3_1 GPIO_NUM_35
#define EXT_GPIO_P3_2 GPIO_NUM_22
#define EXT_GPIO_CN1_1 GPIO_NUM_22
#define EXT_GPIO_CN1_2 GPIO_NUM_27

#endif // _BOARD_CONFIG_H_

