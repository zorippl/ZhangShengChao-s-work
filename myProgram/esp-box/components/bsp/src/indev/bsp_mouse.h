/**
 * @file bsp_mouse.h
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

#pragma once

#include <stdbool.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int32_t x;
    int32_t y;
    bool press;
} bsp_mouse_state_t;

typedef struct {
    bool init_ble_service;
} bsp_mouse_conf_t;


esp_err_t bsp_mouse_get_state(bsp_mouse_state_t *state);

esp_err_t bsp_mouse_init(bsp_mouse_conf_t *conf);

esp_err_t bsp_mouse_init_default(void);



#ifdef __cplusplus
}
#endif
