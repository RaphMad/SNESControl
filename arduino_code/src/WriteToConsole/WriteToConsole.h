#ifndef WriteToConsole_H
#define WriteToConsole_H

#include <Arduino.h>
#include "common.h"

class WriteToConsole {
    private:
        ButtonData lastPreparedData;
        ButtonData lastAdditionalData;
    public:
        void begin();

        /*
         * Prepares button data for the next latch cycle.
         *
         * This function is meant for data that can be prepared in advance (like from a replay).
         */
        void prepareData(const ButtonData buttonData);

        /*
         * Combines data with prepared data.
         * This overlay buttons will be combined with the prepared regular data.
         *
         * This function is meant for data that comes in dynamically (like from a controller).
         */
        void addData(const ButtonData additionalData);

        /*
         * Retrieves the latest data that has been read by the console (combined of prepared and combined data).
         */
        const ButtonData getLatestData();
};

/*
 * Writes inputs to the SNES console.
 */
extern WriteToConsole ConsoleWriter;

#endif
