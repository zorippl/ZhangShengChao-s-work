/**
 * @file ui_core.c
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

#include <string.h>
#include "esp_check.h"
#include "ui_core.h"
#include "soc/soc_memory_layout.h"

#if UI_CORE_MALLOC_STDLIB
    #include <stdlib.h>
#else
    #include "esp_heap_caps.h"
#endif

static const char *TAG = "ui_core";
static ui_core_handle_t root_handle = NULL;
static ui_core_handle_t cur_handle = NULL;

static void *ui_core_malloc(size_t size)
{
    void *ptr = NULL;

#if UI_CORE_MALLOC_STDLIB
    ptr = malloc(size);
#elif UI_CORE_MALLOC_PSRAM
    ptr = heap_caps_malloc(size, MALLOC_CAP_SPIRAM);
#else
    ptr = heap_caps_malloc(size, MALLOC_CAP_INTERNAL);
#endif

    return ptr;
}

ui_core_handle_t ui_core_init(void)
{
    if (NULL == root_handle) {
        root_handle = (ui_core_t *) ui_core_malloc(sizeof(ui_core_t));
    }

    ESP_RETURN_ON_FALSE(NULL != root_handle, NULL, TAG, "NO_MEM");
    memset(root_handle, 0, sizeof(ui_core_t));
    return root_handle;
}

ui_core_handle_t ui_core_start(void)
{
    ui_core_t *self = (ui_core_t *) root_handle;
    ESP_RETURN_ON_FALSE(NULL != root_handle, NULL, TAG, "INVALID_OBJ");
    cur_handle = self;
    ui_cb_ctx_t ctx = {
        .caller = NULL,
        .req = UI_REQ_SHOW,
        .user_data = NULL,
    };
    self->ui_func(&ctx);

    return self;
}

ui_core_handle_t ui_core_call_func(ui_core_handle_t target, void *param, void *user_data)
{
    ui_core_t *self = (ui_core_t *) target;
    ui_core_t *cur = (ui_core_t *) cur_handle;
    ESP_RETURN_ON_FALSE(NULL != target, NULL, TAG, "INVALID_OBJ");
    ESP_RETURN_ON_FALSE(NULL != cur_handle, NULL, TAG, "INVALID_OBJ");
    
    ui_cb_ctx_t ctx = {
        .caller = cur,
        .req = UI_REQ_HIDE,
        .user_data = user_data,
    };
    if (esp_ptr_executable(cur->ui_func)) {
        cur->ui_func(&ctx);
    }

    ctx.req = UI_REQ_SHOW;
    cur_handle = self;
    if (esp_ptr_executable(self->ui_func)) {
        self->ui_func(&ctx);
    }
    return self;
}

ui_core_handle_t ui_core_get_self_handle(void)
{
    return cur_handle;
}

ui_core_handle_t ui_core_get_root_handle(void)
{
    return root_handle;
}

ui_core_handle_t ui_core_set_callback(ui_core_handle_t target, ui_core_cb_t callback)
{
    ui_core_t *self = (ui_core_t *) target;
    ESP_RETURN_ON_FALSE(NULL != self, NULL, TAG, "INVALID_OBJ");
    self->ui_func = callback;
    return self;
}

ui_core_handle_t ui_core_add_next(ui_core_handle_t target)
{
    ui_core_t *self = (ui_core_t *) target;
    ui_core_t *next = ui_core_malloc(sizeof(ui_core_t));
    ESP_RETURN_ON_FALSE(NULL != next, NULL, TAG, "NO_MEM");
    memset(next, 0, sizeof(ui_core_t));
    self->next = next;
    next->prev = self;
    next->parent = self->parent;
    return next;
}

ui_core_handle_t ui_core_add_child(ui_core_handle_t target)
{
    ui_core_t *self = (ui_core_t *) target;
    ui_core_t *child = ui_core_malloc(sizeof(ui_core_t));
    ESP_RETURN_ON_FALSE(NULL != child, NULL, TAG, "NO_MEM");
    memset(child, 0, sizeof(ui_core_t));
    self->child = child;
    child->parent = self;
    return child;
}

ui_core_handle_t ui_core_set_child(ui_core_handle_t target, ui_core_handle_t child)
{
    ui_core_t *self = (ui_core_t *) target;
    ESP_RETURN_ON_FALSE(NULL != self, NULL, TAG, "INVALID_OBJ");
    self->child = child;
    return target;
}

ui_core_handle_t ui_core_get_child(ui_core_handle_t target)
{
    ui_core_t *self = (ui_core_t *) target;
    ESP_RETURN_ON_FALSE(NULL != self, NULL, TAG, "INVALID_OBJ");
    return self->child;
}

ui_core_handle_t ui_core_get_prev(ui_core_handle_t target)
{
    ui_core_t *self = (ui_core_t *) target;
    ESP_RETURN_ON_FALSE(NULL != self, NULL, TAG, "INVALID_OBJ");
    return self->prev;
}

ui_core_handle_t ui_core_get_next(ui_core_handle_t target)
{
    ui_core_t *self = (ui_core_t *) target;
    ESP_RETURN_ON_FALSE(NULL != self, NULL, TAG, "INVALID_OBJ");
    return self->next;
}

ui_core_handle_t ui_core_get_parent(ui_core_handle_t target)
{
    ui_core_t *self = (ui_core_t *) target;
    ESP_RETURN_ON_FALSE(NULL != self, NULL, TAG, "INVALID_OBJ");
    return self->parent;
}
