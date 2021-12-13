/**
 * @file bsp_btn.c
 * @brief 
 * @version 0.1
 * @date 2021-12-10
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
#include "driver/adc.h"
#include "esp_err.h"
#include "iot_button.h"

/* **************** BOARD CAPS **************** */
#define INDEV_BTN_GPIO      0
#define INDEV_BTN_ADC_TAB   1, 2, 3,

#define INDEV_BTN_ADC       1
#define INDEV_BTN_ADC_CHAN  ADC1_CHANNEL_0
#define INDEV_BTN_ADC_TAB   { 2971, 2427, 957 }

static const uint32_t voltage_err = 50;
static const uint32_t voltage_table[] = INDEV_BTN_ADC_TAB;
static button_handle_t iot_btn[sizeof(voltage_table) / sizeof(voltage_table[0])] = {
    [0 ... sizeof(iot_btn) / sizeof(iot_btn[0]) - 1] = NULL };

esp_err_t adc_button_init(void)
{
#if !INDEV_BTN_ADC
    return ESP_ERR_NOT_SUPPORTED;
#else
    static button_config_t btn_cfg = {
        .type = BUTTON_TYPE_ADC,
        .adc_button_config = {
            .adc_channel = INDEV_BTN_ADC_CHAN,
        }
    };

    for (size_t i = 0; i < sizeof(voltage_table) / sizeof(voltage_table[0]); i++) {
        btn_cfg.adc_button_config.button_index = i;
        btn_cfg.adc_button_config.min = voltage_table[i] - voltage_err;
        btn_cfg.adc_button_config.max = voltage_table[i] + voltage_err;
        iot_btn[i] = iot_button_create(&btn_cfg);
    }

    return ESP_OK;
#endif
}

esp_err_t bsp_btn_register_callback(size_t btn_id, button_event_t event, button_cb_t callback)
{
#if !INDEV_BTN_ADC
    return ESP_ERR_NOT_SUPPORTED;
#else
    if (NULL == callback) {
        return iot_button_unregister_cb(iot_btn[btn_id], event);
    }

    return iot_button_register_cb(iot_btn[btn_id], event, callback);
#endif
}
