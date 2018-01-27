#ifndef ReadController_h
#define ReadController_h

#include "types.h"

class ReadController {
    public:
        static void begin();

        /*
         * Poll controller for data.
         */
        static ButtonData getData();
};

#endif
