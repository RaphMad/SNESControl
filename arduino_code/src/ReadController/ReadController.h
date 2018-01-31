#ifndef ReadController_h
#define ReadController_h

#include <Arduino.h>
#include "common.h"

class ReadController {
    public:
        void begin() const;

        /*
         * Poll controller for data.
         */
        ButtonData getData() const;
};

extern ReadController ControllerReader;

#endif
