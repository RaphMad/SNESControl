#include "LoadButtonData.h"

static void requestData();

void LoadButtonData::loadInitialData() {
    reset();
    requestData();
}

bool LoadButtonData::hasData() {
    return hasInitialData;
}

bool LoadButtonData::processIncomingData(const uint8_t* const buf, const uint8_t size) {
    // write data into inactive input buffer
    if (!isInputBuffer1Active) {
        memcpy(inputBuffer1, buf, size);
    } else {
        memcpy(inputBuffer2, buf, size);
    }

    if (!hasInitialData) {
        // initial case, set buffer1 to active and load data for buffer 2
        hasInitialData = true;
        isInputBuffer1Active = true;
        requestData();

        // also prepare the first frame
        return true;
    }

    return false;
}

ButtonData LoadButtonData::getData() {
    if (hasInitialData) {
        if (isInputBuffer1Active) {
            return readFromBuffer1();
        } else {
            return readFromBuffer2();
        }
    }

    return ButtonData();
}

const ButtonData LoadButtonData::readFromBuffer1() {
    const uint8_t* nextBytes = inputBuffer1 + inputBuffer1Index;

    inputBuffer1Index += sizeof(ButtonData);

    if (inputBuffer1Index + sizeof(ButtonData) > BUFFER_SIZE) {
        isInputBuffer1Active = false;
        inputBuffer1Index = 0;
        requestData();
    }

    return bytesToButtonData(nextBytes);
}

const ButtonData LoadButtonData::readFromBuffer2() {
    const uint8_t* nextBytes = inputBuffer2 + inputBuffer2Index;

    inputBuffer2Index += sizeof(ButtonData);

    if (inputBuffer2Index + sizeof(ButtonData) > BUFFER_SIZE) {
        isInputBuffer1Active = true;
        inputBuffer2Index = 0;
        requestData();
    }

    return bytesToButtonData(nextBytes);
}

static void requestData() {
    const uint8_t content[] = { MAX_CONTENT_SIZE - MAX_CONTENT_SIZE % sizeof(ButtonData) };
    MessageProcessor.sendMessage(LOAD, content, 1);
}


void LoadButtonData::reset() {
    inputBuffer1Index = 0;
    inputBuffer2Index = 0;
    isInputBuffer1Active = false;
    hasInitialData = false;
}

LoadButtonData ButtonDataLoader;
