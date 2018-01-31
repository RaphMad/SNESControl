#ifndef StoreButtonData_h
#define StoreButtonData_h

#include <Arduino.h>
#include "common.h"
#include "../Communication/Messenger.h"

class StoreButtonData {
    private:
        /*
        * Wait for this many bytes before actually sending data to the client.
        * With a value of 64 about two save requests are sent per second (assuming inputs come in at 50 or 60Hz).
        */
        static const uint8_t BUFFER_SIZE = MAX_CONTENT_SIZE;

        uint8_t outputBuffer[BUFFER_SIZE];
        uint8_t outputBufferIndex= 0;

    public:

        /*
         * Stores the passed in ButtonData structure.
         */
        void storeData(const ButtonData buttonData);

        void reset();
};

/*
 * Stores button data to the client application.
 */
extern StoreButtonData ButtonDataStorage;

#endif
