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

enum planck_layers { _QWERTY, _COLEMAK, _COLEMAKDH, _DVORAK, _LOWER, _RAISE, _FUNC, _NUM, _PLOVER, _ADJUST };

enum planck_keycodes { PLOVER = SAFE_RANGE, BACKLIT, EXT_PLV, KC_JIGG, ALT_TAB, SNAP_LFT, SNAP_RT, SNAP_TOP, SNAP_BTM };

#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)
#define FUNC MO(_FUNC)
#define NUM TG(_NUM)

#define QWERTY PDF(_QWERTY)
#define COLEMAK PDF(_COLEMAK)
#define COLEMAKDH PDF(_COLEMAKDH)
#define DVORAK PDF(_DVORAK)

enum combos {
  JK_ESC,
  QW_CAPS
};

const uint16_t PROGMEM jk_combo[] = {KC_J, KC_K, COMBO_END};
const uint16_t PROGMEM qw_combo[] = {KC_Q, KC_W, COMBO_END};


combo_t key_combos[COMBO_COUNT] = {
  // For Vim, put Escape on the home row
  [JK_ESC]    = COMBO(jk_combo, KC_ESC),
  [QW_CAPS]   = COMBO(qw_combo, KC_CAPS)

};

// Tap Dance declarations
enum {
    TD_SPC_ENT,
};

// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Space, twice for Enter Lock
    [TD_SPC_ENT] = ACTION_TAP_DANCE_DOUBLE(KC_SPC, KC_ENT),
};

static uint16_t alt_tab_timer = 0;
static bool alt_tab_active = false;

void matrix_scan_user(void) {
    if (alt_tab_active && timer_elapsed(alt_tab_timer) > 1000) {
        // No presses for 1000ms, release Alt
        unregister_code(KC_LALT);
        alt_tab_active = false;
    }
}

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

// For _QWERTY layer
#define OSL_FUN  OSL(_FUNC)
#define LOW_TAB  LT(_LOWER, KC_TAB)
#define RSE_BSP  LT(_RAISE, KC_BSPC)
#define OSM_SFT  OSM(MOD_LSFT)
#define OSM_RSFT  OSM(MOD_RSFT)
#define OSM_ALT  OSM(MOD_LALT)
#define OSM_CTL  OSM(MOD_RCTL)
#define OSM_GUI  OSM(MOD_LGUI)

/* clang-format off */
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Qwerty
 * ,-----------------------------------------------------------------------------------.
 * | Alt  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | \ |  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Ctrl |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  | Func |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Brite|      | Alt  | GUI  |LowTab|Space |Enter |RaiBsp|Shift |      |      |LCKWIN|
 * `-----------------------------------------------------------------------------------'
 */
[_QWERTY] = LAYOUT_planck_grid(
    KC_LALT,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,        KC_P,    KC_BSLS,
    KC_LSFT,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,        KC_SCLN, KC_QUOT,
    KC_LCTL,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,      KC_SLSH, OSL_FUN ,
    NUM,  KC_LCTL, ALT_TAB, KC_LGUI, LOW_TAB, KC_SPC,  KC_ENT,  RSE_BSP, KC_RSFT, LCA(KC_DEL), KC_JIGG, LGUI(KC_L)
),

/* Colemak
 * ,-----------------------------------------------------------------------------------.
 * | Alt  |   Q  |   W  |   F  |   P  |   G  |   J  |   L  |   U  |   Y  |   ;  | Bksp |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Ctrl |   A  |   R  |   S  |   T  |   D  |   H  |   N  |   E  |   I  |   O  |  '   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   K  |   M  |   ,  |   .  |   /  | Func |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Brite|      | Alt  | GUI  |LowTab|Space |Enter |RaiBsp|Shift |      |      |LCKWIN|
 * `-----------------------------------------------------------------------------------'
 */
[_COLEMAK] = LAYOUT_planck_grid(
    KC_LALT,  KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,    KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN, KC_BSPC,
    KC_LSFT,  KC_A,    KC_R,    KC_S,    KC_T,    KC_D,    KC_H,    KC_N,    KC_E,    KC_I,    KC_O,    KC_QUOT,
    KC_LCTL, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, OSL_FUN ,
    _______, _______, _______, OSM_GUI, LOW_TAB, KC_SPC,  KC_ENT,  RSE_BSP, KC_RSFT, _______, _______, _______
),

