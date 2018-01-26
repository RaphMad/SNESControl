#ifndef WriteToConsole_H
#define WriteToConsole_H

#include "types.h"

class WriteToConsole {
    public:
        static void begin();

        /*
         * Prepares button data for the next latch cycle.
         *
         * This function is meant for data that can be prepared in advance (like from a replay).
         */
        static void prepareData(ButtonData buttonData);

        /*
         * Combines data with prepared data.
         * This overlay buttons will be combined with the prepared regular data.
         *
         * This function is meant for data that comes in dynamically (like from a controller).
         */
        static void addData(ButtonData additionalData);

        /*
         * Retrieves the latest data that has been read by the console (may be a combination of prepared and combined data).
         */
        static ButtonData getLatestData();
};

#endif