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

void Messenger::registerMessageHandler(MessageType type, const MessageHandler handler) {
    messageHandlers[type] = handler;
}

void Messenger::handleMessage(const MessageType messageType, const uint8_t* const payload, const uint8_t size) {
    if (messageHandlers[messageType]) {
        messageHandlers[messageType](payload, size);
    }
}

void Messenger::sendData(const MessageType type, const uint8_t* const payload, const uint8_t size) {
    Serial.write(START_MARKER);
    Serial.write(type);

    for (int i = 0; i < size; i++) {
        uint8_t currentByte = payload[i];

        if (needsEncoding(currentByte)) {
            Serial.write(ENCODE_MARKER);
            Serial.write(encode(currentByte));
        } else {
            Serial.write(currentByte);
        }
    }

    Serial.write(END_MARKER);
}

static bool needsEncoding(const uint8_t byte) {
    return byte == START_MARKER || byte == ENCODE_MARKER || byte == END_MARKER;
}

static uint8_t encode (const uint8_t byte) {
    // coding scheme: 0 <-> 2 3 , 1 <-> 2 4, 2 <-> 2 5
    return byte + ENCODE_MARKER + 1;
}

void Messenger::print(const String& text) {
    uint8_t bytes[text.length() + 1];
    text.getBytes(bytes, text.length() + 1);

    sendData(PRINT, bytes, text.length());
}

Messenger MessageProcessor;
