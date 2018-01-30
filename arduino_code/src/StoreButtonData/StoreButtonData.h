#ifndef StoreButtonData_h
#define StoreButtonData_h

#include "common.h"

class StoreButtonData {
    public:

        /*
         * Stores the passed in ButtonData structure.
         */
        static void storeData(ButtonData buttonData);

        static void reset();
};

#endif
