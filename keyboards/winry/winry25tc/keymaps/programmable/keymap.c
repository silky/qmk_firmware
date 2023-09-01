#include QMK_KEYBOARD_H

enum my_layers {
    _FIRST_LAYER
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_FIRST_LAYER] = LAYOUT(
        KC_A, KC_B, KC_C, KC_D, KC_E,
        KC_F, KC_G, KC_H, KC_I, KC_J,
        KC_K, KC_L, KC_M, KC_N, KC_O,
        KC_P, KC_Q, KC_R, KC_S, KC_T,
        KC_U, KC_V, KC_W, KC_X, KC_Y
    ),
};

/* Because snake pattern of leds */
const uint8_t remap[25] = {
    20,21,22,23,24,
    19,6,7,8,9,
    18,5,0,1,10,
    17,4,3,2,11,
    16,15,14,13,12,
};

void raw_hid_receive(uint8_t *data, uint8_t length) {
    if( data[0] == 1 ){
        setrgb(RGB_WHITE, &led[remap[data[1] - 'A']]);
    }

    if( data[0] == 0 ){
        setrgb(RGB_RED, &led[remap[data[1] - 'A']]);
    }

    rgblight_set();
}

void keyboard_post_init_user(void) {
    /* rgblight_enable_noeeprom(); */
    /* rgblight_mode(RGBLIGHT_MODE_STATIC_LIGHT); */
    /* rgblight_set(); */
}

