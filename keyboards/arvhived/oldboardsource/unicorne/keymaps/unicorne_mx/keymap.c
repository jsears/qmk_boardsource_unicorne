#include QMK_KEYBOARD_H
#include "print.h"

extern uint8_t is_master;

void keyboard_post_init_user(void) {
#ifdef RGB_MATRIX_ENABLE
    rgb_matrix_enable();
    rgb_matrix_set_speed(RGB_MATRIX_DEFAULT_SPD);
    //rgb_matrix_mode(RGB_MATRIX_SOLID_REACTIVE);
    rgb_matrix_sethsv(HSV_PURPLE);
#endif
    debug_enable=true;
    debug_matrix=true;
    debug_keyboard=true;
}

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _QWERTY 0
#define _LOWER  1
#define _RAISE  2
#define _FUNC   3
#define _SPCHRS 4

enum custom_keycodes {
    DEFAULT = SAFE_RANGE,
    LOWER,
    RAISE,
    FUNC,
    LOCKWIN,
    KC_JIGG,
    C_ALT_D,
    SNAP_LFT,
    SNAP_RT,
    SNAP_TOP,
    SNAP_BTM,
    KVM_SW
};

enum combos {
  JK_ESC
};

const uint16_t PROGMEM jk_combo[] = {KC_J, KC_K, COMBO_END};


combo_t key_combos[COMBO_COUNT] = {
  // For Vim, put Escape on the home row
  [JK_ESC]    = COMBO(jk_combo, KC_ESC),

};

__attribute__((weak))
bool process_record_keymap(uint16_t keycode, keyrecord_t *record) { return true; }

__attribute__((weak))
bool process_record_secrets(uint16_t keycode, keyrecord_t *record) { return true; }

/*declare boolean for jiggler*/
bool is_jiggling = false;

/*timers*/
uint32_t idle_timeout = 30000; // (after 30s)
uint32_t mouse_interval = 10000; // (every 10s)

static uint32_t idle_callback(uint32_t trigger_time, void* cb_arg) {
    // now idle
    if(is_jiggling) {
        #ifdef CONSOLE_ENABLE
        dprintf("sending: %s\n", "X_F15");
        #endif
        SEND_STRING(SS_TAP(X_F15));
    }
    return mouse_interval;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

     // on every key event start or extend `idle_callback()` deferred execution after IDLE_TIMEOUT_MS
    static deferred_token idle_token = INVALID_DEFERRED_TOKEN;

    #ifdef CONSOLE_ENABLE
        dprintf("deferred_exec: %s\n", !extend_deferred_exec(idle_token, idle_timeout) ? "true" : "false");
        #endif

    if (!extend_deferred_exec(idle_token, idle_timeout)) {
        idle_token = defer_exec(idle_timeout, idle_callback, NULL);
    }

#ifdef CONSOLE_ENABLE
    // uprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
#endif

    switch (keycode) {
        case KC_JIGG:
            if (record->event.pressed) {
                    is_jiggling = !is_jiggling; /*flip boolean to true*/
                    #ifdef CONSOLE_ENABLE
                    dprintf("is_jiggling: %s\n", is_jiggling ? "true" : "false");
                    #endif
                    if(is_jiggling) {
                        layer_on(_SPCHRS);
                    } else {
                        layer_off(_SPCHRS);
                    }
            }

            break;
        case LOCKWIN:
            if (record->event.pressed) {
                // when keycode LOCKWIN is pressed
                register_code(KC_LGUI);  // press the Windows key
                register_code(KC_L); // press the L key
            } else {
                // when keycode LOCKWIN is released
                unregister_code(KC_LGUI); // release the Windows key
                unregister_code(KC_L);  // release the L key
            }
            break;
        case C_ALT_D:
            if(record->event.pressed) {
                register_code(KC_LCTL);
                register_code(KC_LALT);
                register_code(KC_DEL);
            } else {
                unregister_code(KC_LCTL);
                unregister_code(KC_LALT);
                unregister_code(KC_DEL);
            }
            break;
        case KVM_SW:
            if(record->event.pressed) {
                register_code(KC_NUM);
                register_code(KC_NUM);
            } else {
                unregister_code(KC_NUM);
                unregister_code(KC_NUM);
            }
        break;
        case SNAP_LFT:
            if(record->event.pressed) {
                register_code(KC_LGUI);
                register_code(KC_LEFT);
            } else {
                unregister_code(KC_LGUI);
                unregister_code(KC_LEFT);
            }
        break;
        case SNAP_RT:
            if(record->event.pressed) {
                register_code(KC_LGUI);
                register_code(KC_RIGHT);
            } else {
                unregister_code(KC_LGUI);
                unregister_code(KC_RIGHT);
            }
        break;
        case SNAP_TOP:
            if(record->event.pressed) {
                register_code(KC_LGUI);
                register_code(KC_UP);
            } else {
                unregister_code(KC_LGUI);
                unregister_code(KC_UP);
            }
        break;
        case SNAP_BTM:
            if(record->event.pressed) {
                register_code(KC_LGUI);
                register_code(KC_DOWN);
            } else {
                unregister_code(KC_LGUI);
                unregister_code(KC_DOWN);
            }
        break;
    }
    // return true;
    return process_record_keymap(keycode, record) && process_record_secrets(keycode, record);
}

