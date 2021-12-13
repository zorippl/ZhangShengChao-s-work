/**
 * @file ui_main.h
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

#include "ui_core.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Start UI
 * 
 */
void ui_main_start(void);

/**
 * @brief 
 * 
 * @param ctx 
 */
void ui_dev_func(ui_cb_ctx_t *ctx);

void ui_voice_func(ui_cb_ctx_t *ctx);

void ui_setting_func(ui_cb_ctx_t *ctx);

#ifdef __cplusplus
}
#endif
