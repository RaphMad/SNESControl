#include "StoreButtonData.h"
#include <Arduino.h>

const byte BUFFER_SIZE = 64;
byte sendBuffer[BUFFER_SIZE];

byte bufferPosition = 0;

byte getFirstByte(ButtonData buttonData) {
    byte byte1 = 0;

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

byte getSecondByte(ButtonData buttonData) {
    byte byte2 = 0;

    if (buttonData.A) bitSet(byte2, 7);
    if (buttonData.X) bitSet(byte2, 6);
    if (buttonData.SHOULDER_LEFT) bitSet(byte2, 5);
    if (buttonData.SHOULDER_RIGHT) bitSet(byte2, 4);

    return byte2;
}

void sendData() {
    Serial.print("SAVE|");

    for (int i = 0; i < BUFFER_SIZE; i+=2) {
        Serial.print(sendBuffer[i]);
        Serial.print(" ");
        Serial.print(sendBuffer[i+1]);
        Serial.print(" ");
    }

    Serial.println();
    bufferPosition = 0;
}

void StoreButtonData::storeData(ButtonData buttonData) {
    sendBuffer[bufferPosition] = getFirstByte(buttonData);
    sendBuffer[bufferPosition + 1] = getSecondByte(buttonData);

    bufferPosition += 2;

    if (bufferPosition == BUFFER_SIZE) {
        sendData();
    }
}
