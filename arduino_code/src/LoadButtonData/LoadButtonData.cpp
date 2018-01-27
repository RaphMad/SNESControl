#include <Arduino.h>
#include "LoadButtonData.h"
#include "../tools/tools.h"
#include "types.h"

/*
 * Use two alternating input buffers to ensure a consistent flow of inputs to the console.
 *
 * The active input buffer is assumed to already contain data (and is being read from).
 * Meanwhile data for the inactive buffer is being fetched from the client.
 *
 * With a value of 128 about one save request is sent per second (assuming inputs come in at 50 or 60Hz).
 */
const byte BUFFER_SIZE = MAX_CONTENT_SIZE;

byte inputBuffer1[BUFFER_SIZE];
byte inputBuffer1Index = 0;

byte inputBuffer2[BUFFER_SIZE];
byte inputBuffer2Index = 0;

bool isInputBuffer1Active = false;
bool hasFirstData = false;

void requestData() {
    Serial.println("LOAD|" + BUFFER_SIZE);
}

void LoadButtonData::begin() {
    // request initial data for inputBuffer1 immediately
    requestData();
}

bool LoadButtonData::processIncomingData() {
    // write data into inactive input buffer
    if (!isInputBuffer1Active) {
        inputBuffer1[0] = 0;
    } else {
        inputBuffer2[0] = 0;
    }

    bool wasInitial = !hasFirstData;

    if (!hasFirstData) {
        // initial case, set buffer1 to active and load data for buffer 2
        hasFirstData = true;
        isInputBuffer1Active = true;
        requestData();
    }

    return wasInitial;
}

ButtonData readFromBuffer1() {
    byte* nextBytes = inputBuffer1 + inputBuffer1Index;

    inputBuffer1Index += 2;

    if (inputBuffer1Index == BUFFER_SIZE) {
        isInputBuffer1Active = false;
        requestData();
    }

    return bytesToButtonData(nextBytes);
}

ButtonData readFromBuffer2() {
    byte* nextBytes = inputBuffer2 + inputBuffer2Index;

    inputBuffer2Index += 2;

    if (inputBuffer2Index == BUFFER_SIZE) {
        isInputBuffer1Active = true;
        requestData();
    }

    return bytesToButtonData(nextBytes);
}

ButtonData LoadButtonData::getData() {
    ButtonData buttonData;

    if (isInputBuffer1Active) {
        buttonData = readFromBuffer1();
    } else {
        buttonData = readFromBuffer2();
    }

    return buttonData;
}
