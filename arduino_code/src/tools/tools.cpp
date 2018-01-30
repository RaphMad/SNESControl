#include "tools.h"

void buttonDataToBytes(const ButtonData* const buttonData, byte* const buf) {
    memcpy(buf, buttonData, sizeof(ButtonData));
}

const ButtonData bytesToButtonData(const byte* const bytes) {
    ButtonData buttonData;

    // note that buttons are stored in reverse order
    buttonData.B = bitRead(bytes[0], 7);
    buttonData.Y = bitRead(bytes[0], 6);
    buttonData.SELECT = bitRead(bytes[0], 5);
    buttonData.START = bitRead(bytes[0], 4);
    buttonData.UP = bitRead(bytes[0], 3);
    buttonData.DOWN = bitRead(bytes[0], 2);
    buttonData.LEFT = bitRead(bytes[0], 1);
    buttonData.RIGHT = bitRead(bytes[0], 0);

    buttonData.A = bitRead(bytes[1], 7);
    buttonData.X = bitRead(bytes[1], 6);
    buttonData.SHOULDER_LEFT = bitRead(bytes[1], 5);
    buttonData.SHOULDER_RIGHT = bitRead(bytes[1], 4);

    buttonData.pressedAt = bytesToInt(bytes + 2);

    return buttonData;
}

void intToBytes(const uint16_t value, byte* const buf) {
    memcpy(buf, &value, sizeof(uint16_t));
}

uint16_t bytesToInt(const byte* const bytes) {
    return (bytes[1] << 8) + bytes[0];
}

String formatButtonData(const ButtonData& buttonData) {
    String pressedButtons = "";

    if (!buttonData.B) pressedButtons += "B, ";
    if (!buttonData.Y) pressedButtons += "Y, ";
    if (!buttonData.SELECT) pressedButtons += "SELECT, ";
    if (!buttonData.START) pressedButtons += "START, ";
    if (!buttonData.UP) pressedButtons += "UP, ";
    if (!buttonData.DOWN) pressedButtons += "DOWN, ";
    if (!buttonData.LEFT) pressedButtons += "LEFT, ";
    if (!buttonData.RIGHT) pressedButtons += "RIGHT, ";
    if (!buttonData.A) pressedButtons += "A, ";
    if (!buttonData.X) pressedButtons += "X, ";
    if (!buttonData.SHOULDER_LEFT) pressedButtons += "SHOULDER_LEFT, ";
    if (!buttonData.SHOULDER_RIGHT) pressedButtons += "SHOULDER_RIGHT, ";

    if (pressedButtons.endsWith(", ")) {
        pressedButtons = pressedButtons.substring(0, pressedButtons.length() - 2);
    }

    return pressedButtons;
}

uint16_t getFreeRam() {
  extern uint16_t __heap_start, *__brkval;
  uint16_t v;
  return (uint16_t) &v - (__brkval == 0 ? (uint16_t) &__heap_start : (uint16_t) __brkval);
}
