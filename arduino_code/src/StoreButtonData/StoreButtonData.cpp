#include "StoreButtonData.h"
#include <Arduino.h>
#include <stdint.h>
#include "../Communication/messageTypes.h"

const uint8_t TEMP_STORE_SIZE = 50;

void StoreButtonData::begin() {
    Serial.begin(9600);
}

/*
 * Data is temporarily stored in this array and automatically shifted to the PC when its full.
 * Note that the last, eventually incomplete data portion needs to be flushed!
 */
ButtonData tempButtonData[TEMP_STORE_SIZE];
uint8_t currentPosition = 0;

void sendButtonData(ButtonData buttonData) {
    // interpret struct as array of bytes
    Serial.write((uint8_t*)&buttonData, 2);
}

void pushData() {
    Serial.write(currentPosition);

    for (int i = 0; i < currentPosition; i++) {
        sendButtonData(tempButtonData[i]);
    }

    currentPosition = 0;
}

void StoreButtonData::storeData(ButtonData buttonData) {
    tempButtonData[currentPosition] = buttonData;
    currentPosition++;

    if (currentPosition == TEMP_STORE_SIZE) {
        Serial.write(SaveButtonData);
        pushData();
    }
}

void StoreButtonData::flush() {
    Serial.write(EndButtonData);
    pushData();
}