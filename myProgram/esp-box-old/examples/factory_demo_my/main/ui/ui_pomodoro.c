/**
 * @file ui_pomodoro.c
 * @brief 
 * @version 0.1
 * @date 2021-12-13
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

#include <stdbool.h>
#include "app_pomodoro.h"
#include "lvgl.h"
#include "ui_main.h"

LV_FONT_DECLARE(font_en_24)
LV_FONT_DECLARE(font_en_16)

static void btn_back_pomodoro_cb(lv_event_t *event)
{
    ui_pomodoro(false);
    ui_clock(true);
}

static void btn_start_pomodoro(lv_event_t *event)
{
    ;
}

static void set_angle(void * obj, int32_t v)
{
    lv_arc_set_value(obj, v);
}

void ui_pomodoro(bool show)
{
    static lv_obj_t *widget = NULL;

    if (NULL == widget) {
        widget = lv_obj_create(lv_scr_act());
        lv_obj_set_style_radius(widget, 20, LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(widget, 0, LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(widget, lv_color_tomato(), LV_STATE_DEFAULT);
        lv_obj_set_size(widget, 290, 180);
        lv_obj_align(widget, LV_ALIGN_BOTTOM_MID, 0, -15);
        lv_obj_clear_flag(widget, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_style_shadow_width(widget, 20, LV_STATE_DEFAULT);
    }

    static lv_obj_t *btn_back = NULL;
    if (NULL == btn_back) {
        btn_back = lv_label_create(widget);
        lv_obj_add_flag(btn_back, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_ext_click_area(btn_back, 15);
        lv_label_set_text_static(btn_back, LV_SYMBOL_LEFT);
        lv_obj_set_style_text_font(btn_back, &lv_font_montserrat_16, LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(btn_back, lv_color_black(), LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(btn_back, lv_color_white(), LV_STATE_PRESSED);
        lv_obj_align(btn_back, LV_ALIGN_CENTER, -120, -48 - 20);
        lv_obj_add_event_cb(btn_back, btn_back_pomodoro_cb, LV_EVENT_CLICKED, NULL);
    }

    static lv_obj_t *label_countdown = NULL;
    if (NULL == label_countdown) {
        label_countdown = lv_label_create(widget);
        lv_label_set_text_static(label_countdown, "25:00");
        lv_obj_set_style_text_font(label_countdown, &font_en_24, 0);
        lv_obj_set_style_text_color(label_countdown, lv_color_white(), 0);
        lv_obj_align(label_countdown, LV_ALIGN_CENTER, 0, -20);
    }

    static lv_obj_t *arc_countdown = NULL;
    if (NULL == arc_countdown) {
       arc_countdown = lv_arc_create(widget);
        lv_arc_set_rotation(arc_countdown, 270);
        lv_arc_set_bg_angles(arc_countdown, 0, 360);
        lv_obj_remove_style(arc_countdown, NULL, LV_PART_KNOB);
        lv_obj_clear_flag(arc_countdown, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_center(arc_countdown);

        lv_anim_t arc;
        lv_anim_init(&arc);
        lv_anim_set_var(&arc, arc_countdown);
        lv_anim_set_exec_cb(&arc, set_angle);
        lv_anim_set_time(&arc, 3000);
        lv_anim_set_repeat_count(&arc, LV_ANIM_REPEAT_INFINITE);    /*Just for the demo*/
        arc.repeat_cnt = 1; /* 仅播放一次 */
        lv_anim_set_values(&arc, 0, 100);
        lv_anim_start(&arc);   
    }

    static lv_obj_t *btn_start = NULL;
    if (NULL == btn_start) {
        btn_start = lv_label_create(widget);
        lv_obj_set_ext_click_area(btn_start, 20);
        lv_label_set_text_static(btn_start, LV_SYMBOL_PLAY);
        lv_obj_set_style_text_font(btn_start, &lv_font_montserrat_24, LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(btn_start, lv_color_white(), LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(btn_start, lv_color_black(), LV_STATE_PRESSED);
        lv_obj_align(btn_start, LV_ALIGN_CENTER, 0, 7);
        lv_obj_add_event_cb(btn_start, btn_start_pomodoro, LV_EVENT_CLICKED, NULL);
    }

    if (show) {
        lv_obj_clear_flag(widget, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(widget, LV_OBJ_FLAG_HIDDEN);
    }
}