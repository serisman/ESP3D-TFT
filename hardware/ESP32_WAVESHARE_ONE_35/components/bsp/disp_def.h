// Display definitions for ESP32_WAVESHARE_ONE_35
// Display driver ILI9486 (SPI to 16-bit parallel)
#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#define TFT_DISPLAY_CONTROLLER "ILI9486"

#include "ili9486.h"

/*
PORTRAIT                0
PORTRAIT_INVERTED       1
LANDSCAPE               2
LANDSCAPE_INVERTED      3
*/

#define DISP_ORIENTATION 2  // landscape

#if DISP_ORIENTATION == 2 || DISP_ORIENTATION == 3  // landscape mode
#define DISP_HOR_RES_MAX 480
#define DISP_VER_RES_MAX 320
#else  // portrait mode
#define DISP_HOR_RES_MAX 320
#define DISP_VER_RES_MAX 480
#endif

#define DISP_USE_DOUBLE_BUFFER (true)

#if WITH_PSRAM
  // 1/10 (32-line) buffer (30KB) in external PSRAM
  #define DISP_BUF_SIZE (DISP_HOR_RES_MAX * DISP_VER_RES_MAX / 10)
  #define DISP_BUF_MALLOC_TYPE  MALLOC_CAP_SPIRAM
#else
  // 1/20 (16-line) buffer (15KB) in internal DRAM
  #define DISP_BUF_SIZE (DISP_HOR_RES_MAX * 16)
  #define DISP_BUF_MALLOC_TYPE  MALLOC_CAP_DMA
#endif  // WITH_PSRAM

#define DISP_BUF_SIZE_BYTES    (DISP_BUF_SIZE * 2)

esp_lcd_panel_dev_config_t disp_panel_cfg = {
    .reset_gpio_num = 0, // GPIO 0
    .color_space = ESP_LCD_COLOR_SPACE_BGR,
    .bits_per_pixel = 16,
};

// SPI (shared with Touch, SD)
// NOTE: Waveshare TFT w/ ILI9486 uses a SPI to 16-bit parallel bus that requires 16 clks even if only sending 8-bit cmd/param values
esp_lcd_panel_io_spi_config_t disp_spi_cfg = {
    .dc_gpio_num = 4, // GPIO 4
    .cs_gpio_num = 5, // GPIO 5
    .pclk_hz = 32 * 1000 * 1000,
    .lcd_cmd_bits = 16,
    .lcd_param_bits = 16,
    .spi_mode = 0,
    .trans_queue_depth = 2,
};

#ifdef __cplusplus
} /* extern "C" */
#endif
