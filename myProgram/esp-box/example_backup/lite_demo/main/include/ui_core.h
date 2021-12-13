/**
 * @file ui_core.h
 * @brief 
 * @version 0.1
 * @date 2021-12-03
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
#include <stdint.h>

#define UI_CORE_MALLOC_STDLIB       1
#if UI_CORE_MALLOC_STDLIB
    #define UI_CORE_MALLOC_PSRAM    0
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    UI_TYPE_NONE = 0,
    UI_TYPE_MENU,
    UI_TYPE_CONTENT,
    UI_TYPE_MAX,
} ui_type_t;

typedef enum {
    UI_REQ_HIDE = 0,
    UI_REQ_SHOW = 1,
    UI_REQ_DEL = -1,
} ui_req_t;

typedef struct {
    void * caller;
    ui_req_t req;
    void * user_data;
} ui_cb_ctx_t; 

typedef void (*ui_core_cb_t)(ui_cb_ctx_t *);

typedef struct _ui_core_t {
    ui_type_t type;
    void *img_src;
    char *name;
    struct _ui_core_t *parent;
    struct _ui_core_t *child;
    struct _ui_core_t *prev;
    struct _ui_core_t *next;
    void *user_data;
    ui_core_cb_t ui_func;
} ui_core_t;

typedef struct _ui_core_t *ui_core_handle_t;

ui_core_handle_t ui_core_init(void);

ui_core_handle_t ui_core_start(void);

ui_core_handle_t ui_core_call_func(ui_core_handle_t target, void *param, void *user_data);

ui_core_handle_t ui_core_get_self_handle(void);

ui_core_handle_t ui_core_get_root_handle(void);

ui_core_handle_t ui_core_set_callback(ui_core_handle_t target, ui_core_cb_t callback);

ui_core_handle_t ui_core_add_next(ui_core_handle_t target);

ui_core_handle_t ui_core_add_child(ui_core_handle_t parent);

ui_core_handle_t ui_core_set_child(ui_core_handle_t parent, ui_core_handle_t child);

ui_core_handle_t ui_core_get_child(ui_core_handle_t parent);

ui_core_handle_t ui_core_get_prev(ui_core_handle_t target);

ui_core_handle_t ui_core_get_next(ui_core_handle_t target);

ui_core_handle_t ui_core_get_parent(ui_core_handle_t child);

#ifdef __cplusplus
}
#endif
