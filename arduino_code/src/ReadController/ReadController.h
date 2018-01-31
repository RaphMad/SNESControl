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
        ButtonData getData();
};

extern ReadController ControllerReader;

#endif
