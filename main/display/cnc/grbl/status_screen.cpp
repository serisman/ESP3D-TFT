/*
  esp3d_tft

  Copyright (c) 2022 Luc Lebosse. All rights reserved.

  This code is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This code is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <list>
#include <string>

#include "esp3d_hal.h"
#include "esp3d_log.h"
#include "esp3d_styles.h"
#include "esp3d_tft_ui.h"
#include "esp3d_values.h"

/**********************
 *  STATIC PROTOTYPES
 **********************/
// Create style for buttons
lv_style_t style_button_back;
std::list<std::string> ui_status_screen_list;
lv_obj_t *status_list = nullptr;
lv_timer_t *delay_timer = NULL;

#define MAX_STATUS_SCREEN_LIST 10
void main_screen();
void status_screen();

bool status_list_cb(ESP3DValuesIndex index, const char *value,
                    ESP3DValuesCbAction action) {
  if (action == ESP3DValuesCbAction::Add ||
      action == ESP3DValuesCbAction::Update) {
    esp3d_log("status_list_cb: %d", (int)status_list);
    if (ui_status_screen_list.size() > MAX_STATUS_SCREEN_LIST) {
      ui_status_screen_list.pop_back();
    }
    ui_status_screen_list.push_front(value);
    if (esp3dTftui.get_current_screen() == ESP3DScreenType::status_list) {
      status_screen();
    }
  } else if (action == ESP3DValuesCbAction::Clear) {
    ui_status_screen_list.clear();
  } else {
    return false;
  }
  return true;
}

static void event_handler_button_back(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_CLICKED) {
    if (!delay_timer) {
      esp3d_log("Clicked");
      main_screen();
    }
  }
}

void status_screen() {
  esp3dTftui.set_current_screen(ESP3DScreenType::status_list);
  // Screen creation
  esp3d_log("Main screen creation");
  lv_obj_t *ui_status_screen = lv_obj_create(NULL);
  const ESP3DValuesDescription *status_bar_desc =
      esp3dTftValues.get_description(ESP3DValuesIndex::status_bar_label);
  // the control is deleted but not the label is not set to null so we need to
  // do it to avoid crash if we try to update it
  if (status_bar_desc != nullptr &&
      ESP3DScreenType::status_list == esp3dTftui.get_current_screen()) {
    status_bar_desc->label = nullptr;
  }
  // Apply background color
  apply_style(ui_status_screen, ESP3DStyleType::main_bg);
  // Create screen container
  lv_obj_t *ui_status_screen_container = lv_obj_create(ui_status_screen);
  apply_style(ui_status_screen_container, ESP3DStyleType::col_container);
  lv_obj_set_height(ui_status_screen_container, LV_VER_RES);
  lv_obj_set_width(ui_status_screen_container, LV_HOR_RES);

  status_list = lv_list_create(ui_status_screen_container);
  apply_style(status_list, ESP3DStyleType::status_list);
  lv_obj_set_align(status_list, LV_ALIGN_TOP_MID);
  for (auto &line : ui_status_screen_list) {
    lv_list_add_btn(status_list, "", line.c_str());
  }

  lv_obj_t *btn_back = lv_btn_create(ui_status_screen_container);
  apply_style(btn_back, ESP3DStyleType::embedded_button);
  lv_obj_set_width(btn_back, LV_HOR_RES);
  lv_obj_t *label_btn_back = lv_label_create(btn_back);
  lv_label_set_text(label_btn_back, LV_SYMBOL_UP);
  lv_obj_set_align(label_btn_back, LV_ALIGN_CENTER);
  lv_obj_set_align(btn_back, LV_ALIGN_BOTTOM_MID);
  lv_obj_add_event_cb(btn_back, event_handler_button_back, LV_EVENT_CLICKED,
                      NULL);
  lv_obj_update_layout(btn_back);
  lv_obj_set_height(status_list, LV_VER_RES - lv_obj_get_height(btn_back));
  lv_obj_set_width(status_list, LV_HOR_RES);
  // Display new screen and delete old one
  lv_obj_t *ui_current_screen = lv_scr_act();
  lv_scr_load(ui_status_screen);
  lv_obj_del(ui_current_screen);
}