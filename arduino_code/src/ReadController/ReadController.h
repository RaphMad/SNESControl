#ifndef ReadController_H
#define ReadController_H

#include "types.h"

class ReadController {
    public:
        static void begin();
        static ButtonData getData();
};

#endif
