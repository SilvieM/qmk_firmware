// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.KC_0-or-later

#include QMK_KEYBOARD_H

typedef struct {
  bool is_press_action;
  int state;
} tap;

enum {
  SINGLE_TAP = 1,
  SINGLE_HOLD = 2,
  DOUBLE_TAP = 3,
  DOUBLE_HOLD = 4,
  DOUBLE_SINGLE_TAP = 5, //send two single taps
  TRIPLE_TAP = 6,
  TRIPLE_HOLD = 7
};

//Tap dance enums
enum {
  A_Umlaut = 1
};

int cur_dance (tap_dance_state_t *state);

//for the x tap dance. Put it here so it can be used in any keymap
void x_finished (tap_dance_state_t *state, void *user_data);
void x_reset (tap_dance_state_t *state, void *user_data);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*130add1a29l1a29l1a29l3d4-018k413
     */
    [0] = LAYOUT(
        QK_GESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                                          KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS,
        KC_MINS,  TD(A_Umlaut),    KC_S,    KC_D,    KC_F,    KC_G,                                         KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
        OSM(MOD_LSFT), KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                                    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
        KC_LCTL, KC_EQL,   KC_LALT,   KC_LGUI,          OSL(1), OSM(MOD_LSFT), KC_DEL,       KC_BSPC, OSM(MOD_LSFT), OSL(1),          KC_LBRC, KC_RBRC,   KC_SCLN, KC_QUOT,
                                                        KC_SPC, KC_LEFT, KC_RGHT,          KC_DOWN, KC_UP, KC_ENT                     
                                                          
    ),
    [1] = LAYOUT(
        QK_BOOT,  KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                              KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    QK_BOOT,
        KC_TRNS,    KC_EXLM,  KC_AT,    KC_HASH,    KC_DLR,        KC_PERC,                              KC_CIRC,    KC_AMPR,    KC_ASTR,    KC_LPRN,    KC_RPRN,    KC_TRNS,
        KC_0,       KC_1,    KC_2,    KC_3,         KC_4,            KC_5,                              KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_TRNS,
        KC_TRNS, KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                               KC_TRNS,    KC_TRNS,    KC_TRNS, KC_TRNS,  KC_BSLS, KC_TRNS,
        KC_TRNS, KC_TRNS,   KC_TRNS,   KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS,                KC_TRNS, KC_TRNS, KC_TRNS,           KC_TRNS, KC_PIPE,   KC_TRNS, KC_TRNS,
                                                        KC_TRNS, KC_TRNS, KC_TRNS,                KC_TRNS, KC_TRNS, KC_TRNS                     
                                                          
    ),
    [2] = LAYOUT(
        QK_GESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                                          KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS,
        KC_MINS,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                                         KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
        OSM(MOD_LSFT), KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                                    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
        KC_LCTL, KC_EQL,   KC_LALT,   KC_LGUI,          OSL(1), OSM(MOD_LSFT), KC_DEL,       KC_BSPC, OSM(MOD_LSFT), OSL(1),          KC_LBRC, KC_RBRC,   KC_SCLN, KC_QUOT,
                                                        KC_SPC, KC_LEFT, KC_RGHT,          KC_DOWN, KC_UP, KC_ENT                     
                                                          
    ),
};
/* Return an integer that corresponds to what kind of tap dance should be executed.
 *
 * How to figure out tap dance state: interrupted and pressed.
 *
 * Interrupted: If the state of a dance is "interrupted", that means that another key has been hit
 *  under the tapping term. This is typically indicitive that you are trying to "tap" the key.
 *
 * Pressed: Whether or not the key is still being pressed. If this value is true, that means the tapping term
 *  has ended, but the key is still being pressed down. This generally means the key is being "held".
 *
 * One thing that is currenlty not possible with qmk software in regards to tap dance is to mimic the "permissive hold"
 *  feature. In general, advanced tap dances do not work well if they are used with commonly typed letters.
 *  For example "A". Tap dances are best used on non-letter keys that are not hit while typing letters.
 *
 * Good places to put an advanced tap dance:
 *  z,q,x,j,k,v,b, any function key, home/end, comma, semi-colon
 *
 * Criteria for "good placement" of a tap dance key:
 *  Not a key that is hit frequently in a sentence
 *  Not a key that is used frequently to double tap, for example 'tab' is often double tapped in a terminal, or
 *    in a web form. So 'tab' would be a poor choice for a tap dance.
 *  Letters used in common words as a double. For example 'p' in 'pepper'. If a tap dance function existed on the
 *    letter 'p', the word 'pepper' would be quite frustating to type.
 *
 * For the third point, there does exist the 'TD_DOUBLE_SINGLE_TAP', however this is not fully tested
 *
 */
int cur_dance (tap_dance_state_t *state) {
  if (state->count == 1) {
    if (state->interrupted || !state->pressed)  return SINGLE_TAP;
    //key has not been interrupted, but they key is still held. Means you want to send a 'HOLD'.
    else return SINGLE_HOLD;
  }
  else if (state->count == 2) {
    /*
     * DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
     * action when hitting 'pp'. Suggested use case for this return value is when you want to send two
     * keystrokes of the key, and not the 'double tap' action/macro.
    */
    if (state->interrupted) return DOUBLE_SINGLE_TAP;
    else if (state->pressed) return DOUBLE_HOLD;
    else return DOUBLE_TAP;
  }
  //Assumes no one is trying to type the same letter three times (at least not quickly).
  //If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
  //an exception here to return a 'TRIPLE_SINGLE_TAP', and define that enum just like 'DOUBLE_SINGLE_TAP'
  if (state->count == 3) {
    if (state->interrupted || !state->pressed)  return TRIPLE_TAP;
    else return TRIPLE_HOLD;
  }
  else return 8; //magic number. At some point this method will expand to work for more presses
}

//instanalize an instance of 'tap' for the 'x' tap dance.
static tap xtap_state = {
  .is_press_action = true,
  .state = 0
};

void a_finished (tap_dance_state_t *state, void *user_data) {
  xtap_state.state = cur_dance(state);
  switch (xtap_state.state) {
    case SINGLE_TAP: register_code(KC_A); break;
    case SINGLE_HOLD: register_code(KC_RALT); register_code(KC_Q); break;
  }
}

void a_reset (tap_dance_state_t *state, void *user_data) {
  switch (xtap_state.state) {
    case SINGLE_TAP: unregister_code(KC_A); break;
    case SINGLE_HOLD: unregister_code(KC_RALT); unregister_code(KC_Q); break;
  }
  xtap_state.state = 0; 
}

tap_dance_action_t tap_dance_actions[] = {
  [A_Umlaut]     = ACTION_TAP_DANCE_FN_ADVANCED(NULL,a_finished, a_reset)
};