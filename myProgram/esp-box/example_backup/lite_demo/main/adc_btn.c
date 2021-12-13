/**
 * @file adc_btn.c
 * @brief 
 * @version 0.1
 * @date 2021-12-02
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

#include <stdint.h>
#include <stdio.h>
#include "adc_btn.h"
#include "driver/adc_common.h"
#include "esp_adc_cal.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static adc1_channel_t s_channel = ADC1_CHANNEL_MAX;
static adc_btn_cb_t s_adc_btn_cb = NULL;

static void adc_read_task(void *pvParam);

esp_err_t adc_btn_init(adc1_channel_t channel)
{
    // esp_adc_cal_characteristics_t adc1_chars;
    // ESP_ERROR_CHECK(esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP_FIT));
    // ESP_ERROR_CHECK(esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars));

    s_channel = channel;
    ESP_ERROR_CHECK(adc1_config_width(ADC_WIDTH_BIT_DEFAULT));
    ESP_ERROR_CHECK(adc1_config_channel_atten(s_channel, ADC_ATTEN_DB_11));

    xTaskCreatePinnedToCore(
        (TaskFunction_t)        adc_read_task,
        (const char * const)    "ADC Read Task",
        (const uint32_t)        4 * 1024,
        (void * const)          NULL,
        (UBaseType_t)           1,
        (TaskHandle_t * const)  NULL,
        (const BaseType_t)      0);

    return ESP_OK;
}

adc_btn_t adc_btn_get_val(void)
{
    const uint32_t err = 50;
    const uint32_t voltage_table[] = {
        810, 1555, 2290,
    };
    static adc_btn_t prev_btn_id = ADC_BTN_NONE;


    /* Get voltage of ADC button pin */
    if (ADC1_CHANNEL_MAX == s_channel) {
        return ADC_BTN_NONE;
    }
    uint32_t voltage = adc1_get_raw(s_channel);
    // uint32_t voltage = esp_adc_cal_raw_to_voltage(adc1_get_raw(s_channel), &adc1_chars);

    /* Match button key value */
    for (size_t i = 0; i < sizeof(voltage_table) / sizeof(voltage_table[i]); i++) {
        if ((voltage > voltage_table[i] - err) && (voltage < voltage_table[i] + err)) {
            if (prev_btn_id == )
            if (esp_ptr_executable(s_adc_btn_cb)) {
                adc_btn_cb_ctx_t adc_btn_cb_ctx;
                adc_btn_cb_ctx.btn_id = i;
                s_adc_btn_cb(&adc_btn_cb_ctx);
            }
            return (adc_btn_t) i;
        }
    }

    return ADC_BTN_NONE;
}

esp_err_t adc_btn_register_callback(adc_btn_cb_t *cb)
{
    //
}

static void adc_read_task(void *pvParam)
{
    (void) pvParam;

    while (true) {
        adc_btn_get_val();
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
