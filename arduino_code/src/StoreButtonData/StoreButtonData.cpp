#include <Arduino.h>
#include "StoreButtonData.h"
#include "../tools/tools.h"
#include "../Communication/Messenger.h"

/*
 * Wait for this many bytes before actually sending data to the client.
 * With a value of 64 about two save requests are sent per second (assuming inputs come in at 50 or 60Hz).
 */
const int OUTPUT_BUFFER_SIZE = MAX_CONTENT_SIZE;

byte outputBuffer[OUTPUT_BUFFER_SIZE];
int outputBufferIndex= 0;

void StoreButtonData::reset() {
    outputBufferIndex = 0;
}

void sendData() {
    Messenger::sendData(SAVE, outputBuffer, OUTPUT_BUFFER_SIZE);
    outputBufferIndex = 0;
}

void StoreButtonData::storeData(ButtonData buttonData) {
    buttonData.pressedAt = millis() - appInfo.firstLatch;
    buttonDataToBytes(&buttonData, outputBuffer + outputBufferIndex);

    outputBufferIndex += 4;

    // buffer is full, send data
    if (outputBufferIndex == OUTPUT_BUFFER_SIZE) {
        sendData();
    }
}
