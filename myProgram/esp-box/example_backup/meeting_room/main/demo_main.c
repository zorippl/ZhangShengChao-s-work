/**
 * @file demo_main.c
 * @brief 
 * @version 0.1
 * @date 2021-06-23
 * 
 * @copyright Copyright 2021 Espressif Systems (Shanghai) Co. Ltd.
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *               http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 */

#include "app_audio.h"
#include "app_ir.h"
#include "app_sr.h"
#include "app_espnow.h"
#include "bsp_board.h"
#include "bsp_lcd.h"
#include "bsp_storage.h"
#include "bsp_tp.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "lv_demo.h"
#include "lv_port.h"
#include "lvgl.h"
#include "ui_main.h"

void app_main(void)
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK( nvs_flash_erase() );
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );

    ESP_ERROR_CHECK(bsp_board_init());
    ESP_ERROR_CHECK(bsp_board_power_ctrl(POWER_MODULE_AUDIO, true));
    ESP_ERROR_CHECK(bsp_spiffs_init("model", "/srmodel", 4));
    ESP_ERROR_CHECK(bsp_spiffs_init("storage", "/spiffs", 10));

    /* Initialize WS2812 LED(s) */
    // ESP_ERROR_CHECK(app_led_init(GPIO_RMT_LED));


    /* Initialize LCD and GUI */
    ESP_ERROR_CHECK(bsp_lcd_init());
    ESP_ERROR_CHECK(bsp_tp_init());
    ESP_ERROR_CHECK(lv_port_init());
    
    // example_wifi_init();
    app_espnow_init();

    app_audio_load(ANNOUNCEMENT);
    app_audio_load(COMINGIN);
    app_audio_load(WARNING);
    app_audio_load(WAKE);

    if(xTaskCreate(app_audio_task, "app_audio", 4096, NULL, 5, NULL) != pdTRUE) {
        ESP_LOGE("app_main", "create audio task failed");
    }
    rmt_ir_init();
    ESP_ERROR_CHECK(ui_main_start());
    
    /* Start speech recognition task. I2S is initialized in task pinned to core 1 */
    ESP_ERROR_CHECK(app_sr_start(false));

    /* Run LVGL task handler */
    while (vTaskDelay(2), true) {
        lv_task_handler();
    }
}
