// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.KC_0-or-later

#include QMK_KEYBOARD_H

enum custom_keycodes {
  QUOTE = SAFE_RANGE,
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        QK_GESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                                                     KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
        KC_TAB,  KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,                                                      KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN,    RALT(KC_Y), //ü
        KC_MINS,  KC_A,    KC_R,    KC_S,    KC_T,    KC_G,                                                     KC_M,    KC_N,    KC_E,    KC_I,    KC_O, RALT(KC_Q),//ä
        OSM(MOD_LSFT), KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,                                                KC_K,    KC_H,    KC_COMM, KC_DOT,  KC_SLSH, RALT(KC_P),//ö
        KC_LCTL, KC_EQL,   KC_LGUI,   KC_BSLS,       OSL(1), OSM(MOD_LSFT), MT(MOD_LALT, KC_DEL),       MT(MOD_RALT, KC_BSPC), OSM(MOD_LSFT), OSL(1),          KC_LBRC, KC_RBRC, QUOTE, RALT(KC_S),
                                                        KC_SPC, KC_LEFT, KC_RGHT,                          KC_DOWN, KC_UP, KC_ENT                     
                                                          
    ),
    [1] = LAYOUT(
        QK_BOOT,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,                                        KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,    KC_F11,
        KC_TRNS,    KC_EXLM,  KC_AT,    KC_HASH,    KC_DLR,        KC_PERC,                             KC_CIRC,    KC_AMPR,    KC_ASTR,    KC_LPRN,    KC_RPRN,    KC_F12,
        KC_0,       KC_1,    KC_2,    KC_3,         KC_4,            KC_5,                              KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_TRNS,
        KC_TRNS, KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                               KC_TRNS,    KC_TRNS,    KC_TRNS, KC_TRNS,  KC_BSLS, KC_TRNS,
        KC_TRNS, KC_TRNS,   KC_TRNS,   KC_TRNS,          OSL(2), KC_TRNS, KC_TRNS,                OSL(2), KC_TRNS, KC_TRNS,           KC_TRNS, KC_PIPE,   KC_TRNS, KC_TRNS,
                                                        KC_TRNS, KC_TRNS, KC_TRNS,                KC_TRNS, KC_TRNS, KC_TRNS                     
                                                          
    ),
    [2] = LAYOUT(
        QK_GESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                                          KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    RALT(KC_Y),
        KC_MINS,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                                         KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, RALT(KC_Q),
        OSM(MOD_LSFT), KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                                    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, RALT(KC_P),
        KC_LCTL, KC_EQL,   KC_LALT,   KC_LGUI,          OSL(1), OSM(MOD_LSFT), KC_DEL,       KC_BSPC, OSM(MOD_LSFT), OSL(1),          KC_LBRC, KC_RBRC, QUOTE ,RALT(KC_S)
                                                        KC_SPC, KC_LEFT, KC_RGHT,          KC_DOWN, KC_UP, KC_ENT                     
                                                          
    ),
};

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
  switch (keycode) {
    case QUOTE:
      if (record->event.pressed) {
        tap_code(KC_QUOT);
        tap_code(KC_SPC);
      }
      return false;
  }
  return true;
}
