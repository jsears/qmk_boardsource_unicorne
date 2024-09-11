/* Copyright 2015-2023 Jack Humbert
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

enum planck_layers { _QWERTY, _COLEMAK, _COLEMAKDH, _DVORAK, _LOWER, _RAISE, _FUNC, _PLOVER, _ADJUST };

enum planck_keycodes { QWERTY = SAFE_RANGE, COLEMAK, COLEMAKDH, DVORAK, PLOVER, BACKLIT, EXT_PLV, LOCKWIN, C_ALT_D, SNAP_LFT, SNAP_RT, SNAP_TOP, SNAP_BTM};

//Combos
enum combos { JK_ESC };
const uint16_t PROGMEM jk_combo[] = {KC_J, KC_K, COMBO_END};
// For Vim, put Escape on the home row
combo_t key_combos[COMBO_COUNT] = { [JK_ESC] = COMBO(jk_combo, KC_ESC),};

// Tap Dance declarations
enum { TD_SPC_ENT, };

// Tap Dance definitions
// Tap once for Space, twice for Enter Lock
tap_dance_action_t tap_dance_actions[] = {
    [TD_SPC_ENT] = ACTION_TAP_DANCE_DOUBLE(KC_SPC, KC_ENT),
};


#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)

// For _QWERTY layer
#define OSL_FUN  OSL(_FUNC)
#define LOW_TAB  LT(_LOWER, KC_TAB)
#define RSE_BSP  LT(_RAISE, KC_BSPC)
#define OSM_SFT  OSM(MOD_LSFT)
#define OSM_RSFT  OSM(MOD_RSFT)
#define OSM_ALT  OSM(MOD_LALT)
#define OSM_CTL  OSM(MOD_LCTL)
#define OSM_GUI  OSM(MOD_LGUI)

// For _LOWER layer
#define ALT_TAB LALT(KC_TAB)

/* clang-format off */
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Qwerty
 * ,-----------------------------------------------------------------------------------.
 * | Alt  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | \ |  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift|   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Ctrl |   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  | Func |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      | Alt  | GUI  |Lower |Space |Enter |Raise |Shift |      |      |LCKWIN|
 * `-----------------------------------------------------------------------------------'
 */
[_QWERTY] = LAYOUT_planck_grid(
    OSM_ALT, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS,
    OSM_SFT, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
    OSM_CTL, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, OSL_FUN,
    XXXXXXX, XXXXXXX, OSM_ALT, OSM_GUI, LOW_TAB, KC_SPC,  KC_ENT,  RSE_BSP, OSM_RSFT,C_ALT_D, XXXXXXX, LOCKWIN
),

/* Colemak
 * ,-----------------------------------------------------------------------------------.
 * | Alt  |   Q  |   W  |   F  |   P  |   G  |   J  |   L  |   U  |   Y  |   ;  | \ |  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Esc  |   A  |   R  |   S  |   T  |   D  |   H  |   N  |   E  |   I  |   O  |  '   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   K  |   M  |   ,  |   .  |   /  | Func |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      | Alt  | GUI  |LowTab|Space |Enter |RaiBsp|Shift |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_COLEMAK] = LAYOUT_planck_grid(
    OSM_ALT,  KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,    KC_J,    KC_L,    KC_U,     KC_Y,    KC_SCLN, KC_BSLS,
    OSM_SFT,  KC_A,    KC_R,    KC_S,    KC_T,    KC_D,    KC_H,    KC_N,    KC_E,     KC_I,    KC_O,    KC_QUOT,
    OSM_CTL,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_K,    KC_M,    KC_COMM,  KC_DOT,  KC_SLSH, OSL_FUN,
    XXXXXXX,  XXXXXXX, OSM_ALT, OSM_GUI, LOW_TAB, KC_SPC,  KC_ENT,  RSE_BSP, OSM_RSFT, XXXXXXX, XXXXXXX, XXXXXXX
),

/* ColemakDH
 * ,-----------------------------------------------------------------------------------.
 * | Alt  |   Q  |   W  |   F  |   P  |   B  |   J  |   L  |   U  |   Y  |   ;  | Bksp |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Esc  |   A  |   R  |   S  |   T  |   G  |   M  |   N  |   E  |   I  |   O  |  '   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   D  |   V  |   K  |   H  |   ,  |   .  |   /  | Func |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      | Alt  | GUI  |LowTab|Space |Enter |RaiBsp|Shift |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_COLEMAKDH] = LAYOUT_planck_grid(
    OSM_ALT,  KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,    KC_J,    KC_L,    KC_U,     KC_Y,    KC_SCLN, KC_BSPC,
    OSM_SFT,  KC_A,    KC_R,    KC_S,    KC_T,    KC_D,    KC_H,    KC_N,    KC_E,     KC_I,    KC_O,    KC_QUOT,
    OSM_CTL,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_K,    KC_M,    KC_COMM,  KC_DOT,  KC_SLSH, OSL_FUN ,
    XXXXXXX,  XXXXXXX, OSM_ALT, OSM_GUI, LOW_TAB, KC_SPC,  KC_ENT,  RSE_BSP, OSM_RSFT, XXXXXXX, XXXXXXX, XXXXXXX
),

/* Dvorak
 * ,-----------------------------------------------------------------------------------.
 * | Alt  |   '  |   ,  |   .  |   P  |   Y  |   F  |   G  |   C  |   R  |   L  | Bksp |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Esc  |   A  |   O  |   E  |   U  |   I  |   D  |   H  |   T  |   N  |   S  |  /   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift|   ;  |   Q  |   J  |   K  |   X  |   B  |   M  |   W  |   V  |   Z  | Func |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      | Alt  | GUI  |LowTab|Space |Enter |RaiBsp|Shift |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_DVORAK] = LAYOUT_planck_grid(
    OSM_ALT, KC_QUOT, KC_COMM, KC_DOT,  KC_P,    KC_Y,    KC_F,    KC_G,    KC_C,     KC_R,    KC_L,    KC_BSPC,
    KC_ESC,  KC_A,    KC_O,    KC_E,    KC_U,    KC_I,    KC_D,    KC_H,    KC_T,     KC_N,    KC_S,    KC_SLSH,
    KC_LSFT, KC_SCLN, KC_Q,    KC_J,    KC_K,    KC_X,    KC_B,    KC_M,    KC_W,     KC_V,    KC_Z,    KC_ENT ,
    XXXXXXX, XXXXXXX, OSM_ALT, OSM_GUI, LOW_TAB, KC_SPC,  KC_ENT,  RSE_BSP, OSM_RSFT, XXXXXXX, XXXXXXX, XXXXXXX
),

/* Lower
 * ,-----------------------------------------------------------------------------------.
 * |  ~ ` |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |  F7  |  F8  |  F9  |  F10 |  F12 |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |  1 ! |  2 @ |  3 # |  4 $ |  5 % |  6 ^ |  7 & |  8 * |  9 ( |  0 ) |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      | = +  |   [  |   {  |   }  |   ]  |  - _ |      |      | Func |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      | Alt  | GUI  |LowTab|Space |Enter |RaiBsp|Shift |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_LOWER] = LAYOUT_planck_grid(
    KC_GRV,  KC_F1,   KC_F2,   KC_F3,  KC_F4,   KC_F5,   KC_F6,   KC_F7,    KC_F8,   KC_F9,   KC_F10,  KC_F12,
    _______, KC_1,    KC_2,    KC_3,   KC_4,    KC_5,    KC_6,    KC_7,     KC_8,    KC_9,    KC_0,    _______,
    _______, _______, _______, KC_EQL, KC_LBRC, KC_LCBR, KC_RCBR, KC_RBRC,  KC_MINS, _______, _______, OSL_FUN,
    _______, _______, OSM_ALT, OSM_GUI,LOW_TAB, KC_SPC,  KC_ENT,  RSE_BSP,  OSM_SFT, _______, _______, _______
),

