#include "Messenger.h"

void Messenger::registerMessageHandler(MessageType type, const MessageHandler handler) {
    messageHandlers[type] = handler;
}

void Messenger::pollForMessage() {
    const uint8_t messageSize = encoder.pollForData();

    if (messageSize != NO_MESSAGE) {
        const uint8_t* message = encoder.readMessage();
        const MessageType type = (MessageType)message[0];

        if (messageHandlers[type]) {
            messageHandlers[type](message + 1, messageSize - 1);
        }
    }
}

void Messenger::sendMessage(const MessageType type, const uint8_t* const payload, const uint8_t size) {
    encoder.sendMessage(type, payload, size);
}

void Messenger::print(const String& text) {
    uint8_t bytes[text.length() + 1];
    text.getBytes(bytes, text.length() + 1);

    sendMessage(PRINT, bytes, text.length());
}

Messenger MessageProcessor;
