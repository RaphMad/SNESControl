#ifndef types_h
#define types_h

#include <stdint.h>
#include <Arduino.h>

// pin to button mapping for console communication
const byte PIN_LATCH = 2;

const byte PIN_BTN_B = 4;
const byte PIN_BTN_Y = 5;
const byte PIN_BTN_SELECT = 6;
const byte PIN_BTN_START = 7;
const byte PIN_BTN_UP = 8;
const byte PIN_BTN_DOWN = 9;
const byte PIN_BTN_LEFT = 10;
const byte PIN_BTN_RIGHT = 11;
const byte PIN_BTN_A = 12;

const byte PIN_BTN_X = A0;
const byte PIN_BTN_SHOULDER_LEFT = A1;
const byte PIN_BTN_SHOULDER_RIGHT = A2;

// pin to button mapping fo controller communication
const byte PIN_CONTROLLER_CLOCK = A3;
const byte PIN_CONTROLLER_LATCH = A4;
const byte PIN_CONTROLLER_DATA = A5;

/*
 * This corresponds to the bitwise represenation of the buttons in the SNES protocol.
 * Note that true means NOT pressed.
 */
struct ButtonData {
    // reverse bit order within the bytes
    // (just as a convenience, this way the resulting bits can be aligned to clock cycles)
    bool RIGHT:1;
    bool LEFT:1;
    bool DOWN:1;
    bool UP:1;
    bool START:1;
    bool SELECT:1;
    bool Y:1;
    bool B:1;

    // unused padding bits
    bool UNUSED_13: 1;
    bool UNUSED_14: 1;
    bool UNUSED_15: 1;
    bool UNUSED_16: 1;
    bool SHOULDER_RIGHT:1;
    bool SHOULDER_LEFT:1;
    bool X:1;
    bool A:1;

    ButtonData() {
        // initialize all fields with true, this makes creating manual instances easier
        // (remember that true means NOT pressed)
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

/*
 * Identifiers of messages that are exchanged between the Arduino and the client application.
 * Message have the following form:
 *
 * <START_BYTE> <MessageType> ... <Payload bytes> ... <END_BYTE>
 *
 * <MessageType> and <Payload bytes> will need to be encoded to not contain <START_BYTE> and <END_BYTE>.
 * The size of <Payload bytes> is message specific.
 */
typedef enum {
    // start with a value of 3 for more efficient encoding (the encoding will use 2 bytes for 0,1,2)
    ENABLE_SAVE = 3,
    SAVE = 4,
    ENABLE_LOAD = 5,
    LOAD = 6,
    PING = 7,
    PONG = 8,
    PRINT = 9,
    REQUEST_STATUS = 10,
    DISABLE_SAVE = 11,
    DISABLE_LOAD = 12,
    LOAD_RESPONSE = 13,
    INFO_RESPONSE = 14
} MessageType;

/*
 * Maximum size of the message content.
 */
const byte MAX_CONTENT_SIZE = 128;

/*
 * Holds information about the application.
 */
struct AppInfo {
    int maxLoopDuration;

    int lastLatchDuration;
    int numberOfLatches;
    int longLatches;
    int shortLatches;

    bool isInSaveMode;
    bool isInReplayMode;
};

#endif
