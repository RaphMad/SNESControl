#include "StoreButtonData.h"
#include <Arduino.h>
#include "../Communication/messageTypes.h"

void StoreButtonData::begin() {
    Serial.begin(9600);
}

uint8_t getFirstByte(ButtonData buttonData) {
    uint8_t byte1 = 0;

    if (buttonData.B) bitSet(byte1, 7);
    if (buttonData.Y) bitSet(byte1, 6);
    if (buttonData.SELECT) bitSet(byte1, 5);
    if (buttonData.START) bitSet(byte1, 4);
    if (buttonData.UP) bitSet(byte1, 3);
    if (buttonData.DOWN) bitSet(byte1, 2);
    if (buttonData.LEFT) bitSet(byte1, 1);
    if (buttonData.RIGHT) bitSet(byte1, 0);

    return byte1;
}

uint8_t getSecondByte(ButtonData buttonData) {
    uint8_t byte2 = 0;

    if (buttonData.A) bitSet(byte2, 7);
    if (buttonData.X) bitSet(byte2, 6);
    if (buttonData.SHOULDER_LEFT) bitSet(byte2, 5);
    if (buttonData.SHOULDER_RIGHT) bitSet(byte2, 4);

    return byte2;
}

void StoreButtonData::storeData(ButtonData buttonData) {
    Serial.write(getFirstByte(buttonData));
    Serial.write(getSecondByte(buttonData));
}