// For _QWERTY layer
#define OSL_FUN  OSL(_FUNC)
#define LOW_TAB  LT(_LOWER, KC_TAB)
#define RSE_BSP  LT(_RAISE, KC_BSPC)
#define OSM_SFT  OSM(MOD_LSFT)
#define OSM_ALT  OSM(MOD_LALT)
#define OSM_CTL  OSM(MOD_LCTL)
#define OSM_GUI  OSM(MOD_LGUI)

// For _LOWER layer
#define ALT_TAB LALT(KC_TAB)


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[_QWERTY] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
     OSM_ALT,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                       KC_Y    ,KC_U    ,KC_I    ,KC_O    ,KC_P  ,  KC_BSLS,\
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
     OSM_SFT,   KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                       KC_H    ,KC_J    ,KC_K    ,KC_L   , KC_SCLN, KC_QUOT,\
  //|---- ----+--------+--------+--------+--------+-------|                    |--------+--------+--------+--------+--------+--------|
     OSM_CTL,   KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                       KC_N    ,KC_M    ,KC_COMM ,KC_DOT  ,KC_SLSH ,OSL_FUN,\
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                         OSM_GUI, LOW_TAB, KC_SPC,     KC_ENT, RSE_BSP, OSM_SFT \
                                      //`--------------------------'  `--------------------------'
  ),

[_LOWER] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      KC_GRV,  KC_F1,   KC_F2,  KC_F3,   KC_F4,   KC_F5,                         KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F12,\
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, KC_1,    KC_2,   KC_3,    KC_4,    KC_5,                          KC_6,    KC_7,    KC_8,    KC_9,    KC_0,   _______,\
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, XXXXXXX, XXXXXXX, KC_EQL, KC_LBRC, KC_LCBR,                       KC_RCBR, KC_RBRC, KC_MINS, XXXXXXX, XXXXXXX,KC_TRNS,\
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_TRNS, KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS \
                                      //`--------------------------'  `--------------------------'
  ),
  
[_RAISE] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
     _______, KC_DEL , KC_HOME, KC_PGUP, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, KC_UP,   XXXXXXX, XXXXXXX, _______,\
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
     _______, KC_INS,  KC_END,  KC_PGDN, XXXXXXX, XXXXXXX,                      _______, KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX, _______,\
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_MPLY, KC_MPRV, KC_MNXT, KC_VOLD, KC_VOLU, KC_TRNS,\
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS\
                                      //`--------------------------'  `--------------------------'
  ),

[_FUNC] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                     XXXXXXX, XXXXXXX, SNAP_TOP, XXXXXXX, RM_TOGG, _______,\
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      C_ALT_D, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                     XXXXXXX,SNAP_LFT, SNAP_BTM, SNAP_RT, KC_CALC, _______,\
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                     LOCKWIN, QK_RBT,  QK_BOOT, EE_CLR,  KC_SLEP, _______,\
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_TRNS, KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS\
                                      //`--------------------------'  `--------------------------'
  )
};

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
      case LT(_RAISE, KC_BSPC):
            return TAPPING_TERM_THUMB;
      case LT(_LOWER, KC_TAB):
            return TAPPING_TERM_THUMB;
      default:
            return TAPPING_TERM;
    }
}

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
    case _QWERTY:
        rgb_matrix_sethsv(HSV_PURPLE);
        break;
    case _LOWER:
        rgb_matrix_sethsv(HSV_BLUE);
        break;
    case _RAISE:
        rgb_matrix_sethsv(HSV_GREEN);
        break;
    case _FUNC:
        rgb_matrix_sethsv(HSV_RED);
        break;
    case _SPCHRS:
        // rgb_matrix_set_color(i, HSV_YELLOW);
        rgb_matrix_sethsv(HSV_YELLOW);
        break;
    }
  return state;
}


#if defined(ENCODER_ENABLE) && defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_QWERTY] = { ENCODER_CCW_CW(KC_WH_U, KC_WH_D),              ENCODER_CCW_CW(KC_WH_R, KC_WH_L)},
    [_FUNC] =   { ENCODER_CCW_CW(RGB_HUD, RGB_HUI),              ENCODER_CCW_CW(RGB_RMOD, RGB_MOD)  },
    [_RAISE] =  { ENCODER_CCW_CW(KC_VOLD, KC_VOLU),             ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
    [_LOWER] =  { ENCODER_CCW_CW(KC_VOLD, KC_VOLU),             ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
};
#endif // defined(ENCODER_ENABLE) && defined(ENCODER_MAP_ENABLE)

#ifdef OLED_ENABLE
bool oled_task_user(void) {
    // Host Keyboard Layer Status

    if (is_keyboard_master()) {
        switch (get_highest_layer(layer_state)) {
            case _QWERTY:
                oled_write_P(PSTR("Default\n"), false);
                break;
            case _LOWER:
                oled_write_P(PSTR("Lower\n"), false);
                break;
            case _RAISE:
                oled_write_P(PSTR("Raise\n"), false);
                break;
            case _FUNC:
                oled_write_P(PSTR("FN\n"), false);
                break;
            default:
                // Or use the write_ln shortcut over adding '\n' to the end of your string
                oled_write_ln_P(PSTR("Undefined"), false);
        }
    } else {
        // oled_write_raw(logo, sizeof(logo));
        oled_write_raw_P(bs_logo_img, sizeof(bs_logo_img));
    }
    return false;
}
#endif