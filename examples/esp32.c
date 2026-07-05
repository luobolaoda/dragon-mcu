// esp32.c
// ESP32 中国龙算法示例 (ESP-IDF)
// 编译: idf.py build

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "dragon_mcu.h"

static const char* TAG = "DRAGON";
#define MAT_SIZE 16

static const dragon_atom_t weights[MAT_SIZE * MAT_SIZE] = {0};
static dragon_atom_t input[MAT_SIZE * MAT_SIZE];
static dragon_atom_t output[MAT_SIZE * MAT_SIZE];

void app_main(void) {
    dragon_init();
    ESP_LOGI(TAG, "版本: %s", dragon_version());

    for (int i = 0; i < MAT_SIZE * MAT_SIZE; i++)
        input[i] = DRAGON_MAKE(i % 3, i % 8);

    int64_t start = esp_timer_get_time();
    dragon_gemm(MAT_SIZE, MAT_SIZE, MAT_SIZE, weights, input, output);
    int64_t elapsed = esp_timer_get_time() - start;

    ESP_LOGI(TAG, "GEMM %dx%d: %lld us", MAT_SIZE, MAT_SIZE, elapsed);

    while (1) vTaskDelay(pdMS_TO_TICKS(1000));
}
