#ifndef Messenger_H
#define Messenger_H

#include "../types.h"

class Messenger {
    public:
        /*
         * This is meant to be called regularly when incoming data may be available.
         */
        static void checkForData();

        /*
         * Send data with its payload to the client.
         */
        static void sendData(MessageType type, byte* payload, int size);

        /*
         * Convencience function to print text to the client.
         */
        static void print(String text);

        /*
         * Sets the function that will deliver data for the REQUEST_STATUS message.
         */
        static void setMaxLoopDurationFunction(unsigned long (*getMaxLoopDuration) ());
};

#endif
