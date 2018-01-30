#include "Messenger.h"
#include "../tools/tools.h"
#include "../LoadButtonData/LoadButtonData.h"
#include "../StoreButtonData/StoreButtonData.h"
#include "../WriteToConsole/WriteToConsole.h"

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
const int BUFFER_SIZE = MAX_CONTENT_SIZE * 2 + 2 + 2;

byte receiveBuffer[BUFFER_SIZE];
int receiveBufferIndex;
bool isReceiving;

/*
 * This could be allocated on the stack for each sent message,
 * but declaring it globally makes it show up in the program data size
 * and avoids allocating a lot of stack memory each time a message is sent.
 */
byte sendBuffer[BUFFER_SIZE];

void Messenger::sendData(MessageType type, byte* payload, int size) {
    if (size <= MAX_CONTENT_SIZE) {
        sendBuffer[0] = START_MARKER;
        sendBuffer[1] = type; // needs encoding should we ever introduce types 0,1,2
        int sendIndex = 2;

        for (int i = 0; i < size; i++) {
            byte currentByte = payload[i];

            // coding scheme: 0 <-> 2 3 , 1 <-> 2 4, 2 <-> 2 5
            if (currentByte == START_MARKER) {
                sendBuffer[sendIndex] = ENCODE_MARKER;
                sendIndex++;
                sendBuffer[sendIndex] = currentByte + ENCODE_MARKER + 1;
            } else if (currentByte == ENCODE_MARKER) {
                sendBuffer[sendIndex] = ENCODE_MARKER;
                sendIndex++;
                sendBuffer[sendIndex] = currentByte + ENCODE_MARKER + 1;
            } else if (currentByte == END_MARKER) {
                sendBuffer[sendIndex] = ENCODE_MARKER;
                sendIndex++;
                sendBuffer[sendIndex] = currentByte + ENCODE_MARKER + 1;
            } else {
                sendBuffer[sendIndex] = currentByte;
            }

            sendIndex++;
        }

        sendBuffer[sendIndex] = END_MARKER;
        sendIndex++;

        Serial.write(sendBuffer, sendIndex);
    }
}

void Messenger::print(String text) {
    byte bytes[text.length() + 1];
    text.getBytes(bytes, text.length() + 1);

    sendData(PRINT, bytes, text.length());
}

void sendInfo() {
    uint8_t dataBufferSize = sizeof(AppInfo) + sizeof(uint16_t);
    uint8_t bytesToSend[dataBufferSize];

    memcpy(bytesToSend, &appInfo, sizeof(AppInfo));

    // amount of free RAM is not part of AppInfo
    intToBytes(getFreeRam(), bytesToSend + sizeof(AppInfo));

    Messenger::sendData(INFO_RESPONSE, bytesToSend, dataBufferSize);
}

void handleMessage(MessageType messageType, byte* payload, int size) {
    switch(messageType) {
        case ENABLE_SAVE:
            appInfo.firstLatch = 0;
            StoreButtonData::reset();
            appInfo.isInSaveMode = true;
            break;
        case SAVE:
            break;
        case ENABLE_LOAD:
            appInfo.firstLatch = 0;
            appInfo.isInReplayMode = true;
            LoadButtonData::begin();
            break;
        case LOAD:
            break;
        case PING:
            Messenger::sendData(PONG, payload, size);
            break;
        case PONG:
            break;
        case PRINT:
            break;
        case REQUEST_STATUS:
            sendInfo();
            break;
        case DISABLE_SAVE:
            appInfo.isInSaveMode = false;
            StoreButtonData::reset();
            break;
        case DISABLE_LOAD:
            appInfo.isInReplayMode = false;

            // reset all ongoing replay actions
            LoadButtonData::reset();
            WriteToConsole::prepareData(ButtonData());
        case RESET_DATA:
            appInfo.isInReplayMode = false;
            appInfo.isInSaveMode = false;
            appInfo.delayCount = 0;
            appInfo.firstLatch = 0;
            appInfo.lastLatchDuration = 0;
            appInfo.longLatches = 0;
            appInfo.maxLoopDuration = 0;
            appInfo.shortLatches = 0;
            appInfo.skipCount = 0;

            // reset all ongoing replay actions
            LoadButtonData::reset();
            StoreButtonData::reset();
            WriteToConsole::prepareData(ButtonData());
            break;
        case LOAD_RESPONSE:
             LoadButtonData::processIncomingData(payload, size);
            break;
        case INFO_RESPONSE:
             LoadButtonData::processIncomingData(payload, size);
            break;
    }
}

void decodeReceivedMessage(int numberOfBytes) {
    int decodedBytes = 0;

    for (int i = 0; i < numberOfBytes; i++) {
        byte currentByte = receiveBuffer[i];

        if (currentByte == ENCODE_MARKER) {
            // coding scheme: 0 <-> 2 3 , 1 <-> 2 4, 2 <-> 2 5
            i++;
            byte nextByte = receiveBuffer[i];
            currentByte = nextByte - ENCODE_MARKER - 1;
        }

        // re-use the receive buffer for the decoded data
        receiveBuffer[decodedBytes] = currentByte;
        decodedBytes++;
    }

    MessageType messageType = (MessageType)receiveBuffer[0];
    byte* payload = receiveBuffer + 1;

    handleMessage(messageType, payload, decodedBytes - 1);
}

void Messenger::checkForData() {
    while (Serial.available() > 0) {
        byte receivedByte = Serial.read();

        if (receivedByte == START_MARKER) {
            receiveBufferIndex = 0;
            isReceiving = true;
        }

        if (isReceiving && receivedByte != START_MARKER && receivedByte != END_MARKER) {
            receiveBuffer[receiveBufferIndex] = receivedByte;
            receiveBufferIndex++;
        }

        if (receivedByte == END_MARKER) {
            isReceiving = false;
            decodeReceivedMessage(receiveBufferIndex);
        }
    }
}
