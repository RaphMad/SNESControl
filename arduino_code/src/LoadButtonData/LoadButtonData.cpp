#include <Arduino.h>
#include "LoadButtonData.h"

const byte BUFFER_SIZE = 64;

byte inputBuffer1[BUFFER_SIZE];
byte buffer1Index = 0;

byte inputBuffer2[BUFFER_SIZE];
byte buffer2Index = 0;

bool isInputBuffer1Active = false;
bool isInputBuffer2Active = false;

void requestBytes() {
    Serial.println("LOAD|" + BUFFER_SIZE);
    Serial.print("Loaded data! 1 active: " + isInputBuffer1Active);
    Serial.println(", 2 active: " + isInputBuffer1Active);
}

void LoadButtonData::begin() {
    requestBytes();
}

bool LoadButtonData::handleIncomingData() {
    if (!isInputBuffer1Active) {
        Serial.readBytes(inputBuffer1, BUFFER_SIZE);
    }

    if (!isInputBuffer2Active) {
        Serial.readBytes(inputBuffer2, BUFFER_SIZE);
    }

    bool isInitial = !isInputBuffer1Active && !isInputBuffer2Active;

    if (isInitial) {
        // initial case, set buffer1 to active and load data for buffer 2
        isInputBuffer1Active = true;
        requestBytes();
    }

    return isInitial;
}

ButtonData LoadButtonData::getData() {
    ButtonData buttonData;
    byte byte1;
    byte byte2;

    if (isInputBuffer1Active) {
        byte1 = inputBuffer1[buffer1Index];
        byte2 = inputBuffer1[buffer1Index + 1];

        buffer1Index += 2;

        if (buffer1Index == BUFFER_SIZE) {
            isInputBuffer1Active = false;
            isInputBuffer2Active = true;
            requestBytes();
            buffer1Index = 0;
        }
    }

    if (isInputBuffer2Active) {
        byte1 = inputBuffer2[buffer2Index];
        byte2 = inputBuffer2[buffer2Index + 1];

        buffer2Index += 2;

        if (buffer1Index == BUFFER_SIZE) {
            isInputBuffer1Active = false;
            isInputBuffer2Active = true;
            requestBytes();
            buffer2Index = 0;
        }
    }

    buttonData.B = bitRead(byte1, 7);
    buttonData.Y = bitRead(byte1, 6);
    buttonData.SELECT = bitRead(byte1, 5);
    buttonData.START = bitRead(byte1, 4);
    buttonData.UP = bitRead(byte1, 3);
    buttonData.DOWN = bitRead(byte1, 2);
    buttonData.LEFT = bitRead(byte1, 1);
    buttonData.RIGHT = bitRead(byte1, 0);
    buttonData.A = bitRead(byte2, 7);
    buttonData.X = bitRead(byte2, 6);
    buttonData.SHOULDER_LEFT = bitRead(byte2, 5);
    buttonData.SHOULDER_RIGHT = bitRead(byte2, 4);

    return buttonData;
}