/* Copyright 2025 Noon van der Silk
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


/*
 *
 * Notes:
 *
 * From the root directory.
 *
 * > nix-shell
 *
 * To compile:
 *
 * > qmk compile -kb winry/winry25tc -km color-toggle
 *
 * To flash:
 *
 * > qmk flash -kb winry/winry25tc -km color-toggle
 *
 * (Need to first press the little button under the thing.)
 *
 * */

#include <stdlib.h>

#include QMK_KEYBOARD_H

// clang-format off

// Exact keymap is irrelevant as we're using rows/cols
// but we need _something_ set so we're using no-ops
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
    )};

uint8_t tiles[5][5] = {
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
};

/* Because snake pattern of leds */
const uint8_t remap[25] = {
    20,21,22,23,24,
    19,6,7,8,9,
    18,5,0,1,10,
    17,4,3,2,11,
    16,15,14,13,12,
};

// clang-format on

bool is_same_colour(void) {
    if(tiles[0][0] == 0){
        return false;
    }

    for (uint8_t y = 0; y < 5; y++) {
        for (uint8_t x = 0; x < 5; x++) {
            if (tiles[x][y] != tiles[0][0]) {
                return false;
            }
        }
    }
    return true;
}

void do_move(uint8_t x, uint8_t y) {
    // Randomly change the colour to something different
    uint8_t new;

    while ( true ){
        new = rand() % 4;
        if(new != tiles[x][y]) {
            break;
        }
    }
    tiles[x][y] = new;
}

void refresh_leds(void) {
    for (uint8_t y = 0; y < 5; y++) {
        for (uint8_t x = 0; x < 5; x++) {
            uint8_t tile  = tiles[x][y];
            uint8_t index = (y * 5) + x;
            switch(tile){
                case 0:
                    setrgb(RGB_WHITE, &led[remap[index]]);
                    break;
                case 1:
                    setrgb(252, 64, 255, &led[remap[index]]);
                    break;
                case 2:
                    setrgb(28, 160, 255, &led[remap[index]]);
                    break;
                case 3:
                    setrgb(245, 192, 34, &led[remap[index]]);
                    break;
                default:
                    setrgb(RGB_WHITE, &led[remap[index]]);

            }
        }
    }
    rgblight_set();
}

uint8_t initial_moves = 1;

void start_game(void) {
    rgblight_mode(RGBLIGHT_MODE_STATIC_LIGHT);

    // Reset colour
    for (uint8_t y = 0; y < 5; y++) {
        for (uint8_t x = 0; x < 5; x++) {
            tiles[x][y] = 0;
        }
    }
    refresh_leds();
}

void keyboard_post_init_user(void) {
    rgblight_enable_noeeprom();
    start_game();
}

bool won = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        if (won) {
            /* initial_moves++; */
            won = false;
            start_game();
        } else {
            uint8_t x = record->event.key.col;
            uint8_t y = record->event.key.row;
            do_move(x, y);
            if (is_same_colour()) {
                rgblight_mode(RGBLIGHT_MODE_RAINBOW_SWIRL);
                won = true;
            }
        }
    }
    refresh_leds();
    return true;
}
