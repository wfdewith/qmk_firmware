/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include "tetris.h"

#ifdef OLED_ENABLE
void process_record_user_oled(uint16_t keycode, keyrecord_t *record);
#endif

enum crkbd_layers {
  _COLEMAK = 0,
  _QWERTY,
  _DVORAK,
  _LOWER,
  _RAISE,
  _CONFIG,
  _FN,
  _TETRIS,
};

enum crkbd_keycodes {
  QWERTY = SAFE_RANGE,
  COLEMAK,
  DVORAK,
  VOMIT,
  TETRIS,
  TET_EX,
  TET_LFT,
  TET_RGT,
  TET_CW,
  TET_CCW,
};

#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)
#define CTL_ESC LCTL_T(KC_ESC)
#define QWE_GUI LM(_QWERTY, MOD_LGUI)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_COLEMAK] = LAYOUT_split_3x6_3(
  KC_TAB,  KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,                         KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN, KC_DEL,
  CTL_ESC, KC_A,    KC_R,    KC_S,    KC_T,    KC_D,                         KC_H,    KC_N,    KC_E,    KC_I,    KC_O,    KC_QUOT,
  KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_ENT,
                                      QWE_GUI, LOWER,   KC_BSPC,    KC_SPC,  RAISE,   KC_LALT
),

[_QWERTY] = LAYOUT_split_3x6_3(
  _______, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    _______,
  _______, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, _______,
  _______, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, _______,
                                      _______, _______, _______,    _______, _______, _______
),

[_DVORAK] = LAYOUT_split_3x6_3(
  _______, KC_QUOT, KC_COMM, KC_DOT,  KC_P,    KC_Y,                         KC_F,    KC_G,    KC_C,    KC_R,    KC_L,    _______,
  _______, KC_A,    KC_O,    KC_E,    KC_U,    KC_I,                         KC_D,    KC_H,    KC_T,    KC_N,    KC_S,    KC_SLSH,
  _______, KC_SCLN, KC_Q,    KC_J,    KC_K,    KC_X,                         KC_B,    KC_M,    KC_W,    KC_V,    KC_Z,    _______,
                                      _______, _______, _______,    _______, _______, _______
),

[_LOWER] = LAYOUT_split_3x6_3(
  XXXXXXX, XXXXXXX, XXXXXXX, KC_LBRC, KC_RBRC, KC_BSLS,                      KC_PIPE, KC_LCBR, KC_RBRC, XXXXXXX, XXXXXXX, XXXXXXX,
  _______, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                      KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, XXXXXXX,
  _______, XXXXXXX, XXXXXXX, KC_TILD, KC_MINS, KC_UNDS,                      KC_PLUS, KC_EQL,  KC_GRV,  XXXXXXX, XXXXXXX, XXXXXXX,
                                      _______, _______, XXXXXXX,    XXXXXXX, _______, _______
),

[_RAISE] = LAYOUT_split_3x6_3(
  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                        KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
  _______, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    XXXXXXX,
  _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, XXXXXXX,
                                      _______, _______, XXXXXXX,    XXXXXXX, _______, _______
),

[_CONFIG] = LAYOUT_split_3x6_3(
  QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, VOMIT,                        XXXXXXX, XXXXXXX, XXXXXXX, QWERTY,  COLEMAK, DVORAK,
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, TETRIS,                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                      XXXXXXX, _______, XXXXXXX,    XXXXXXX, _______, XXXXXXX
),

[_TETRIS] = LAYOUT_split_3x6_3(
  TET_EX,  XXXXXXX, XXXXXXX, TET_CW,  XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  XXXXXXX, XXXXXXX, TET_LFT, TET_CCW, TET_RGT, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                      XXXXXXX, _______, XXXXXXX,    XXXXXXX, _______, XXXXXXX
)

};

