#ifndef LoadButtonData_h
#define LoadButtonData_h

#include "types.h"

class LoadButtonData {
    public:
        /*
         * Internally requests the first bulk of data,
         * so that it (hopefully) is already available when data is requested.
         */
        static void begin();

        /*
         * This is meant to be called regularly when incoming data is available.
         *
         * The return value indicates whether this was the very first portion of data that has been received.
         */
        static bool processIncomingData();

        /*
         * Read the next available button data.
         */
        static ButtonData getData();
};

#endif
