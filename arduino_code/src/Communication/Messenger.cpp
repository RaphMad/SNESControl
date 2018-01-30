#include "Messenger.h"

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

void Messenger::decodeReceivedMessage(uint8_t numberOfBytes) {
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

void Messenger::handleMessage(const MessageType messageType, const uint8_t* const payload, const uint8_t size) {
    switch(messageType) {
        case ENABLE_SAVE:
            appInfo.firstLatch = 0;
            appInfo.isInSaveMode = true;
            ButtonDataStorage.reset();
            break;
        case SAVE:
            break;
        case ENABLE_LOAD:
            appInfo.firstLatch = 0;
            appInfo.isInReplayMode = true;
            ButtonDataLoader.begin();
            break;
        case LOAD:
            break;
        case PING:
            sendData(PONG, payload, size);
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
            ButtonDataStorage.reset();
            break;
        case DISABLE_LOAD:
            appInfo.isInReplayMode = false;
            ButtonDataLoader.reset();
            ConsoleWriter.prepareData(ButtonData());
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
            ButtonDataLoader.reset();
            ButtonDataStorage.reset();
            ConsoleWriter.prepareData(ButtonData());
            break;
        case LOAD_RESPONSE:
             ButtonDataLoader.processIncomingData(payload, size);
            break;
        case INFO_RESPONSE:
             ButtonDataLoader.processIncomingData(payload, size);
            break;
    }
}

void Messenger::sendData(const MessageType type, const uint8_t* const payload, const uint8_t size) {
    if (size <= MAX_CONTENT_SIZE) {
        sendBuffer[0] = START_MARKER;
        sendBuffer[1] = type; // needs encoding should we ever introduce types 0,1,2
        uint8_t sendIndex = 2;

        for (uint8_t i = 0; i < size; i++) {
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

void Messenger::sendInfo() {
    uint8_t dataBufferSize = sizeof(AppInfo) + sizeof(uint16_t);
    uint8_t bytesToSend[dataBufferSize];

    memcpy(bytesToSend, &appInfo, sizeof(AppInfo));

    // amount of free RAM is not part of AppInfo
    intToBytes(getFreeRam(), bytesToSend + sizeof(AppInfo));

    sendData(INFO_RESPONSE, bytesToSend, dataBufferSize);
}

void Messenger::print(const String& text) {
    byte bytes[text.length() + 1];
    text.getBytes(bytes, text.length() + 1);

    sendData(PRINT, bytes, text.length());
}

Messenger MessageProcessor;
