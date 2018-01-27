#ifndef Messenger_H
#define Messenger_H

#include "../types.h"

class Messenger {
    public:
        /*
         * This is meant to be called regularly when incoming data may be available.
         */
        static void checkForData();
};

#endif
