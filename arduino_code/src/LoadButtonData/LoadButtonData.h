#ifndef LoadButtonData_h
#define LoadButtonData_h

#include "types.h"

class LoadButtonData {
    public:
        static void begin();
        static bool handleIncomingData();
        static ButtonData getData();
};

#endif
