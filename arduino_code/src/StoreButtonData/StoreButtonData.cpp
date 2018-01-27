#include "StoreButtonData.h"
#include <Arduino.h>
#include "../tools/tools.h"

/*
 * Wait for this many bytes before actually sending data to the client.
 * With a value of 128 about one save request is sent per second (assuming inputs come in at 50 or 60Hz).
 */
const byte BUFFER_SIZE = 128;

byte sendBuffer[BUFFER_SIZE];
byte bufferPosition = 0;

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
    buttonDataToBytes(buttonData, sendBuffer + bufferPosition);

    bufferPosition += 2;

    // buffer is full, send data
    if (bufferPosition == BUFFER_SIZE) {
        sendData();
    }
}
