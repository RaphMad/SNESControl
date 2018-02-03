#include "Encoding.h"

static uint8_t decode (const uint8_t byte);

uint8_t Encoding::pollData() {
    uint8_t messageSize = NO_MESSAGE;

    while (Serial.available() > 0) {
        const uint8_t receivedByte = Serial.read();

        if (receivedByte == START_MARKER) {
            receiveBufferIndex = 0;
            isReceiving = true;
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

uint8_t* Encoding::getMessage() {
    return receiveBuffer;
}

static uint8_t decode (const uint8_t byte) {
    // coding scheme: 0 <-> 2 3 , 1 <-> 2 4, 2 <-> 2 5
    return byte - ENCODE_MARKER - 1;
}
