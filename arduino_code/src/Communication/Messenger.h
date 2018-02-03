#ifndef Messenger_H
#define Messenger_H

#include "../tools/tools.h"

/*
 * Markers were chosen deliberately to have low values, since most of the transmitted data will be high values.
 * (HIGH means buttons are not pressed)
 * This will ensure a minimal encoding overhead for typical scenarios.
 */
typedef enum {
    START_MARKER = 0,
    END_MARKER = 1,
    ENCODE_MARKER = 2
} ByteMarker;

typedef void (* MessageHandler)(const uint8_t *, uint8_t);

class Messenger {
    private:
       /*
        * Due the encoding, in a worst case we may need double the content size
        * plus 2 bytes for the message type plus 2 bytes for the start / end marker.
        */
        static const uint8_t BUFFER_SIZE = MAX_CONTENT_SIZE * 2 + 2 + 2;

        uint8_t receiveBuffer[BUFFER_SIZE];
        uint8_t receiveBufferIndex;
        bool isReceiving;

        /*
         * Stores the registered message handlers.
         */
        MessageHandler messageHandlers[NUM_MESSAGE_TYPES];

        void handleMessage(MessageType, const uint8_t*, uint8_t);
        void decodeReceivedMessage(uint8_t);

    public:
        /*
         * This is meant to be called regularly when incoming data may be available.
         */
        void checkForData();

        /*
         * Registers a message handler for a specific message type.
         */
        void registerMessageHandler(MessageType type, const MessageHandler handler);

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
