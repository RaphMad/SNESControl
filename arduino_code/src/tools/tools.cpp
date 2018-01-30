#include "tools.h"

void intToBytes(int value, byte* buf) {
    buf[0] = value & 0xFF;
    buf[1] = (value >> 8) & 0xFF;
}

void buttonDataToBytes(ButtonData* buttonData, byte* buf) {
    memcpy(buf, buttonData, sizeof(ButtonData));
}

int bytesToInt(byte* bytes) {
    return (bytes[1] << 8) + bytes[0];
}

ButtonData bytesToButtonData(byte* bytes) {
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

String formatButtonData(ButtonData* buttonData) {
    String pressedButtons = "";

    if (!buttonData->B) pressedButtons += "B, ";
    if (!buttonData->Y) pressedButtons += "Y, ";
    if (!buttonData->SELECT) pressedButtons += "SELECT, ";
    if (!buttonData->START) pressedButtons += "START, ";
    if (!buttonData->UP) pressedButtons += "UP, ";
    if (!buttonData->DOWN) pressedButtons += "DOWN, ";
    if (!buttonData->LEFT) pressedButtons += "LEFT, ";
    if (!buttonData->RIGHT) pressedButtons += "RIGHT, ";
    if (!buttonData->A) pressedButtons += "A, ";
    if (!buttonData->X) pressedButtons += "X, ";
    if (!buttonData->SHOULDER_LEFT) pressedButtons += "SHOULDER_LEFT, ";
    if (!buttonData->SHOULDER_RIGHT) pressedButtons += "SHOULDER_RIGHT, ";

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
