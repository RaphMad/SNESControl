#include "tools.h"
#include "../Communication/Messenger.h"

void buttonDataToBytes(const ButtonData* const buttonData, uint8_t* const buf) {
    memcpy(buf, buttonData, sizeof(ButtonData));
}

ButtonData bytesToButtonData(const uint8_t* const bytes) {
    ButtonData buttonData;

    buttonData.B = bitRead(bytes[0], 0);
    buttonData.Y = bitRead(bytes[0], 1);
    buttonData.SELECT = bitRead(bytes[0], 2);
    buttonData.START = bitRead(bytes[0], 3);
    buttonData.UP = bitRead(bytes[0], 4);
    buttonData.DOWN = bitRead(bytes[0], 5);
    buttonData.LEFT = bitRead(bytes[0], 6);
    buttonData.RIGHT = bitRead(bytes[0], 7);

    buttonData.A = bitRead(bytes[1], 0);
    buttonData.X = bitRead(bytes[1], 1);
    buttonData.SHOULDER_LEFT = bitRead(bytes[1], 2);
    buttonData.SHOULDER_RIGHT = bitRead(bytes[1], 3);

    buttonData.latchNumber = bytesToLong(bytes + 2);

    return buttonData;
}

void appInfoToBytes(const AppInfo* appInfo, uint8_t* buf) {
    memcpy(buf, appInfo, sizeof(AppInfo));
}

void intToBytes(const uint16_t value, uint8_t* const buf) {
    memcpy(buf, &value, sizeof(uint16_t));
}

uint16_t bytesToInt(const uint8_t* const bytes) {
    return (bytes[1] << 8) + bytes[0];
}

uint32_t bytesToLong(const uint8_t* const bytes) {
    return (uint32_t)bytes[3] * 256 * 256 * 256 + (uint32_t)bytes[2] * 256 * 256 + (bytes[1] << 8) + bytes[0];
}

const String formatButtonData(const ButtonData& buttonData) {
    String pressedButtons = "";
    pressedButtons.reserve(81);

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
