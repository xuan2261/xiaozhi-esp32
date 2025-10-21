#include "wifi_board.h"
#include "codecs/dummy_audio_codec.h"
#include "display/lcd_display.h"
#include "button.h"
#include "config.h"
#include "led/led.h"
#include "backlight.h"
#include "application.h"

#include <esp_log.h>
#include <esp_lcd_panel_vendor.h>
#include <esp_lcd_panel_io.h>
#include <esp_lcd_panel_ops.h>
#include <driver/spi_master.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define TAG "ESP32-2432S028"

// Simple GPIO LED class for ESP32 (active low)
class SimpleLed : public Led {
private:
    gpio_num_t gpio_;
    bool inverted_;

public:
    SimpleLed(gpio_num_t gpio, bool inverted = false) : gpio_(gpio), inverted_(inverted) {
        gpio_config_t io_conf = {};
        io_conf.intr_type = GPIO_INTR_DISABLE;
        io_conf.mode = GPIO_MODE_OUTPUT;
        io_conf.pin_bit_mask = (1ULL << gpio);
        io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
        io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
        gpio_config(&io_conf);
        TurnOff();
    }

    void TurnOn() {
        gpio_set_level(gpio_, inverted_ ? 0 : 1);
    }

    void TurnOff() {
        gpio_set_level(gpio_, inverted_ ? 1 : 0);
    }

    void OnStateChanged() override {
        // Simple implementation: just turn on
        TurnOn();
    }
};

class Esp32_2432S028_Board : public WifiBoard {
private:
    Button boot_button_;
    LcdDisplay* display_;
    esp_lcd_panel_io_handle_t panel_io_;
    esp_lcd_panel_handle_t panel_;

    void InitializeSpi() {
        ESP_LOGI(TAG, "Initialize SPI bus for LCD");
        spi_bus_config_t buscfg = {};
        buscfg.mosi_io_num = DISPLAY_MOSI;
        buscfg.miso_io_num = DISPLAY_MISO;
        buscfg.sclk_io_num = DISPLAY_SCLK;
        buscfg.quadwp_io_num = GPIO_NUM_NC;
        buscfg.quadhd_io_num = GPIO_NUM_NC;
        buscfg.max_transfer_sz = DISPLAY_WIDTH * DISPLAY_HEIGHT * sizeof(uint16_t);
        ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO));
    }

    void InitializeIli9341Display() {
        ESP_LOGI(TAG, "Initialize ILI9341 LCD display");

        // Initialize panel IO (SPI)
        esp_lcd_panel_io_spi_config_t io_config = {};
        io_config.cs_gpio_num = DISPLAY_CS;
        io_config.dc_gpio_num = DISPLAY_DC;
        io_config.spi_mode = 0;
        io_config.pclk_hz = 40 * 1000 * 1000;  // 40 MHz
        io_config.trans_queue_depth = 10;
        io_config.lcd_cmd_bits = 8;
        io_config.lcd_param_bits = 8;
        ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi(SPI2_HOST, &io_config, &panel_io_));

        // Initialize LCD panel driver
        // Note: ILI9341 is compatible with ST7789 driver in most cases
        esp_lcd_panel_dev_config_t panel_config = {};
        panel_config.reset_gpio_num = DISPLAY_RST;
        panel_config.rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR;  // ILI9341 uses BGR
        panel_config.bits_per_pixel = 16;

        // Use ST7789 driver as fallback (compatible with ILI9341)
        ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(panel_io_, &panel_config, &panel_));

        // Reset and initialize panel
        ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_));
        ESP_ERROR_CHECK(esp_lcd_panel_init(panel_));

        // Set orientation
        ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(panel_, DISPLAY_SWAP_XY));
        ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_, DISPLAY_MIRROR_X, DISPLAY_MIRROR_Y));
        ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_, false));

        // Create display object
        display_ = new SpiLcdDisplay(panel_io_, panel_,
                                    DISPLAY_WIDTH, DISPLAY_HEIGHT,
                                    DISPLAY_OFFSET_X, DISPLAY_OFFSET_Y,
                                    DISPLAY_MIRROR_X, DISPLAY_MIRROR_Y, DISPLAY_SWAP_XY);

        ESP_LOGI(TAG, "LCD display initialized successfully");
    }

    void InitializeButtons() {
        boot_button_.OnClick([this]() {
            auto& app = Application::GetInstance();
            app.ToggleChatState();
        });
    }

public:
    Esp32_2432S028_Board() : boot_button_(BOOT_BUTTON_GPIO) {
        ESP_LOGI(TAG, "Initializing ESP32-2432S028 (Cheap Yellow Display) Board");
        InitializeSpi();
        InitializeIli9341Display();
        InitializeButtons();
        GetBacklight()->RestoreBrightness();
        ESP_LOGI(TAG, "Board initialization complete");
    }

    virtual ~Esp32_2432S028_Board() {
        if (display_) {
            delete display_;
        }
    }

    virtual std::string GetBoardType() override {
        return "ESP32-2432S028";
    }

    virtual Led* GetLed() override {
        // Use red LED (active low)
        static SimpleLed led(BUILTIN_LED_GPIO, true);  // true = inverted (active low)
        return &led;
    }

    virtual AudioCodec* GetAudioCodec() override {
        // This board doesn't have audio codec hardware
        // Use dummy codec for compatibility
        static DummyAudioCodec audio_codec(AUDIO_INPUT_SAMPLE_RATE, AUDIO_OUTPUT_SAMPLE_RATE);
        return &audio_codec;
    }

    virtual Display* GetDisplay() override {
        return display_;
    }
    
    virtual Backlight* GetBacklight() override {
        static PwmBacklight backlight(DISPLAY_BACKLIGHT_PIN, DISPLAY_BACKLIGHT_OUTPUT_INVERT);
        return &backlight;
    }
};

DECLARE_BOARD(Esp32_2432S028_Board);

