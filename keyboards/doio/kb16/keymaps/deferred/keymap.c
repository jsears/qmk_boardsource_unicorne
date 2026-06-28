/* Copyright 2022 DOIO
 * Copyright 2022 HorrorTroll <https://github.com/HorrorTroll>
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

// OLED animation
#include "lib/layer_status/layer_status.h"

void keyboard_post_init_user(void) {
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_sethsv_noeeprom(191, 255, 255);
    // rgb_matrix_sethsv_noeeprom(HSV_OFF);
}

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.

enum layer_names {
    _BASE,
    _FN,
    _FN1,
    _FN2
};

enum layer_keycodes { KC_JIGG = SAFE_RANGE};

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
                        layer_on(_FN1);
                    } else {
                        layer_off(_FN1);
                    }
            }

            break;
    }
    // return true;
    return process_record_keymap(keycode, record) && process_record_secrets(keycode, record);
}
            

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/*
       ┌────┬────┬────┬────┐   ┌────┐ ┌────┐
       │Ctrl│CST │    │    │   │Play│ │TO1 │
       ├────┼────┼────┼────┤   └────┘ └────┘
       │ShGS│CtC │CtV │    │
       ├────┼────┼────┼────┤
       │    │CtSP│CtSV│    │      ┌────┐
       ├────┼────┼────┼────┤      │Mute│
       │    │    │    │CAD │      └────┘
       └────┴────┴────┴────┘
*/
    /*  Row:    0         1        2        3         4      */
    [_BASE] = LAYOUT(
                KC_LCTL,       LCS(KC_T),  _______,   _______, KC_MPLY,
                LSG(KC_S),     C(KC_C),    C(KC_V),   _______, TO(_FN),
                _______,       LCS(KC_P),  LCS(KC_V), _______, KC_MUTE,
                KC_JIGG,       _______,    _______,   C(A(KC_DEL))
            ),

/*
       ┌───┬───┬───┬───┐   ┌───┐ ┌───┐
       │ 7 │ 8 │ 9 │ / │   │CAD│ │TO2│
       ├───┼───┼───┼───┤   └───┘ └───┘
       │ 4 │ 5 │ 6 │ * │
       ├───┼───┼───┼───┤
       │ 1 │ 2 │ 3 │ - │      ┌───┐
       ├───┼───┼───┼───┤      │   │
       │ + │ 0 │Del│Ent│      └───┘
       └───┴───┴───┴───┘
*/
    /*  Row:    0        1        2        3        4       */
    [_FN] = LAYOUT(
                KC_7,     KC_8,    KC_9,    KC_PSLS,     KC_CALC,
                KC_4,     KC_5,    KC_6,    KC_PAST,     TO(_FN1),
                KC_1,     KC_2,    KC_3,    KC_PMNS,     KC_CALC,
                KC_PPLS,  KC_0,    KC_DEL,  KC_PENT
            ),

/*
       ┌───┬───┬───┬───┐   ┌───┐ ┌───┐
       │   │   │   │   │   │   │ │   │
       ├───┼───┼───┼───┤   └───┘ └───┘
       │   │   │   │   │
       ├───┼───┼───┼───┤
       │   │   │   │   │      ┌───┐
       ├───┼───┼───┼───┤      │   │
       │   │   │   │   │      └───┘
       └───┴───┴───┴───┘
*/
    /*  Row:    0        1        2        3        4       */
    [_FN1] = LAYOUT(
                KC_NO,    KC_NO,   KC_NO,   DB_TOGG,     _______,
                KC_NO,    KC_NO,   KC_NO,   KC_NO,       TO(_FN2),
                KC_NO,    KC_NO,   KC_NO,   KC_NO,       _______,
                KC_JIGG,  KC_NO,   KC_NO,   KC_NO
            ),

/*
       ┌───┬───┬───┬───┐   ┌───┐ ┌───┐
       │Spi│Spd│   │   │   │   │ │TO0│
       ├───┼───┼───┼───┤   └───┘ └───┘
       │Sai│Sad│   │   │
       ├───┼───┼───┼───┤
       │Tog│Mod│Hui│   │      ┌───┐
       ├───┼───┼───┼───┤      │   │
       │   │Vai│Hud│Vad│      └───┘
       └───┴───┴───┴───┘
*/
    /*  Row:    0        1        2        3        4        */
    [_FN2] = LAYOUT(
                RM_SPDU, RM_SPDD, _______, QK_BOOT, _______,
                RM_SATU, RM_SATD, _______, _______, TO(_BASE),
                RM_TOGG, RM_NEXT, RM_HUEU, _______, _______,
                _______, RM_VALU, RM_HUED, RM_VALD
            ),
};

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
    case _BASE:
        if(is_jiggling) {
            rgb_matrix_sethsv_noeeprom(43, 255, 255);
        } else {
            // Set Backlight LEDS to PURPLE
            rgb_matrix_sethsv_noeeprom(191, 255, 255);
        }
        break;
    case _FN:
        if(is_jiggling) {
            rgb_matrix_sethsv_noeeprom(43, 255, 255);
        } else {
            // Set Backlight LEDS to PURPLE
            rgb_matrix_sethsv_noeeprom(170, 255, 255);
        }
        break;
    case _FN1:
        if(is_jiggling) {
            rgb_matrix_sethsv_noeeprom(43, 255, 255);
        } else {
            // Set Backlight LEDS to PURPLE
            rgb_matrix_sethsv_noeeprom(85, 255, 255);
        }
        break;
    case _FN2:
        if(is_jiggling) {
            rgb_matrix_sethsv_noeeprom(43, 255, 255);
        } else {
            // Set Backlight LEDS to PURPLE
            rgb_matrix_sethsv_noeeprom(0, 255, 255);
        }
        break;
    }
  return state;
}

#ifdef OLED_ENABLE
    bool oled_task_user(void) {
        render_layer_status();

        return true;
    }
#endif

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_BASE] = { ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(KC_PGDN, KC_PGUP), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_FN]   = { ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [_FN1]  = { ENCODER_CCW_CW(MS_WHLD, MS_WHLU), ENCODER_CCW_CW(MS_WHLL, MS_WHLR), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [_FN2]  = { ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
};
#endif
