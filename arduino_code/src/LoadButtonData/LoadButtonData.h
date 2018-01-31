#ifndef LoadButtonData_h
#define LoadButtonData_h

#include "../tools/tools.h"
#include "../Communication/Messenger.h"
#include "../WriteToConsole/WriteToConsole.h"

/*
 * Use two alternating input buffers to ensure a consistent flow of inputs to the console.
 *
 * The active input buffer is the one that is being read from.
 * Meanwhile data for the inactive buffer is being fetched from the client.
 */
class LoadButtonData {
    private:
        /*
        * With a value of 64 about two load request are sent per second (assuming inputs come in at 50 or 60Hz).
        */
        static const uint8_t BUFFER_SIZE = MAX_CONTENT_SIZE;

        uint8_t inputBuffer1[BUFFER_SIZE];
        uint8_t inputBuffer1Index = 0;

        uint8_t inputBuffer2[BUFFER_SIZE];
        uint8_t inputBuffer2Index = 0;

        bool isInputBuffer1Active = false;
        bool hasInitialData = false;

        const ButtonData readFromBuffer1();
        const ButtonData readFromBuffer2();

    public:
        /*
         * Internally requests the first bulk of data,
         * so that it (hopefully) is already available when data is requested.
         */
        void begin();

        /*
         * This is meant to be called regularly when incoming data is available.
         */
        void processIncomingData(const uint8_t* buf, uint8_t size);

        /*
         * Read the next available button data.
         */
        const ButtonData getData();

        /*
         * Resets to the initial state.
         */
        void reset();
};

/*
 * Loads button data from the client application.
 */
extern LoadButtonData ButtonDataLoader;

#endif
