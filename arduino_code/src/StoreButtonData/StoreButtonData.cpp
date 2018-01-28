#include "StoreButtonData.h"
#include <Arduino.h>
#include "../tools/tools.h"
#include "types.h"
#include "../Communication/Messenger.h"

/*
 * Wait for this many bytes before actually sending data to the client.
 * With a value of 128 about one save request is sent per second (assuming inputs come in at 50 or 60Hz).
 */
const int BUFFER_SIZE = MAX_CONTENT_SIZE;

byte sendBuffer[BUFFER_SIZE];
int bufferPosition = 0;

void sendData() {
    Messenger::sendData(SAVE, sendBuffer, BUFFER_SIZE);
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
