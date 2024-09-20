#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s_std.h"
#include "esp_system.h"
#include "esp_log.h"

#define I2S_NUM         (0)
#define I2S_BCK_IO      (GPIO_NUM_8)
#define I2S_WS_IO       (GPIO_NUM_6)
#define I2S_DO_IO       (GPIO_NUM_9)  // Data Out to MAX98357
#define I2S_DI_IO       (GPIO_NUM_7)  // Data In from ICS-43434
#define SAMPLE_RATE     (48000)
#define SAMPLE_BITS     (32)
#define DMA_BUF_LEN     (64)
#define DMA_NUM_BUF     (2)

static const char* TAG = "AUDIO_PASSTHROUGH";

static i2s_chan_handle_t rx_handle = NULL;
static i2s_chan_handle_t tx_handle = NULL;

static esp_err_t i2s_driver_init(void)
{
    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM, I2S_ROLE_MASTER);
    chan_cfg.auto_clear = true;
    chan_cfg.dma_desc_num = DMA_NUM_BUF;
    chan_cfg.dma_frame_num = DMA_BUF_LEN;
    
    ESP_ERROR_CHECK(i2s_new_channel(&chan_cfg, &tx_handle, &rx_handle));

    i2s_std_config_t std_cfg = {
        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(SAMPLE_RATE),
        .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(SAMPLE_BITS, I2S_SLOT_MODE_STEREO),
        .gpio_cfg = {
            .mclk = I2S_GPIO_UNUSED,
            .bclk = I2S_BCK_IO,
            .ws = I2S_WS_IO,
            .dout = I2S_DO_IO,
            .din = I2S_DI_IO,
            .invert_flags = {
                .mclk_inv = false,
                .bclk_inv = false,
                .ws_inv = false,
            },
        },
    };

    ESP_ERROR_CHECK(i2s_channel_init_std_mode(rx_handle, &std_cfg));
    ESP_ERROR_CHECK(i2s_channel_init_std_mode(tx_handle, &std_cfg));
    
    ESP_ERROR_CHECK(i2s_channel_enable(rx_handle));
    ESP_ERROR_CHECK(i2s_channel_enable(tx_handle));
    
    return ESP_OK;
}

static void audio_passthrough_task(void *args)
{
    ESP_LOGI(TAG, "Audio passthrough task started");

    size_t bytes_read = 0;
    size_t bytes_written = 0;
    int32_t samples[DMA_BUF_LEN * 2];  // Stereo buffer
    int32_t mono_samples[DMA_BUF_LEN]; // Mono buffer for processing

    while (1) {
        // Read stereo samples from microphone
        esp_err_t ret = i2s_channel_read(rx_handle, samples, sizeof(samples), &bytes_read, portMAX_DELAY);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Error reading from I2S: %s", esp_err_to_name(ret));
            continue;
        }

        // Extract left channel and prepare mono output
        for (int i = 0, j = 0; i < bytes_read / sizeof(int32_t); i += 2, j++) {
            mono_samples[j] = samples[i];  // Left channel
        }

        // Write mono samples to both left and right channels for MAX98357
        for (int i = 0, j = 0; i < DMA_BUF_LEN; i++, j += 2) {
            samples[j] = mono_samples[i];     // Left channel
            samples[j + 1] = mono_samples[i]; // Right channel (duplicate of left)
        }

        // Write processed audio to MAX98357
        ret = i2s_channel_write(tx_handle, samples, bytes_read, &bytes_written, portMAX_DELAY);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Error writing to I2S: %s", esp_err_to_name(ret));
            continue;
        }

        ESP_LOGD(TAG, "Read %d bytes, wrote %d bytes", bytes_read, bytes_written);
    }
}

void app_main(void)
{
    ESP_ERROR_CHECK(i2s_driver_init());
    ESP_LOGI(TAG, "I2S driver initialized successfully");

    xTaskCreate(audio_passthrough_task, "audio_passthrough", 4096, NULL, 5, NULL);
    ESP_LOGI(TAG, "Audio passthrough task created");
}