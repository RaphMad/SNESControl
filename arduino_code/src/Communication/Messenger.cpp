#include "Messenger.h"

static bool needsEncoding(const uint8_t byte);
static uint8_t encode (const uint8_t byte);

void Messenger::registerMessageHandler(MessageType type, const MessageHandler handler) {
    messageHandlers[type] = handler;
}

void Messenger::pollData() {
    const uint8_t messageSize = encoder.pollData();

    if (messageSize != NO_MESSAGE) {
        const uint8_t* payload = encoder.getMessage();
        const MessageType type = (MessageType)payload[0];

        if (messageHandlers[type]) {
            messageHandlers[type](payload + 1, messageSize - 1);
        }
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
