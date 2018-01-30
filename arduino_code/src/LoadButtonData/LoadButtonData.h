#ifndef LoadButtonData_h
#define LoadButtonData_h

#include "common.h"

class LoadButtonData {
    public:
        /*
         * Internally requests the first bulk of data,
         * so that it (hopefully) is already available when data is requested.
         */
        static void begin();

        /*
         * This is meant to be called regularly when incoming data is available.
         */
        static void processIncomingData(byte* buf, int size);

        /*
         * Read the next available button data.
         */
        static ButtonData getData();

        /*
         * Resets to the initial state.
         */
        static void reset();
};

#endif