layer_state_t layer_state_set_user(layer_state_t state) {
  return update_tri_layer_state(state, _LOWER, _RAISE, _CONFIG);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        set_single_persistent_default_layer(_QWERTY);
      }
      return false;
    case COLEMAK:
      if (record->event.pressed) {
        set_single_persistent_default_layer(_COLEMAK);
      }
      return false;
    case DVORAK:
      if (record->event.pressed) {
        set_single_persistent_default_layer(_DVORAK);
      }
      return false;
    case VOMIT:
      if (record->event.pressed) {
        rgb_matrix_toggle_noeeprom();
        rgb_matrix_mode_noeeprom(RGB_MATRIX_CYCLE_LEFT_RIGHT);
        rgb_matrix_set_speed_noeeprom(255);
      }
      return false;
    case TETRIS:
      if (record->event.pressed) {
        layer_on(_TETRIS);
        tetris_init();
      }
      return false;
    case TET_EX:
      if (record->event.pressed) {
        layer_off(_TETRIS);
      }
      return false;
    case TET_LFT:
      tetris_input(TETRIS_LEFT, record->event.pressed);
      return false;
    case TET_RGT:
      tetris_input(TETRIS_RIGHT, record->event.pressed);
      return false;
    case TET_CW:
      tetris_input(TETRIS_CW, record->event.pressed);
      return false;
    case TET_CCW:
      tetris_input(TETRIS_CCW, record->event.pressed);
      return false;
  }
#ifdef OLED_ENABLE
  process_record_user_oled(keycode, record);
#endif
  return true;
}

void keyboard_post_init_user(void) {
#ifdef RGB_MATRIX_ENABLE
  rgb_matrix_disable_noeeprom();
#endif
}

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (!is_keyboard_left()) {
    return OLED_ROTATION_180;
  }
  return rotation;
}

void oled_render_layer_state(void) {
  oled_write_P(PSTR("Layer: "), false);
  switch (get_highest_layer(layer_state)) {
    case _COLEMAK:
    case _QWERTY:
    case _DVORAK:
      oled_write_ln_P(PSTR("Default"), false);
      break;
    case _LOWER:
      oled_write_ln_P(PSTR("Lower"), false);
      break;
    case _RAISE:
      oled_write_ln_P(PSTR("Raise"), false);
      break;
    case _CONFIG:
      oled_write_ln_P(PSTR("Config"), false);
      break;
    default:
      oled_write_ln_P(PSTR("Unknown"), false);
      break;
  }
}

char keylog_str[24] = {};

const char code_to_name[60] = {
  ' ', ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f',
  'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
  'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
  'R', 'E', 'B', 'T', '_', '-', '=', '[', ']', '\\',
  '#', ';', '\'', '`', ',', '.', '/', ' ', ' ', ' '
};

void set_keylog(uint16_t keycode, keyrecord_t *record) {
  char name = ' ';
  if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) || (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX)) {
    keycode = keycode & 0xFF;
  }
  if (keycode < 60) {
    name = code_to_name[keycode];
  }

  snprintf(keylog_str, sizeof(keylog_str), "%dx%d, k%2d : %c", record->event.key.row, record->event.key.col, keycode, name);
}

void oled_render_keylog(void) {
  oled_write_ln(keylog_str, false);
}

void oled_render_wpm(void) {
  char wpm_str[8] = {0};
#ifdef WPM_ENABLE
  snprintf(wpm_str, sizeof(wpm_str), "WPM: %d", get_current_wpm());
#endif
  oled_write_ln(wpm_str, false);
}

void oled_render_logo(void) {
  static const char PROGMEM crkbd_logo[] = {
      0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
      0x90, 0x91, 0x92, 0x93, 0x94, 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa,
      0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5,
      0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0
  };
  oled_write_P(crkbd_logo, false);
}

bool oled_task_user(void) {
  if (is_keyboard_master()) {
    if (IS_LAYER_ON(_TETRIS)) {
      tetris_render();
    } else {
      oled_write_raw_byte(0xF0, 33);
    }
  } else {
      oled_render_layer_state();
      oled_render_keylog();
      oled_render_wpm();
    oled_render_logo();
  }
  return false;
}

void process_record_user_oled(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
    set_keylog(keycode, record);
  }
}
#endif // OLED_ENABLE