/* ColemakDH
 * ,-----------------------------------------------------------------------------------.
 * | Alt  |   Q  |   W  |   F  |   P  |   B  |   J  |   L  |   U  |   Y  |   ;  | Bksp |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Ctrl |   A  |   R  |   S  |   T  |   G  |   M  |   N  |   E  |   I  |   O  |  '   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   D  |   V  |   K  |   H  |   ,  |   .  |   /  | Func |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      | Alt  | GUI  |LowTab|Space |Enter |RaiBsp|Shift |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_COLEMAKDH] = LAYOUT_planck_grid(
    KC_LALT,  KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,    KC_J,    KC_L,    KC_U,     KC_Y,    KC_SCLN, KC_BSPC,
    KC_LSFT,  KC_A,    KC_R,    KC_S,    KC_T,    KC_D,    KC_H,    KC_N,    KC_E,     KC_I,    KC_O,    KC_QUOT,
    KC_LCTL,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_K,    KC_M,    KC_COMM,  KC_DOT,  KC_SLSH, OSL_FUN ,
    _______,  XXXXXXX, _______, OSM_GUI, LOW_TAB, KC_SPC,  KC_ENT,  RSE_BSP, KC_RSFT,  XXXXXXX, XXXXXXX, XXXXXXX
),

/* Dvorak
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   '  |   ,  |   .  |   P  |   Y  |   F  |   G  |   C  |   R  |   L  | Bksp |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Esc  |   A  |   O  |   E  |   U  |   I  |   D  |   H  |   T  |   N  |   S  |  /   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift|   ;  |   Q  |   J  |   K  |   X  |   B  |   M  |   W  |   V  |   Z  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Brite| Ctrl | Alt  | GUI  |Lower |    Space    |Raise | Left | Down |  Up  |Right |
 * `-----------------------------------------------------------------------------------'
 */
[_DVORAK] = LAYOUT_planck_grid(
    KC_TAB,  KC_QUOT, KC_COMM, KC_DOT,  KC_P,    KC_Y,    KC_F,    KC_G,    KC_C,    KC_R,    KC_L,    KC_BSPC,
    KC_ESC,  KC_A,    KC_O,    KC_E,    KC_U,    KC_I,    KC_D,    KC_H,    KC_T,    KC_N,    KC_S,    KC_SLSH,
    KC_LSFT, KC_SCLN, KC_Q,    KC_J,    KC_K,    KC_X,    KC_B,    KC_M,    KC_W,    KC_V,    KC_Z,    KC_ENT ,
    _______, KC_LCTL, _______, KC_LGUI, LOWER,   KC_RSFT, KC_SPC,  RAISE,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT
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
    _______, KC_1,    KC_2,    KC_3,   KC_4,    KC_5,    KC_6,    KC_7,     KC_8,    KC_9,    KC_0,    KC_BSLS,
    _______, _______, _______, KC_EQL, KC_LBRC, KC_LCBR, KC_RCBR, KC_RBRC,  KC_MINS, KC_EQL,  _______, OSL_FUN,
    _______, _______, _______, KC_LGUI,LOW_TAB, KC_SPC,  KC_ENT,  RSE_BSP,  KC_RSFT, _______, _______, _______
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
    _______,  KC_INS,  KC_END,  KC_PGDN, XXXXXXX, XXXXXXX, XXXXXXX, KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX, _______,
    _______,  XXXXXXX, XXXXXXX, KC_EQL,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, OSL_FUN,
    _______,  _______, _______, KC_LGUI,LOW_TAB, KC_SPC,  KC_ENT,  RSE_BSP,  KC_RSFT, _______, _______, _______
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
    _______,  DB_TOGG, XXXXXXX,   XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX,  SNAP_TOP, XXXXXXX, XXXXXXX, XXXXXXX,
    _______,  KC_CALC, LSG(KC_S), XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX, SNAP_LFT, SNAP_BTM, SNAP_RT, XXXXXXX, XXXXXXX,
    _______,  XXXXXXX, XXXXXXX,   XXXXXXX, LCS(KC_V), XXXXXXX, XXXXXXX, QK_RBT,   QK_BOOT,  EE_CLR,  XXXXXXX, OSL_FUN,
    _______,  _______, _______,   KC_LGUI, LOW_TAB,   KC_SPC,  KC_ENT,  RSE_BSP,  KC_RSFT,  _______, _______, _______
),