/* Raise
 * ,-----------------------------------------------------------------------------------.
 * |      | Del  | Home | PGUP |      |      |      |      |  Up  |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      | Ins  | End  | PGDN |      |      |      | Left | Down |  Up  | Right|      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |      |      |      | Func |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      | Alt  | GUI  |LowTab|Space |Enter |RaiBsp|Shift |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_RAISE] = LAYOUT_planck_grid(
    _______,  KC_DEL,  KC_HOME, KC_PGUP, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_UP,   XXXXXXX, XXXXXXX, KC_BSPC,
    _______,  KC_INS,  KC_END,  KC_PGDN, XXXXXXX, XXXXXXX, XXXXXXX, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_BSLS,
    _______,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, OSL_FUN,
    _______,  _______, OSM_ALT, OSM_GUI,LOW_TAB, KC_SPC,  KC_ENT,  RSE_BSP,  OSM_SFT, _______, _______, _______
),

/* Func
 * ,-----------------------------------------------------------------------------------.
 * |      |      |      |      |      |      |      |      |      |      |      | MUTE |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |SNPLFT|SNPBTM|SNPTOP|SNP_RT| CALC |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |LCKWIN|QK_RBT|QK_BT |EE_CLR|      | Func |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      | Alt  | GUI  |LowTab|Space |Enter |RaiBsp|Shift |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_FUNC] = LAYOUT_planck_grid(
    _______,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  SNAP_TOP, XXXXXXX,  XXXXXXX, KC_MUTE,
    _______,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, SNAP_LFT, SNAP_BTM, SNAP_TOP, SNAP_RT, KC_CALC,
    _______,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, LOCKWIN, QK_RBT,   QK_BOOT,  EE_CLR,   XXXXXXX, OSL_FUN,
    XXXXXXX,  XXXXXXX, OSM_ALT, OSM_GUI, LOW_TAB, KC_SPC,  KC_ENT,  RSE_BSP,  OSM_SFT,  XXXXXXX,  XXXXXXX, XXXXXXX
),

/* Plover layer (http://opensteno.org)
 * ,-----------------------------------------------------------------------------------.
 * |   #  |   #  |   #  |   #  |   #  |   #  |   #  |   #  |   #  |   #  |   #  |   #  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |   S  |   T  |   P  |   H  |   *  |   *  |   F  |   P  |   L  |   T  |   D  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |   S  |   K  |   W  |   R  |   *  |   *  |   R  |   B  |   G  |   S  |   Z  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Exit |      |      |   A  |   O  |      |      |   E  |   U  |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_PLOVER] = LAYOUT_planck_grid(
    KC_1,    KC_1,    KC_1,    KC_1,    KC_1,    KC_1,    KC_1,    KC_1,    KC_1,    KC_1,    KC_1,    KC_1   ,
    XXXXXXX, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,
    XXXXXXX, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
    EXT_PLV, XXXXXXX, XXXXXXX, KC_C,    KC_V,    XXXXXXX, XXXXXXX, KC_N,    KC_M,    XXXXXXX, XXXXXXX, XXXXXXX
),

/* Adjust (Lower + Raise)
 *                      v------------------------RGB CONTROL--------------------v
 * ,-----------------------------------------------------------------------------------.
 * |      | Reset|Debug | RGB  |RGBMOD| HUE+ | HUE- | SAT+ | SAT- |BRGTH+|BRGTH-|  Del |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |MUSmod|Aud on|Audoff|AGnorm|AGswap|Qwerty|Colemk|Dvorak|Plover|      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |Voice-|Voice+|Mus on|Musoff|MIDIon|MIDIof|      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      | Alt  | GUI  |LowTab|Space |Enter |RaiBsp|Shift |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_ADJUST] = LAYOUT_planck_grid(
    _______, QK_BOOT, DB_TOGG, RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI,   RGB_VAD, KC_DEL ,
    _______, EE_CLR,  MU_NEXT, AU_ON,   AU_OFF,  AG_NORM, AG_SWAP, QWERTY,  COLEMAK, COLEMAKDH, PLOVER,  _______,
    _______, AU_PREV, AU_NEXT, MU_ON,   MU_OFF,  MI_ON,   MI_OFF,  _______, _______, _______,   _______, _______,
    _______, _______, OSM_ALT, OSM_GUI, LOW_TAB, KC_SPC,  KC_ENT,  RSE_BSP, OSM_SFT, _______,   _______, _______
)

};
/* clang-format on */

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

