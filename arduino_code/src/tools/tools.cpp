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