/* Num
 * ,-----------------------------------------------------------------------------------.
 * |      |      |      |      |      |      |  -   |  +   |  7   |  8   |  9   |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |  *   |  /   |  4   |  5   |  6   |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |  %   |  1   |  2   |  3   | Func |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      | Alt  | GUI  |LowTab|Space |Enter |RaiBsp|Shift |  0   |  .   |      |
 * `-----------------------------------------------------------------------------------'
 */
[_NUM] = LAYOUT_planck_grid(
    _______,  XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX,   XXXXXXX, KC_PMNS, KC_PPLS,  KC_7,    KC_8,    KC_9,    XXXXXXX,
    _______,  XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX,   XXXXXXX, KC_PAST, KC_PSLS,  KC_4,    KC_5,    KC_9,    XXXXXXX,
    _______,  XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX, LCS(KC_5),KC_1,    KC_2,    KC_9,    OSL_FUN,
    _______,  _______, _______,   KC_LGUI, LOW_TAB,   KC_SPC,  KC_ENT,  RSE_BSP,  KC_RSFT, KC_0,    KC_DOT,  _______
),

/* Plover layer (http://opensteno.org)
 * ,-----------------------------------------------------------------------------------.
 * |   #  |   #  |   #  |   #  |   #  |   #  |   #  |   #  |   #  |   #  |   #  |   #  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |   S  |   T  |   P  |   H  |   *  |   *  |   F  |   P  |   L  |   T  |   D  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |   S  |   K  |   W  |   R  |   *  |   *  |   R  |   B  |   G  |   S  |   Z  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Exit |      |      |   A  |   O  |             |   E  |   U  |      |      |      |
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
 * |      |Voice-|Voice+|Mus on|Musoff|MIDIon|MIDIof|ClmkDH|      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_ADJUST] = LAYOUT_planck_grid(
    _______, QK_BOOT, DB_TOGG, UG_TOGG, UG_NEXT, UG_HUEU, UG_HUED, UG_SATU, UG_SATD, UG_SPDU, UG_SPDD, KC_DEL ,
    _______, EE_CLR,  MU_NEXT, AU_ON,   AU_OFF,  AG_NORM, AG_SWAP, QWERTY,  COLEMAK, DVORAK,  PLOVER,  _______,
    _______, AU_PREV, AU_NEXT, MU_ON,   MU_OFF,  MI_ON,   MI_OFF,  COLEMAKDH, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
)

};

#ifdef ENCODER_MAP_ENABLE
/* Rotary Encoders
 */
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    /* Qwerty
     *    v- (index) Clockwise / Counter Clockwise                        v- (index) Clockwise / Counter Clockwise
     * ,---------------------------------------------------------------------------------------.
     * | (0) Vol-    / Vol+    |   |   |   |   |   |   |   |   |   |   | (4) Vol-    / Vol+    |
     * |-----------------------+---+---+---+---+---+---+---+---+---+---+-----------------------|
     * | (1) KC_MNXT / KC_MPRV |   |   |   |   |   |   |   |   |   |   | (5) KC_MNXT / KC_MPRV |
     * |-----------------------+---+---+---+---+---+---+---+---+---+---+-----------------------|
     * | (2) KC_WBAK / KC_WFWD |   |   |   |   |   |   |   |   |   |   | (6) KC_SPC  / KC_ENT  |
     * |-----------------------+---+---+---+---+---+---+---+---+---+---+-----------------------|
     * | (3) KC_LEFT / KC_RGHT |   |   |   |   |       |   |   |   |   | (7) KC_DOWN / KC_UP   |
     * `---------------------------------------------------------------------------------------'
     */
    [_QWERTY] = {
        // LEFT SIDE (index 0 to 3)
        ENCODER_CCW_CW(KC_VOLU, KC_VOLD),
        ENCODER_CCW_CW(KC_MNXT, KC_MPRV),
        ENCODER_CCW_CW(KC_WBAK, KC_WFWD),
        ENCODER_CCW_CW(KC_LEFT, KC_RGHT),
        // RIGHT SIDE (index 4 to 7)
        ENCODER_CCW_CW(KC_VOLU, KC_VOLD),
        ENCODER_CCW_CW(KC_MNXT, KC_MPRV),
        ENCODER_CCW_CW(KC_SPC,  KC_ENT),
        ENCODER_CCW_CW(KC_DOWN, KC_UP)
    },

    /* Adjust (Lower + Raise)
     *    v- (index) Clockwise / Counter Clockwise                        v- (index) Clockwise / Counter Clockwise
     * ,---------------------------------------------------------------------------------------.
     * | (0) _______ / _______ |   |   |   |   |   |   |   |   |   |   | (4) _______ / _______ |
     * |-----------------------+---+---+---+---+---+---+---+---+---+---+-----------------------|
     * | (1) _______ / _______ |   |   |   |   |   |   |   |   |   |   | (5) _______ / _______ |
     * |-----------------------+---+---+---+---+---+---+---+---+---+---+-----------------------|
     * | (2) UG_NEXT / UG_PREV |   |   |   |   |   |   |   |   |   |   | (6) SAT- / SAT+       |
     * |-----------------------+---+---+---+---+---+---+---+---+---+---+-----------------------|
     * | (3) UG_VALD / UG_VALU |   |   |   |   |       |   |   |   |   | (7) HUE- / HUE+       |
     * `---------------------------------------------------------------------------------------'
     */
    [_ADJUST] = {
        // LEFT SIDE (index 0 to 3)
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(UG_NEXT, UG_PREV),
        ENCODER_CCW_CW(UG_VALD, UG_VALU),
        // RIGHT SIDE (index 4 to 7)
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(UG_SATD,  UG_SATU),
        ENCODER_CCW_CW(UG_HUEU,  UG_HUED)
    }
};
#endif
/* clang-format on */

