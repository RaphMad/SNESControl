#ifndef StoreButtonData_H
#define StoreButtonData_H

#include "types.h"

class StoreButtonData {
    public:
        static void begin();
        static void storeData(ButtonData buttonData);

        /*
         * Needs to be called after a consecutive stream of data is finished.
         */
        static void flush();
};

#endif
