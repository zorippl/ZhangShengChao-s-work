/**
 * @file ui_dev.c
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

#include "lvgl.h"
#include "ui_core.h"

LV_IMG_DECLARE(img_flashlight)
LV_IMG_DECLARE(img_mic)
LV_IMG_DECLARE(img_setting)

void ui_dev_func(ui_cb_ctx_t *ctx)
{
    static lv_obj_t *img = NULL;
    static lv_obj_t *lab = NULL;
    ui_core_t *self = ui_core_get_self_handle();
    lv_obj_t *parent = (lv_obj_t *) self->user_data;

    if (NULL == img) {
        img = lv_img_create(parent);
        lv_img_set_src(img, &img_flashlight);
        lv_obj_align(img, LV_ALIGN_CENTER, 0, -20);
    }

    if (NULL == lab) {
        lab = lv_label_create(parent);
        lv_obj_set_style_text_font(lab, &lv_font_montserrat_24, LV_STATE_DEFAULT);
        lv_label_set_text_static(lab, "Light Control");
        lv_obj_align_to(lab, img, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
    }
    switch (ctx->req) {
    case UI_REQ_SHOW:
        lv_obj_clear_flag(img, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(lab, LV_OBJ_FLAG_HIDDEN);
        break;
    case UI_REQ_HIDE:
        lv_obj_add_flag(img, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(lab, LV_OBJ_FLAG_HIDDEN);
        break;
    default:
        break;
    }
}

void ui_voice_func(ui_cb_ctx_t *ctx)
{
    static lv_obj_t *img = NULL;
    static lv_obj_t *lab = NULL;
    ui_core_t *self = ui_core_get_self_handle();
    lv_obj_t *parent = (lv_obj_t *) self->user_data;

    if (NULL == img) {
        img = lv_img_create(parent);
        lv_img_set_src(img, &img_mic);
        lv_obj_align(img, LV_ALIGN_CENTER, 0, -20);
    }

    if (NULL == lab) {
        lab = lv_label_create(parent);
        lv_obj_set_style_text_font(lab, &lv_font_montserrat_24, LV_STATE_DEFAULT);
        lv_label_set_text_static(lab, "Voice Control");
        lv_obj_align_to(lab, img, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
    }
    switch (ctx->req) {
    case UI_REQ_SHOW:
        lv_obj_clear_flag(img, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(lab, LV_OBJ_FLAG_HIDDEN);
        break;
    case UI_REQ_HIDE:
        lv_obj_add_flag(img, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(lab, LV_OBJ_FLAG_HIDDEN);
        break;
    default:
        break;
    }
}

void ui_setting_func(ui_cb_ctx_t *ctx)
{
    static lv_obj_t *img = NULL;
    static lv_obj_t *lab = NULL;
    ui_core_t *self = ui_core_get_self_handle();
    lv_obj_t *parent = (lv_obj_t *) self->user_data;

    if (NULL == img) {
        img = lv_img_create(parent);
        lv_img_set_src(img, &img_setting);
        lv_obj_align(img, LV_ALIGN_CENTER, 0, -20);
    }

    if (NULL == lab) {
        lab = lv_label_create(parent);
        lv_obj_set_style_text_font(lab, &lv_font_montserrat_24, LV_STATE_DEFAULT);
        lv_label_set_text_static(lab, "Setting");
        lv_obj_align_to(lab, img, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
    }
    switch (ctx->req) {
    case UI_REQ_SHOW:
        lv_obj_clear_flag(img, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(lab, LV_OBJ_FLAG_HIDDEN);
        break;
    case UI_REQ_HIDE:
        lv_obj_add_flag(img, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(lab, LV_OBJ_FLAG_HIDDEN);
        break;
    default:
        break;
    }
}
