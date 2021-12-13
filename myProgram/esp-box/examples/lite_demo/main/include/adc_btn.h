/**
 * @file adc_btn.h
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

#pragma once

#include <stdbool.h>
#include "driver/adc_common.h"
#include "adc_btn.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ADC_BTN_NONE = -1,
    ADC_BTN_0 = 0,
    ADC_BTN_1,
    ADC_BTN_2,
    ADC_BTN_MAX,
    ADC_BTN_PREV = ADC_BTN_0,
    ADC_BTN_OK = ADC_BTN_1,
    ADC_BTN_NEXT = ADC_BTN_2,
} adc_btn_t;

typedef struct {
    int btn_id;
    bool pressed;
    void *user_data;
} adc_btn_cb_ctx_t;

typedef void (*adc_btn_cb_t)(adc_btn_cb_ctx_t *);

/**
 * @brief 
 * 
 * @param channel 
 * @return esp_err_t 
 */
esp_err_t adc_btn_init(adc1_channel_t channel);

/**
 * @brief 
 * 
 * @return adc_btn_t 
 */
adc_btn_t adc_btn_get_val(void);

#ifdef __cplusplus
}
#endif
