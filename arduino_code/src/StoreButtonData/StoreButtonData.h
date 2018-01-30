#ifndef StoreButtonData_h
#define StoreButtonData_h

#include <Arduino.h>
#include "common.h"
#include "../Communication/Messenger.h"

class StoreButtonData {
    public:

        /*
         * Stores the passed in ButtonData structure.
         */
        static void storeData(ButtonData buttonData);

        static void reset();
};

#endif
