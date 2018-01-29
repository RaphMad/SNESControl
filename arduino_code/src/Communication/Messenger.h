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
         * Size needs to be less than MAX_CONTENT_SIZE.
         */
        static void sendData(MessageType type, byte* payload, int size);

        /*
         * Convencience function to print text to the client.
         * Only for debugging purposes, String will cause unpredictable
         * memory allocation results in production.
         */
        static void print(String text);

        /*
         * Sets a reference to the app info.
         */
        static void setAppInfo(AppInfo* appInfo);
};

#endif
