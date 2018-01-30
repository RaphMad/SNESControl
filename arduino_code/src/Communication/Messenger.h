#ifndef Messenger_H
#define Messenger_H

#include "../tools/tools.h"
#include "../LoadButtonData/LoadButtonData.h"
#include "../StoreButtonData/StoreButtonData.h"
#include "../WriteToConsole/WriteToConsole.h"

class Messenger {
    private:
       /*
        * Markers were chosen deliberately to have low values, since most of the transmitted data will be high values.
        * (HIGH means buttons are not pressed)
        * This will ensure a minimal encoding overhead for typical scenarios.
        */
        const uint8_t START_MARKER = 0;
        const uint8_t END_MARKER = 1;
        const uint8_t ENCODE_MARKER = 2;

       /*
        * Due the encoding, in a worst case we may need double the content size
        * plus 2 bytes for the message type plus 2 bytes for the start / end marker.
        */
        static const uint8_t BUFFER_SIZE = MAX_CONTENT_SIZE * 2 + 2 + 2;

        uint8_t receiveBuffer[BUFFER_SIZE];
        uint8_t receiveBufferIndex;
        bool isReceiving;

       /*
        * This could be allocated on the stack for each sent message,
        * but declaring it here makes it show up in the program data size
        * and avoids allocating a lot of stack memory each time a message is sent.
        */
        uint8_t sendBuffer[BUFFER_SIZE];

        void handleMessage(MessageType, const uint8_t*, uint8_t);
        void decodeReceivedMessage(uint8_t);
        void sendInfo();

    public:
        /*
         * This is meant to be called regularly when incoming data may be available.
         */
        void checkForData();

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
