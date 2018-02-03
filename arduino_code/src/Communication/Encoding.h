#ifndef Encoding_h
#define Encoding_h

#include "common.h"

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

const uint8_t NO_MESSAGE = 0;

class Encoding {
    private:
       /*
        * Due the encoding, in a worst case we may need double the content size plus 2 bytes for the message type.
        */
        static const uint8_t BUFFER_SIZE = MAX_CONTENT_SIZE + 2;

        uint8_t receiveBuffer[BUFFER_SIZE];
        uint8_t receiveBufferIndex;

        bool isReceiving;
        bool isDecoding;

        void processNextByte(const uint8_t byte);
    public:
        /*
         * This is meant to be called regularly when incoming data may be available.
         *
         * The return value indicates whether a complete frame is available.
         *
         * 0 indicates that no message is available. (All messages have at least size 1, the message type.)
         * A positive value indicates that a message of the given size is available.
         */
        uint8_t pollData();

        uint8_t* getMessage();
};

#endif
