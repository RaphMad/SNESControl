#ifndef common_h
#define common_h

#include <Arduino.h>

typedef enum {
    // pin to button mapping for console communication
    PIN_LATCH = 2,

    PIN_BTN_B = 4,
    PIN_BTN_Y = 5,
    PIN_BTN_SELECT = 6,
    PIN_BTN_START = 7,
    PIN_BTN_UP = 8,
    PIN_BTN_DOWN = 9,
    PIN_BTN_LEFT = 10,
    PIN_BTN_RIGHT = 11,
    PIN_BTN_A = 12,

    PIN_BTN_X = A0,
    PIN_BTN_SHOULDER_LEFT = A1,
    PIN_BTN_SHOULDER_RIGHT = A2,

    // pin to button mapping fo controller communication
    PIN_CONTROLLER_CLOCK = A3,
    PIN_CONTROLLER_LATCH = A4,
    PIN_CONTROLLER_DATA = A5
} ConnectedPin;

/*
 * This corresponds to the bitwise represenation of the buttons in the SNES protocol.
 * Note that true means NOT pressed.
 */
struct ButtonData {
    bool B :1;
    bool Y :1;
    bool SELECT :1;
    bool START :1;
    bool UP :1;
    bool DOWN :1;
    bool LEFT :1;
    bool RIGHT :1;

    bool A :1;
    bool X :1;
    bool SHOULDER_LEFT :1;
    bool SHOULDER_RIGHT :1;

    /*
     * Unused padding bits.
     */
    bool UNUSED_BIT5 :1;
    bool UNUSED_BIT6 :1;
    bool UNUSED_BIT7 :1;
    bool UNUSED_BIT8 :1;

    /*
     * The latch number of this set of input data.
     */
    uint32_t latchNumber;

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
        UNUSED_BIT5 = true;
        UNUSED_BIT6 = true;
        UNUSED_BIT7 = true;
        UNUSED_BIT8 = true;

        latchNumber = 0;
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
    INFO_RESPONSE = 14,
    RESET_DATA = 15
} MessageType;

/*
 * Number of message types - needs to be higher than the highest message type above.
 */
const uint8_t NUM_MESSAGE_TYPES = 16;

/*
 * Maximum size of the message content for incoming messages.
 *
 * Minimum 4 bytes, since this is the size of a single ButtonData structure.
 * Maximum size is at about 230 (depending on current stack size).
 */
const uint8_t MAX_CONTENT_SIZE = 32;

/*
 * Holds information about the application.
 */
struct AppInfo {
    uint16_t maxLoopDuration;

    bool isInSaveMode;
    bool isInReplayMode;
};

#endif
