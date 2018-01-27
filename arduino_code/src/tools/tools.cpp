#include "tools.h"

byte* buttonDataToBytes(ButtonData buttonData) {
    ButtonData* memoryAddress = &buttonData;
    return (byte*) memoryAddress;
}

ButtonData bytesToButtonData(byte* bytes) {
    ButtonData* memoryAddress = (ButtonData*) bytes;
    return *memoryAddress;
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