#ifdef AUDIO_ENABLE
float plover_song[][2]    = SONG(PLOVER_SOUND);
float plover_gb_song[][2] = SONG(PLOVER_GOODBYE_SOUND);
#endif

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QWERTY:
            if (record->event.pressed) {
                print("mode just switched to qwerty and this is a huge string\n");
                set_single_persistent_default_layer(_QWERTY);
            }
            return false;
            break;
        case COLEMAK:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_COLEMAK);
            }
            return false;
            break;
        case COLEMAKDH:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_COLEMAKDH);
            }
            return false;
            break;
        case DVORAK:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_DVORAK);
            }
            return false;
            break;
        case BACKLIT:
            if (record->event.pressed) {
                register_code(KC_RSFT);
            } else {
                unregister_code(KC_RSFT);
            }
            return false;
            break;
        case PLOVER:
            if (record->event.pressed) {
#ifdef AUDIO_ENABLE
                stop_all_notes();
                PLAY_SONG(plover_song);
#endif
                layer_off(_RAISE);
                layer_off(_LOWER);
                layer_off(_ADJUST);
                layer_on(_PLOVER);
                if (!eeconfig_is_enabled()) {
                    eeconfig_init();
                }
                keymap_config.raw  = eeconfig_read_keymap();
                keymap_config.nkro = 1;
                eeconfig_update_keymap(keymap_config.raw);
            }
            return false;
            break;
        case EXT_PLV:
            if (record->event.pressed) {
#ifdef AUDIO_ENABLE
                PLAY_SONG(plover_gb_song);
#endif
                layer_off(_PLOVER);
            }
            return false;
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
    return true;
}

