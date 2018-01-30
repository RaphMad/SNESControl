#ifndef ReadController_h
#define ReadController_h

#include <Arduino.h>
#include "common.h"

class ReadController {
    public:
        void begin();

        /*
         * Poll controller for data.
         */
        const ButtonData getData();
};

extern ReadController ControllerReader;

#endif
