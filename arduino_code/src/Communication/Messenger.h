#ifndef Messenger_H
#define Messenger_H

#include "../tools/tools.h"
#include "Encoding.h"

typedef void (* MessageHandler)(const uint8_t *, uint8_t);

class Messenger {
    private:
       Encoding encoder;

        /*
         * Stores the registered message handlers.
         */
        MessageHandler messageHandlers[NUM_MESSAGE_TYPES];

        void decodeReceivedMessage(uint8_t);
    public:
        /*
         * Registers a message handler for a specific message type.
         */
        void registerMessageHandler(MessageType type, const MessageHandler handler);

        /*
         * This is meant to be called regularly when incoming data may be available.
         */
        void pollData();

        /*
         * Send data with its payload to the client.
         * Size needs to be less than MAX_CONTENT_SIZE.
         */
        void sendData(MessageType type, const uint8_t* payload, uint8_t size);

        /*
         * Convencience function to print text to the client.
         * Only for debugging purposes, String will cause unpredictable
         * memory allocation results in production.
         */
        void print(const String& text);
};

/*
 * Responsible for sending and receiving messages.
 */
extern Messenger MessageProcessor;

#endif
