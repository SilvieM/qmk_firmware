typedef struct {
  bool is_press_action;
  int state;
} tap;

typedef enum {
  TD_SINGLE_TAP = 1,
  TD_SINGLE_HOLD = 2,
  TD_DOUBLE_TAP = 3,
  TD_DOUBLE_HOLD = 4,
  TD_DOUBLE_SINGLE_TAP = 5, //send two single taps
  TD_TRIPLE_TAP = 6,
  TD_TRIPLE_HOLD = 7
} td_state_t;

//Tap dance enums
enum {
    O_Umlaut = 0,
  A_Umlaut = 1
};

int cur_dance (tap_dance_state_t *state);

//for the x tap dance. Put it here so it can be used in any keymap
void x_finished (tap_dance_state_t *state, void *user_data);
void x_reset (tap_dance_state_t *state, void *user_data);


/// here keymap

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
    if (state->interrupted || !state->pressed)  return TD_SINGLE_TAP;
    //key has not been interrupted, but they key is still held. Means you want to send a 'HOLD'.
    else return TD_SINGLE_HOLD;
  }
  else if (state->count == 2) {
    /*
     * DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
     * action when hitting 'pp'. Suggested use case for this return value is when you want to send two
     * keystrokes of the key, and not the 'double tap' action/macro.
    */
    if (state->interrupted) return TD_DOUBLE_SINGLE_TAP;
    else if (state->pressed) return TD_DOUBLE_HOLD;
    else return TD_DOUBLE_TAP;
  }
  //Assumes no one is trying to type the same letter three times (at least not quickly).
  //If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
  //an exception here to return a 'TRIPLE_SINGLE_TAP', and define that enum just like 'DOUBLE_SINGLE_TAP'
  if (state->count == 3) {
    if (state->interrupted || !state->pressed)  return TD_TRIPLE_TAP;
    else return TD_TRIPLE_HOLD;
  }
  else return 8; //magic number. At some point this method will expand to work for more presses
}


typedef struct {
    uint16_t on_tap;
    uint16_t on_hold;
    td_state_t state;
} tap_dance_config_t;

// since the functions already get a `user_data` we may be able to get rid of these "wrappers"
// and manually set things up on the array of tap dance definitions (by creating the struct
// ourselves instead of using the ADVANCED macro), but im lazy to search the codebase and figure out
static tap_dance_config_t a_config = {
    .on_tap  = KC_A,
    .on_hold = RALT(KC_Q), // see how we store the modifier+key combination :D
};
static tap_dance_config_t o_config = {
    .on_tap  = KC_O,
    .on_hold = RALT(KC_P), // see how we store the modifier+key combination :D
};


void common_finished(tap_dance_state_t *state, void *user_data) {
    tap_dance_config_t *config = (tap_dance_config_t *)user_data;
    config->state = cur_dance(state);
  switch (config->state) {
    case TD_SINGLE_TAP: register_code16(config->on_tap); break;
    case TD_DOUBLE_TAP: tap_code16(config->on_tap); register_code16(config->on_tap); break;
    case TD_TRIPLE_TAP: tap_code16(config->on_tap); tap_code16(config->on_tap); register_code16(config->on_tap); break;
    case TD_SINGLE_HOLD: register_code16(config->on_hold); break;
    case TD_DOUBLE_HOLD: tap_code16(config->on_hold); register_code16(config->on_hold); break;
    case TD_TRIPLE_HOLD: tap_code16(config->on_hold); tap_code16(config->on_hold); register_code16(config->on_hold); break;
    case TD_DOUBLE_SINGLE_TAP: tap_code16(config->on_tap); register_code16(config->on_tap); break;
  }
}
void common_reset(tap_dance_state_t *state, void *user_data) {
    tap_dance_config_t *config = (tap_dance_config_t *)user_data;
  switch (config->state) {
    case TD_SINGLE_TAP: unregister_code16(config->on_tap); break;
    case TD_DOUBLE_TAP: unregister_code16(config->on_tap); break;
    case TD_TRIPLE_TAP: unregister_code16(config->on_tap); break;
    case TD_SINGLE_HOLD: unregister_code16(config->on_hold); break;
    case TD_DOUBLE_HOLD: unregister_code16(config->on_hold); break;
    case TD_TRIPLE_HOLD: unregister_code16(config->on_hold); break;
    case TD_DOUBLE_SINGLE_TAP: unregister_code16(config->on_tap); break;
  }
  config->state = 0;
}
void a_finished(tap_dance_state_t *state, void *user_data) {
    common_finished(state, &a_config);
}
void a_reset(tap_dance_state_t *state, void *user_data) {
    common_reset(state, &a_config);
}
void o_finished(tap_dance_state_t *state, void *user_data) {
    common_finished(state, &o_config);
}
void o_reset(tap_dance_state_t *state, void *user_data) {
    common_reset(state, &o_config);
}

tap_dance_action_t tap_dance_actions[] = {
  [A_Umlaut]     = ACTION_TAP_DANCE_FN_ADVANCED(NULL,a_finished, a_reset),
  [O_Umlaut]     = ACTION_TAP_DANCE_FN_ADVANCED(NULL,o_finished, o_reset)
};