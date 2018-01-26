#ifndef types_h
#define types_h

#include <stdint.h>
#include <Arduino.h>

// pin to button mapping for console communication
const uint8_t PIN_LATCH = 2;

const uint8_t PIN_BTN_B = 4;
const uint8_t PIN_BTN_Y = 5;
const uint8_t PIN_BTN_SELECT = 6;
const uint8_t PIN_BTN_START = 7;
const uint8_t PIN_BTN_UP = 8;
const uint8_t PIN_BTN_DOWN = 9;
const uint8_t PIN_BTN_LEFT = 10;
const uint8_t PIN_BTN_RIGHT = 11;
const uint8_t PIN_BTN_A = 12;

const uint8_t PIN_BTN_X = A0;
const uint8_t PIN_BTN_SHOULDER_LEFT = A1;
const uint8_t PIN_BTN_SHOULDER_RIGHT = A2;

// pin to button mapping fo controller communication
const uint8_t PIN_CONTROLLER_CLOCK = A3;
const uint8_t PIN_CONTROLLER_LATCH = A4;
const uint8_t PIN_CONTROLLER_DATA = A5;

/*
 * This corresponds to the bitwise represenation of the buttons in the SNES protocol.
 * Note that true means NOT pressed.
 */
struct ButtonData {
    bool B:1;
    bool Y:1;
    bool SELECT:1;
    bool START:1;
    bool UP:1;
    bool DOWN:1;
    bool LEFT:1;
    bool RIGHT:1;
    bool A:1;
    bool X:1;
    bool SHOULDER_LEFT:1;
    bool SHOULDER_RIGHT:1;

    // unused padding bits
    bool UNUSED_13: 1;
    bool UNUSED_14: 1;
    bool UNUSED_15: 1;
    bool UNUSED_16: 1;

    ButtonData() {
        // initialize all fields with true, this makes creating manual instances easier (remember that true means NOT pressed)
        B = true;
        Y = true;
        SELECT = true;
        START = true;
        UP = true;
        DOWN = true;
        LEFT = true;
        RIGHT = true;
        A = true;
        X = true;
        SHOULDER_LEFT = true;
        SHOULDER_RIGHT = true;
        UNUSED_13 = true;
        UNUSED_14 = true;
        UNUSED_15 = true;
        UNUSED_16 = true;
    }
};

#endif