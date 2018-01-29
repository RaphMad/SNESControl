#ifndef StoreButtonData_h
#define StoreButtonData_h

#include "types.h"

class StoreButtonData {
    public:

        /*
         * Stores the passed in ButtonData structure.
         */
        static void storeData(ButtonData* buttonData);

        /*
         * Sets a reference to the app info.
         */
        static void setAppInfo(AppInfo* appInfo);

        static void reset();
};

#endif
