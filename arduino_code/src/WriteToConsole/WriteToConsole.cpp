#include "WriteToConsole.h"

/*
 * These constants are used to set the ports directly via the registers.
 * The active bits correspond to used pins.
 */
static const uint8_t PORTD_MASK = B11110000;
static const uint8_t PORTB_MASK = B00011111;
static const uint8_t PORTC_MASK = B00000111;

void WriteToConsole::begin() const {
    // set pins 4-7 as output
    DDRD |= PORTD_MASK;

    // equivalent to:
    //pinMode(PIN_BTN_B, OUTPUT);
    //pinMode(PIN_BTN_Y, OUTPUT);
    //pinMode(PIN_BTN_SELECT, OUTPUT);
    //pinMode(PIN_BTN_START, OUTPUT);

    // set pins 8-12 as output
    DDRB |= PORTB_MASK;

    // equivalent to:
    //pinMode(PIN_BTN_UP, OUTPUT);
    //pinMode(PIN_BTN_DOWN, OUTPUT);
    //pinMode(PIN_BTN_LEFT, OUTPUT);
    //pinMode(PIN_BTN_RIGHT, OUTPUT);

    // sets pins A0-A2 as output
    DDRC |= PORTC_MASK;

    // equivalent to:
    //pinMode(PIN_BTN_A, OUTPUT);
    //pinMode(PIN_BTN_X, OUTPUT);
    //pinMode(PIN_BTN_SHOULDER_LEFT, OUTPUT);
    //pinMode(PIN_BTN_SHOULDER_RIGHT, OUTPUT);

    // initialize pins with default button data (all set to HIGH)
    setPins();
}

void WriteToConsole::prepareData(const ButtonData buttonData) {
    lastPreparedData = buttonData;
    setPins();
}

void WriteToConsole::addData(const ButtonData additionalData) {
    lastAdditionalData = additionalData;
    setPins();
}

void WriteToConsole::setPins() const {
    ButtonData combinedData = getLatestData();

    const uint16_t* memoryLocation = (uint16_t*)&combinedData;

    // set values for pin 4-7
    PORTD = (PORTD & ~PORTD_MASK) | (memoryLocation[0] << 4 & PORTD_MASK);

    // equivalent to
    //digitalWrite(PIN_BTN_B, combinedData.B);
    //digitalWrite(PIN_BTN_Y, combinedData.Y);
    //digitalWrite(PIN_BTN_SELECT, combinedData.SELECT);
    //digitalWrite(PIN_BTN_START, combinedData.START);

    // set values for pin 8-12
    PORTB = (PORTB & ~PORTB_MASK) | (memoryLocation[0] >> 4 & PORTB_MASK);

    // equivalent to
    //digitalWrite(PIN_BTN_UP, combinedData.UP);
    //digitalWrite(PIN_BTN_DOWN, combinedData.DOWN);
    //digitalWrite(PIN_BTN_LEFT, combinedData.LEFT);
    //digitalWrite(PIN_BTN_RIGHT, combinedData.RIGHT);
    //digitalWrite(PIN_BTN_A, combinedData.A);

    // set values for pins A0-A2
    PORTC |= (PORTC & ~PORTC_MASK) | (memoryLocation[0] >> 9 & PORTC_MASK);

    // equivalent to
    //digitalWrite(PIN_BTN_X, combinedData.X);
    //digitalWrite(PIN_BTN_SHOULDER_LEFT, combinedData.SHOULDER_LEFT);
    //digitalWrite(PIN_BTN_SHOULDER_RIGHT, combinedData.SHOULDER_RIGHT);

    // wait some time to have the pins actually change their states
    delayMicroseconds(20);
}

ButtonData WriteToConsole::getLatestData() const {
    ButtonData combinedData;
    combinedData.B = lastPreparedData.B && lastAdditionalData.B;
    combinedData.Y = lastPreparedData.Y && lastAdditionalData.Y;
    combinedData.SELECT = lastPreparedData.SELECT && lastAdditionalData.SELECT;
    combinedData.START = lastPreparedData.START && lastAdditionalData.START;
    combinedData.UP = lastPreparedData.UP && lastAdditionalData.UP;
    combinedData.DOWN = lastPreparedData.DOWN && lastAdditionalData.DOWN;
    combinedData.LEFT = lastPreparedData.LEFT && lastAdditionalData.LEFT;
    combinedData.RIGHT = lastPreparedData.RIGHT && lastAdditionalData.RIGHT;
    combinedData.A = lastPreparedData.A && lastAdditionalData.A;
    combinedData.X = lastPreparedData.X && lastAdditionalData.X;
    combinedData.SHOULDER_LEFT = lastPreparedData.SHOULDER_LEFT && lastAdditionalData.SHOULDER_LEFT;
    combinedData.SHOULDER_RIGHT = lastPreparedData.SHOULDER_RIGHT && lastAdditionalData.SHOULDER_RIGHT;

    return combinedData;
}

void WriteToConsole::reset() {
    lastPreparedData = ButtonData();
    lastAdditionalData = ButtonData();

    setPins();
}

WriteToConsole ConsoleWriter;
