#include "Messenger.h"
#include "../tools/tools.h"
#include "../LoadButtonData/LoadButtonData.h"
#include "../WriteToConsole/WriteToConsole.h"
#include "../StoreButtonData/StoreButtonData.h"

/*
 * Markers were chosen deliberately to have low values, since most of the transmitted data will be high values.
 * (HIGH means buttons are not pressed)
 * This will ensure a minimal encoding overhead for typical scenarios.
 */
const byte START_MARKER = 0;
const byte END_MARKER = 1;
const byte ENCODE_MARKER = 2;

/*
 * Due the encoding, in a worst case we may need double the content size
 * plus 2 bytes for the message type plus 2 bytes for the start / end marker.
 */
const int RECEIVE_BUFFER_SIZE = MAX_CONTENT_SIZE * 2 + 2 + 2;
byte receivedBytes[RECEIVE_BUFFER_SIZE];

void Messenger::sendData(MessageType type, byte* payload, int size) {
    byte bytesToSend[size * 2 + 2 + 2];

    bytesToSend[0] = START_MARKER;
    bytesToSend[1] = type; // needs encoding should we ever introduce types 0,1,2
    int sendIndex = 2;

    for (int i = 0; i < size; i++) {
        byte currentByte = payload[i];

        // coding scheme: 0 <-> 2 3 , 1 <-> 2 4, 2 <-> 2 5
        if (currentByte == START_MARKER) {
            bytesToSend[sendIndex] = ENCODE_MARKER;
            sendIndex++;
            bytesToSend[sendIndex] = currentByte + ENCODE_MARKER + 1;
        } else if (currentByte == ENCODE_MARKER) {
            bytesToSend[sendIndex] = ENCODE_MARKER;
            sendIndex++;
            bytesToSend[sendIndex] = currentByte + ENCODE_MARKER + 1;
        } else if (currentByte == END_MARKER) {
            bytesToSend[sendIndex] = ENCODE_MARKER;
            sendIndex++;
            bytesToSend[sendIndex] = currentByte + ENCODE_MARKER + 1;
        } else {
            bytesToSend[sendIndex] = currentByte;
        }

        sendIndex++;
    }

    bytesToSend[sendIndex] = END_MARKER;

    Serial.write(bytesToSend, sendIndex + 1);
}

void Messenger::print(String text) {
    byte bytes[text.length() + 1];
    text.getBytes(bytes, text.length() + 1);

    sendData(PRINT, bytes, text.length());
}

AppInfo* appInfoPointer;

void Messenger::setAppInfo(AppInfo* value) {
    appInfoPointer = value;
}

int getFreeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

void sendInfo() {
    byte bytesToSend[20];

    memcpy(bytesToSend, &appInfoPointer->maxLoopDuration, 2);
    memcpy(bytesToSend + 2, &appInfoPointer->lastLatchDuration, 2);
    memcpy(bytesToSend + 4, &appInfoPointer->firstLatch, 2);
    memcpy(bytesToSend + 6, &appInfoPointer->numberOfLatches, 2);
    memcpy(bytesToSend + 8, &appInfoPointer->shortLatches, 2);
    memcpy(bytesToSend + 10, &appInfoPointer->longLatches, 2);
    memcpy(bytesToSend + 12, &appInfoPointer->isInSaveMode, 1);
    memcpy(bytesToSend + 13, &appInfoPointer->isInReplayMode, 1);
    memcpy(bytesToSend + 14, &appInfoPointer->delayCount, 2);
    memcpy(bytesToSend + 16, &appInfoPointer->skipCount, 2);

    int freeRam = getFreeRam();
    bytesToSend[19] = (freeRam >> 8) & 0xFF;
    bytesToSend[18] = freeRam & 0xFF;

    Messenger::sendData(INFO_RESPONSE, bytesToSend, 20);
}

void decodeReceivedMessage(int numberOfBytes) {
    int decodedBytes = 0;

    for (int i = 0; i < numberOfBytes; i++) {
        byte currentByte = receivedBytes[i];

        if (currentByte == ENCODE_MARKER) {
            // coding scheme: 0 <-> 2 3 , 1 <-> 2 4, 2 <-> 2 5
            i++;
            byte nextByte = receivedBytes[i];
            currentByte = nextByte - ENCODE_MARKER - 1;
        }

        // re-use the receive buffer for the decoded data
        receivedBytes[decodedBytes] = currentByte;
        decodedBytes++;
    }

    MessageType messageType = (MessageType)receivedBytes[0];
    byte* payload = receivedBytes + 1;

    switch(messageType) {
        case ENABLE_SAVE:
            appInfoPointer->firstLatch = 0;
            StoreButtonData::reset();
            appInfoPointer->isInSaveMode = true;
            break;
        case SAVE:
            break;
        case ENABLE_LOAD:
            appInfoPointer->firstLatch = 0;
            appInfoPointer->isInReplayMode = true;
            LoadButtonData::begin();
            break;
        case LOAD:
            break;
        case PING:
            Messenger::sendData(PONG, payload, decodedBytes - 1);
            break;
        case PONG:
            break;
        case PRINT:
            break;
        case REQUEST_STATUS:
            sendInfo();
            break;
        case DISABLE_SAVE:
            appInfoPointer->isInSaveMode = false;
            StoreButtonData::reset();
            break;
        case DISABLE_LOAD:
            appInfoPointer->isInReplayMode = false;

            // reset all ongoing replay actions
            LoadButtonData::reset();
            WriteToConsole::prepareData(ButtonData());
        case RESET_DATA:
            appInfoPointer->isInReplayMode = false;
            appInfoPointer->isInSaveMode = false;
            appInfoPointer->delayCount = 0;
            appInfoPointer->firstLatch = 0;
            appInfoPointer->lastLatchDuration = 0;
            appInfoPointer->longLatches = 0;
            appInfoPointer->maxLoopDuration = 0;
            appInfoPointer->numberOfLatches = 0;
            appInfoPointer->shortLatches = 0;
            appInfoPointer->skipCount = 0;

            // reset all ongoing replay actions
            LoadButtonData::reset();
            StoreButtonData::reset();
            WriteToConsole::prepareData(ButtonData());
            break;
        case LOAD_RESPONSE:
             LoadButtonData::processIncomingData(payload, decodedBytes - 1);
            break;
        case INFO_RESPONSE:
             LoadButtonData::processIncomingData(payload, decodedBytes - 1);
            break;
    }
}

int numberOfBytes;
bool isReceiving;

void Messenger::checkForData() {
    while (Serial.available() > 0) {
        byte receivedByte = Serial.read();

        if (receivedByte == START_MARKER) {
            numberOfBytes = 0;
            isReceiving = true;
        }

        if (isReceiving && receivedByte != START_MARKER && receivedByte != END_MARKER) {
            receivedBytes[numberOfBytes] = receivedByte;
            numberOfBytes++;
        }

        if (receivedByte == END_MARKER) {
            isReceiving = false;
            decodeReceivedMessage(numberOfBytes);
        }
    }
}
