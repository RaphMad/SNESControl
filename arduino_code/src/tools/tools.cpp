#include "tools.h"

void intToBytes(int value, byte* buf) {
    buf[0] = value & 0xFF;
    buf[1] = (value >> 8) & 0xFF;
}

void appInfoToBytes(AppInfo* appInfo, byte* buf) {
    intToBytes(appInfo->maxLoopDuration, buf);
    intToBytes(appInfo->lastLatchDuration, buf + 2);
    intToBytes(appInfo->firstLatch, buf + 4);
    intToBytes(appInfo->numberOfLatches, buf + 6);
    intToBytes(appInfo->shortLatches, buf + 8);
    intToBytes(appInfo->shortLatches, buf + 10);

    buf[12] = appInfo->isInSaveMode;
    buf[13] = appInfo->isInReplayMode;

    intToBytes(appInfo->delayCount, buf + 14);
    intToBytes(appInfo->skipCount, buf + 16);
}

void buttonDataToBytes(ButtonData* buttonData, byte* buf) {
    buf[0] = 0xFF;
    buf[0] = bitClear(buttonData->B, 7);
    buf[0] = bitClear(buttonData->Y, 6);
    buf[0] = bitClear(buttonData->SELECT, 5);
    buf[0] = bitClear(buttonData->START, 4);
    buf[0] = bitClear(buttonData->UP, 3);
    buf[0] = bitClear(buttonData->DOWN, 2);
    buf[0] = bitClear(buttonData->LEFT, 1);
    buf[0] = bitClear(buttonData->RIGHT, 0);

    buf[1] = 0xFF;
    buf[1] = bitClear(buttonData->A, 7);
    buf[1] = bitClear(buttonData->X, 6);
    buf[1] = bitClear(buttonData->SHOULDER_LEFT, 5);
    buf[1] = bitClear(buttonData->SHOULDER_RIGHT, 4);

    intToBytes(buttonData->pressedAt, buf + 2);
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

int getFreeRam() {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
