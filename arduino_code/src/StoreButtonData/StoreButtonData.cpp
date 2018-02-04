#include "StoreButtonData.h"

void StoreButtonData::storeData(const ButtonData buttonData) {
    buttonDataToBytes(&buttonData, outputBuffer + outputBufferIndex);

    outputBufferIndex += sizeof(ButtonData);

    // buffer is full, send data
    if (outputBufferIndex + sizeof(ButtonData) > BUFFER_SIZE) {
        MessageProcessor.sendMessage(SAVE, outputBuffer, outputBufferIndex);
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
