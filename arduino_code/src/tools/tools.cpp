#include "tools.h"

void buttonDataToBytes(ButtonData buttonData, byte* buf) {
    ButtonData* memoryAddress = &buttonData;
    byte* copyFrom = (byte*) memoryAddress;

    memcpy(buf, copyFrom, 2);
    buf[2] = buttonData.pressedAt % 256;
    buf[3] = buttonData.pressedAt / 256;
}

ButtonData bytesToButtonData(byte* bytes) {
    ButtonData buttonData;

    byte byte1 = bytes[0];
    byte byte2 = bytes[1];
    byte byte3 = bytes[2];
    byte byte4 = bytes[3];

    // note that buttons are stored in reverse order
    buttonData.B = bitRead(byte1, 7);
    buttonData.Y = bitRead(byte1, 6);
    buttonData.SELECT = bitRead(byte1, 5);
    buttonData.START = bitRead(byte1, 4);
    buttonData.UP = bitRead(byte1, 3);
    buttonData.DOWN = bitRead(byte1, 2);
    buttonData.LEFT = bitRead(byte1, 1);
    buttonData.RIGHT = bitRead(byte1, 0);

    buttonData.A = bitRead(byte2, 7);
    buttonData.X = bitRead(byte2, 6);
    buttonData.SHOULDER_LEFT = bitRead(byte2, 5);
    buttonData.SHOULDER_RIGHT = bitRead(byte2, 4);

    buttonData.pressedAt = byte3 + byte4 * 256;

    return buttonData;
}

String formatButtonData(ButtonData buttonData) {
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