/* clang-format off */
float melody[8][2][2] = {
    {{440.0f, 8}, {440.0f, 24}}, 
    {{440.0f, 8}, {440.0f, 24}}, 
    {{440.0f, 8}, {440.0f, 24}}, 
    {{440.0f, 8}, {440.0f, 24}}, 
    {{440.0f, 8}, {440.0f, 24}}, 
    {{440.0f, 8}, {440.0f, 24}}, 
    {{440.0f, 8}, {440.0f, 24}}, 
    {{440.0f, 8}, {440.0f, 24}},
};
/* clang-format on */

#define JUST_MINOR_THIRD 1.2
#define JUST_MAJOR_THIRD 1.25
#define JUST_PERFECT_FOURTH 1.33333333
#define JUST_TRITONE 1.42222222
#define JUST_PERFECT_FIFTH 1.33333333

#define ET12_MINOR_SECOND 1.059463
#define ET12_MAJOR_SECOND 1.122462
#define ET12_MINOR_THIRD 1.189207
#define ET12_MAJOR_THIRD 1.259921
#define ET12_PERFECT_FOURTH 1.33484
#define ET12_TRITONE 1.414214
#define ET12_PERFECT_FIFTH 1.498307	

deferred_token tokens[8];

uint32_t reset_note(uint32_t trigger_time, void *note) {
    *(float*)note = 440.0f;
    return 0;
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    cancel_deferred_exec(tokens[index]);
    if (clockwise) {
        melody[index][1][0] = melody[index][1][0] * ET12_MINOR_SECOND;
        melody[index][0][0] = melody[index][1][0] / ET12_PERFECT_FIFTH;
        audio_play_melody(&melody[index], 2, false);
    } else {
        melody[index][1][0] = melody[index][1][0] / ET12_MINOR_SECOND;
        melody[index][0][0] = melody[index][1][0] * ET12_TRITONE;
        audio_play_melody(&melody[index], 2, false);
    }
    tokens[index] = defer_exec(1000, reset_note, &melody[index][1][0]);
    return false;
}

bool dip_switch_update_user(uint8_t index, bool active) {
    switch (index) {
        case 0: {
#ifdef AUDIO_ENABLE
            static bool play_sound = false;
#endif
            if (active) {
#ifdef AUDIO_ENABLE
                if (play_sound) {
                    PLAY_SONG(plover_song);
                }
#endif
                layer_on(_ADJUST);
            } else {
#ifdef AUDIO_ENABLE
                if (play_sound) {
                    PLAY_SONG(plover_gb_song);
                }
#endif
                layer_off(_ADJUST);
            }
#ifdef AUDIO_ENABLE
            play_sound = true;
#endif
            break;
        }
    }
    return true;
}