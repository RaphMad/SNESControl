#include "Messenger.h"

static bool needsEncoding(const uint8_t byte);
static uint8_t encode (const uint8_t byte);
static uint8_t decode (const uint8_t byte);

void Messenger::checkForData() {
    while (Serial.available() > 0) {
        const uint8_t receivedByte = Serial.read();

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

void Messenger::decodeReceivedMessage(const uint8_t numberOfBytes) {
    uint8_t decodedBytes = 0;

    for (uint8_t i = 0; i < numberOfBytes; i++) {
        uint8_t currentByte = receiveBuffer[i];

        if (currentByte == ENCODE_MARKER) {
            i++;
            currentByte = decode(receiveBuffer[i]);
        }

        // re-use the receive buffer for the decoded data
        receiveBuffer[decodedBytes] = currentByte;
        decodedBytes++;
    }

    MessageType messageType = (MessageType)receiveBuffer[0];
    const uint8_t* payload = receiveBuffer + 1;

    handleMessage(messageType, payload, decodedBytes - 1);
}

static uint8_t decode (const uint8_t byte) {
    // coding scheme: 0 <-> 2 3 , 1 <-> 2 4, 2 <-> 2 5
    return byte - ENCODE_MARKER - 1;
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
            break;
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
            break;
    }
}

void Messenger::sendData(const MessageType type, const uint8_t* const payload, const uint8_t size) {
    if (size <= MAX_CONTENT_SIZE) {
        uint8_t sendBufferIndex = 2;
        sendBuffer[0] = START_MARKER;

        if (needsEncoding(type)) {
            sendBuffer[1] = ENCODE_MARKER;
            sendBuffer[2] = encode(type);
            sendBufferIndex++;
        } else {
            sendBuffer[1] = type;
        }

        for (uint8_t i = 0; i < size; i++) {
            const uint8_t currentByte = payload[i];

            if (needsEncoding(currentByte)) {
                sendBuffer[sendBufferIndex] = ENCODE_MARKER;

                sendBufferIndex++;
                sendBuffer[sendBufferIndex] = encode(currentByte);
            } else {
                sendBuffer[sendBufferIndex] = currentByte;
            }

            sendBufferIndex++;
        }

        sendBuffer[sendBufferIndex] = END_MARKER;
        sendBufferIndex++;

        Serial.write(sendBuffer, sendBufferIndex);
    }
}

static bool needsEncoding(const uint8_t byte) {
    return byte == START_MARKER || byte == ENCODE_MARKER || byte == END_MARKER;
}

static uint8_t encode (const uint8_t byte) {
    // coding scheme: 0 <-> 2 3 , 1 <-> 2 4, 2 <-> 2 5
    return byte + ENCODE_MARKER + 1;
}

void Messenger::sendInfo() {
    const uint8_t dataBufferSize = sizeof(AppInfo) + sizeof(uint16_t);
    uint8_t bytesToSend[dataBufferSize];

    memcpy(bytesToSend, &appInfo, sizeof(AppInfo));

    // amount of free RAM is not part of AppInfo
    intToBytes(getFreeRam(), bytesToSend + sizeof(AppInfo));

    sendData(INFO_RESPONSE, bytesToSend, dataBufferSize);
}

void Messenger::print(const String& text) {
    uint8_t bytes[text.length() + 1];
    text.getBytes(bytes, text.length() + 1);

    sendData(PRINT, bytes, text.length());
}

Messenger MessageProcessor;
