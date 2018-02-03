#include "StoreButtonData.h"

void StoreButtonData::storeData(const ButtonData buttonData) {
    buttonDataToBytes(&buttonData, outputBuffer + outputBufferIndex);

    outputBufferIndex += sizeof(ButtonData);

    // buffer is full, send data
    if (outputBufferIndex == BUFFER_SIZE) {
        MessageProcessor.sendMessage(SAVE, outputBuffer, BUFFER_SIZE);
        outputBufferIndex = 0;
    }
}

void StoreButtonData::reset() {
    outputBufferIndex = 0;
}

/*
 * Stores button data.
 */
StoreButtonData ButtonDataStorage;
