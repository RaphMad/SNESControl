#include "Encoding.h"

static uint8_t decode (const uint8_t byte);

static bool needsEncoding(const uint8_t byte);
static uint8_t encode (const uint8_t byte);

uint8_t Encoding::pollForData() {
    uint8_t messageSize = NO_MESSAGE;

    while (Serial.available() > 0) {
        const uint8_t receivedByte = Serial.read();

        if (receivedByte == START_MARKER) {
            isReceiving = true;
            receiveBufferIndex = 0;
        } else if (receivedByte == END_MARKER) {
            isReceiving = false;
            messageSize = receiveBufferIndex;
        } else if (isReceiving) {
            processNextByte(receivedByte);
        }

    }

    return messageSize;
}

void Encoding::processNextByte(const uint8_t byte) {
    if (byte == ENCODE_MARKER) {
        isDecoding = true;
    } else {
        if (isDecoding) {
            receiveBuffer[receiveBufferIndex] = decode(byte);
            isDecoding = false;
        } else {
            receiveBuffer[receiveBufferIndex] = byte;
        }

        receiveBufferIndex++;
    }
}

const uint8_t* Encoding::readMessage() {
    return receiveBuffer;
}

static uint8_t decode (const uint8_t byte) {
    // coding scheme: 0 <-> 2 3 , 1 <-> 2 4, 2 <-> 2 5
    return byte - ENCODE_MARKER - 1;
}

void Encoding::sendMessage(const MessageType type, const uint8_t* const payload, const uint8_t size) {
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
