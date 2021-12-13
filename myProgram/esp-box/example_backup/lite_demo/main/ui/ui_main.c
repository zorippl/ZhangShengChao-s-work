/**
 * @file ui_main.c
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
#include "lvgl.h"
#include "iot_button.h"
#include "ui_core.h"
#include "ui_main.h"

#define PAGE_COUNT	(4)

LV_IMG_DECLARE(img_mic)
LV_IMG_DECLARE(img_folder)
LV_IMG_DECLARE(img_setting)
LV_IMG_DECLARE(img_about)

static int tab_count = 0;
static lv_obj_t *tabview = NULL;
// static lv_obj_t *img[PAGE_COUNT] = { [0 ... PAGE_COUNT - 1] = NULL };
// static lv_obj_t *led[PAGE_COUNT] = { [0 ... PAGE_COUNT - 1] = NULL };
static lv_obj_t *tab[PAGE_COUNT] = { [0 ... PAGE_COUNT - 1] = NULL };
// void *img_src[] = {
// 	(void *) &img_mic,
// 	(void *) &img_folder,
// 	(void *) &img_setting,
// 	(void *) &img_about,
// };

// typedef struct {
// 	void *img_src;
// 	char *name;
// } app_desc_t;

// app_desc_t app_desc[] = {
// 	{ .img_src = (void *) &img_mic, .name = "Voice Control"},
// 	{ .img_src = (void *) &img_folder, .name = "File Manager"},
// 	{ .img_src = (void *) &img_setting, .name = "Settings"},
// 	{ .img_src = (void *) &img_about, .name = "About"},
// };

static void adc_button_init(void);

// static void button_prev_cb(void *arg)
// {
//     if (tab_count != 0) {
// 		tab_count--;
// 	} else {
// 		tab_count = PAGE_COUNT - 1;
// 	}

// 	for (size_t i = 0; i < PAGE_COUNT; i++) {
// 		lv_led_off(led[i]);
// 	}
// 	lv_led_on(led[tab_count]);
// 	lv_obj_scroll_to_view_recursive(img[tab_count], LV_ANIM_ON);
// }

// static void button_ok_cb(void *arg)
// {
//     printf("OK\n");
// }

// static void button_next_cb(void *arg)
// {
//     tab_count++;
// 	if (tab_count >= PAGE_COUNT) {
// 		tab_count = 0;
// 	}

// 	for (size_t i = 0; i < PAGE_COUNT; i++) {
// 		lv_led_off(led[i]);
// 	}
// 	lv_led_on(led[tab_count]);
// 	lv_obj_scroll_to_view_recursive(img[tab_count], LV_ANIM_ON);
// 	lv_led_on(led[tab_count]);
// }

// static void tabview_event_cb(lv_event_t *event)
// {
// 	lv_obj_t *tabview = (lv_obj_t *) event->target;
// 	uint16_t count = ((lv_tabview_t *) tabview)->tab_cur;
// 	tab_count = count;
// 	for (size_t i = 0; i < PAGE_COUNT; i++) {
// 		lv_led_off(led[i]);
// 	}
// 	lv_led_on(led[tab_count]);
// }

static void button_prev_cb(void *arg)
{
	printf("Prev\n");
	ui_core_t *self = ui_core_get_self_handle();
	if (NULL != self->prev) {
		ui_core_call_func(self->prev, NULL, NULL);
	}
}

static void button_next_cb(void *arg)
{
	printf("Next\n");
	ui_core_t *self = ui_core_get_self_handle();
	if (NULL != self->next) {
		ui_core_call_func(self->next, NULL, NULL);
	}
}

static void button_back_cb(void *arg)
{
	printf("Back\n");
	ui_core_t *self = ui_core_get_self_handle();
	if (NULL != self->parent) {
		ui_core_call_func(self->parent, NULL, NULL);
	}
}

static void button_ok_cb(void *arg)
{
	printf("OK\n");
	ui_core_t *self = ui_core_get_self_handle();
	if (NULL != self->child) {
		ui_core_call_func(self->child, NULL, NULL);
	}
}

static void ui_main_page(ui_cb_ctx_t *ctx);

void ui_main_start(void)
{
	/* Use ADC button to control UI */
	adc_button_init();

    // lv_obj_clear_flag(lv_scr_act(), LV_OBJ_FLAG_SCROLLABLE);

	/* Create screen */
	// lv_obj_t *scr = lv_scr_act();
	// lv_obj_set_size(scr, 320, 240);
	// lv_obj_clear_flag(scr, LV_OBJ_FLAG_SCROLLABLE);
	// lv_obj_set_style_radius(scr, 0, LV_STATE_DEFAULT);
	// lv_obj_set_style_border_width(scr, 0, LV_STATE_DEFAULT);
	
	/* Create a Tab view object */
    // tabview = lv_tabview_create(lv_scr_act(), LV_DIR_TOP, 20);
	// lv_obj_clear_flag(tabview, LV_OBJ_FLAG_SCROLLABLE);
	// lv_obj_set_style_bg_color(tabview, lv_obj_get_style_bg_color(lv_scr_act(), LV_PART_MAIN), LV_STATE_DEFAULT);
	// lv_obj_set_size(tabview, 320, 200);
	// lv_obj_center(tabview);
	// // lv_obj_add_event_cb(tabview, tabview_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

	// for (size_t i = 0; i < PAGE_COUNT; i++) {
	// 	lv_tabview_add_tab(tabview, "001");
	// }




	/* **************** UI CORE VERSION **************** */
	/* Create root object */
	ui_core_t *root = ui_core_init();
	ui_core_set_callback(root, ui_main_page);

	ui_core_t *dev_ctrl = ui_core_add_child(root);
	ui_core_set_callback(dev_ctrl, ui_dev_func);
	dev_ctrl->user_data = lv_scr_act();

	ui_core_t *voice_ctrl = ui_core_add_next(dev_ctrl);
	ui_core_set_callback(voice_ctrl, ui_voice_func);
	voice_ctrl->user_data = lv_scr_act();

	ui_core_t *setting = ui_core_add_next(voice_ctrl);
	ui_core_set_callback(setting, ui_setting_func);
	setting->user_data = lv_scr_act();

	/* Start UI core */
	ui_core_start();
}

static void ui_main_page(ui_cb_ctx_t *ctx)
{
	static lv_obj_t *img_logo = NULL;
	static lv_obj_t *img_text = NULL;

	LV_IMG_DECLARE(esp_logo)
	LV_IMG_DECLARE(esp_text)
	if (NULL == img_logo) {
		img_logo = lv_img_create(lv_scr_act());
		lv_img_set_src(img_logo, &esp_logo);
		lv_obj_align(img_logo, LV_ALIGN_CENTER, 0, -40);
	}

	if (NULL == img_text) {
		img_text = lv_img_create(lv_scr_act());
		lv_img_set_src(img_text, &esp_text);
		lv_obj_align_to(img_text, img_logo, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
	}

	switch (ctx->req) {
	case UI_REQ_SHOW:
		lv_obj_clear_flag(img_logo, LV_OBJ_FLAG_HIDDEN);
		lv_obj_clear_flag(img_text, LV_OBJ_FLAG_HIDDEN);
		break;
	case UI_REQ_HIDE:
		lv_obj_add_flag(img_logo, LV_OBJ_FLAG_HIDDEN);
		lv_obj_add_flag(img_text, LV_OBJ_FLAG_HIDDEN);
		break;
	default:
		break;
	}
}
