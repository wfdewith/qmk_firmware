/* Copyright 2020 Wim de With
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H

enum planck_layers {
  _COLEMAK = 0,
  _QWERTY,
  _DVORAK,
  _LOWER,
  _RAISE,
  _CONFIG,
  _FN,
};

enum planck_keycodes {
  QWERTY = SAFE_RANGE,
  COLEMAK,
  DVORAK,
  M_JIGL,
};

#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)
#define FN TT(_FN)
#define CTL_ESC LCTL_T(KC_ESC)
#define QWE_GUI LM(_QWERTY, MOD_LGUI)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[_COLEMAK] = LAYOUT_planck_grid(
  KC_TAB,  KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,    KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN, KC_DEL,
  CTL_ESC, KC_A,    KC_R,    KC_S,    KC_T,    KC_D,    KC_H,    KC_N,    KC_E,    KC_I,    KC_O,    KC_QUOT,
  KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_ENT,
  KC_LCTL, FN,      QWE_GUI, KC_LALT, LOWER,   KC_BSPC, KC_SPC,  RAISE,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT
),

[_QWERTY] = LAYOUT_planck_grid(
  _______, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_PSCR,
  _______, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, _______,
  _______, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, _______,
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
),

[_DVORAK] = LAYOUT_planck_grid(
  _______, KC_QUOT, KC_COMM, KC_DOT,  KC_P,    KC_Y,    KC_F,    KC_G,    KC_C,    KC_R,    KC_L,    _______,
  _______, KC_A,    KC_O,    KC_E,    KC_U,    KC_I,    KC_D,    KC_H,    KC_T,    KC_N,    KC_S,    KC_SLSH,
  _______, KC_SCLN, KC_Q,    KC_J,    KC_K,    KC_X,    KC_B,    KC_M,    KC_W,    KC_V,    KC_Z,    _______,
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
),

[_LOWER] = LAYOUT_planck_grid(
  XXXXXXX, XXXXXXX, XXXXXXX, KC_LBRC, KC_RBRC, KC_BSLS, KC_PIPE, KC_LCBR, KC_RCBR, XXXXXXX, XXXXXXX, XXXXXXX,
  _______, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_RALT,
  _______, XXXXXXX, XXXXXXX, KC_TILD, KC_MINS, KC_UNDS, KC_PLUS, KC_EQL,  KC_GRV,  XXXXXXX, XXXXXXX, XXXXXXX,
  _______, _______, _______, _______, _______, XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
),

[_RAISE] = LAYOUT_planck_grid(
  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
  _______, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    XXXXXXX,
  _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, XXXXXXX,
  _______, _______, _______, _______, _______, XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
),

[_CONFIG] = LAYOUT_planck_grid(
  QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QWERTY,  COLEMAK, DVORAK,
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  XXXXXXX, _______, XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
),

[_FN] = LAYOUT_planck_grid(
  KC_NUM,  KC_MPLY, KC_MSTP, KC_MPRV, KC_MNXT, KC_INS,  KC_HOME, KC_PGUP, KC_P7,   KC_P8,   KC_P9,   KC_PSLS,
  M_JIGL,  XXXXXXX, KC_MUTE, KC_VOLD, KC_VOLU, KC_DEL,  KC_END,  KC_PGDN, KC_P4,   KC_P5,   KC_P6,   KC_PAST,
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_P1,   KC_P2,   KC_P3,   KC_PMNS,
  XXXXXXX, _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_P0,   KC_PDOT, KC_PENT, KC_PPLS
)
};

static bool is_mouse_jiggle_active = false;
static bool mouse_jiggle_direction = false;
static const uint16_t mouse_jiggle_frequency = 5000;
static uint16_t mouse_jiggle_timer = 0;

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
    case M_JIGL:
      if (record->event.pressed) {
        is_mouse_jiggle_active = !is_mouse_jiggle_active;
      }
      return false;
  }
  return true;
}

void keyboard_post_init_user(void) {
  mouse_jiggle_timer = timer_read();
}

void matrix_scan_user(void) {
  if (is_mouse_jiggle_active) {
    if (timer_elapsed(mouse_jiggle_timer) > mouse_jiggle_frequency) {
      mouse_jiggle_timer = timer_read();
      if (mouse_jiggle_direction) {
        tap_code(KC_MS_LEFT);
      } else {
        tap_code(KC_MS_RIGHT);
      }
      mouse_jiggle_direction = !mouse_jiggle_direction;
    }
  }
}