#ifdef AUDIO_ENABLE
float plover_song[][2]    = SONG(PLOVER_SOUND);
float plover_gb_song[][2] = SONG(PLOVER_GOODBYE_SOUND);
#endif

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

bool play_encoder_melody(uint8_t index, bool clockwise);

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
    case _QWERTY:
        if(is_jiggling) {
            rgblight_sethsv_range(43, 255, 255, 0, 9);
        } else {
            // Set Backlight LEDS to PURPLE
            rgblight_sethsv_range(191, 255, 255, 0, 9);
        }
        break;
    case _FUNC:
        if(is_jiggling) {
            rgblight_sethsv_range(43, 255, 255, 0, 9);
        } else {
            // Set Backlight LEDS to PURPLE
            rgblight_sethsv_range(170, 255, 255, 0, 9);
        }
        break;
    case _PLOVER:
        rgblight_sethsv_range(21, 255, 255, 0, 9);
        break;
    }
    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
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
#ifdef ENCODER_MAP_ENABLE
    if (IS_ENCODEREVENT(record->event) && record->event.pressed) {
        play_encoder_melody(record->event.key.col, record->event.type == ENCODER_CCW_EVENT);
    }
#endif
    switch (keycode) {
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
                eeconfig_read_keymap(&keymap_config);
                keymap_config.nkro = 1;
                eeconfig_update_keymap(&keymap_config);
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
        case KC_JIGG:
            if (record->event.pressed) {
                    is_jiggling = !is_jiggling; /*flip boolean to true*/
                    #ifdef CONSOLE_ENABLE
                    dprintf("is_jiggling: %s\n", is_jiggling ? "true" : "false");
                    #endif
                    if(is_jiggling) {
                        layer_on(_FUNC);
                    } else {
                        layer_off(_FUNC);
                    }
            }

            break;
        case ALT_TAB:
            if (record->event.pressed) {
                if (!alt_tab_active) {
                    alt_tab_active = true;
                    register_code(KC_LALT);
                    register_code(KC_TAB);
                } else {
                    // Subsequent press while active, send another Tab
                    register_code(KC_TAB);
                }
                alt_tab_timer = timer_read();
            } else {
                unregister_code(KC_TAB);
            }
            return false;
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
    return process_record_keymap(keycode, record) && process_record_secrets(keycode, record);
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

bool play_encoder_melody(uint8_t index, bool clockwise) {
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

bool encoder_update_user(uint8_t index, bool clockwise) {
    return play_encoder_melody(index, clockwise);
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
