#include "WriteToConsole.h"

static void setPins();

void WriteToConsole::begin() {
    pinMode(PIN_BTN_B, OUTPUT);
    pinMode(PIN_BTN_Y, OUTPUT);
    pinMode(PIN_BTN_SELECT, OUTPUT);
    pinMode(PIN_BTN_START, OUTPUT);
    pinMode(PIN_BTN_UP, OUTPUT);
    pinMode(PIN_BTN_DOWN, OUTPUT);
    pinMode(PIN_BTN_LEFT, OUTPUT);
    pinMode(PIN_BTN_RIGHT, OUTPUT);
    pinMode(PIN_BTN_A, OUTPUT);
    pinMode(PIN_BTN_X, OUTPUT);
    pinMode(PIN_BTN_SHOULDER_LEFT, OUTPUT);
    pinMode(PIN_BTN_SHOULDER_RIGHT, OUTPUT);

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

static void setPins() {
    ButtonData combinedData = ConsoleWriter.getLatestData();

    digitalWrite(PIN_BTN_B, combinedData.B);
    digitalWrite(PIN_BTN_Y, combinedData.Y);
    digitalWrite(PIN_BTN_SELECT, combinedData.SELECT);
    digitalWrite(PIN_BTN_START, combinedData.START);
    digitalWrite(PIN_BTN_UP, combinedData.UP);
    digitalWrite(PIN_BTN_DOWN, combinedData.DOWN);
    digitalWrite(PIN_BTN_LEFT, combinedData.LEFT);
    digitalWrite(PIN_BTN_RIGHT, combinedData.RIGHT);
    digitalWrite(PIN_BTN_A, combinedData.A);
    digitalWrite(PIN_BTN_X, combinedData.X);
    digitalWrite(PIN_BTN_SHOULDER_LEFT, combinedData.SHOULDER_LEFT);
    digitalWrite(PIN_BTN_SHOULDER_RIGHT, combinedData.SHOULDER_RIGHT);
}

const ButtonData WriteToConsole::getLatestData() {
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

WriteToConsole ConsoleWriter;
