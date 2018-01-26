/*
 * Messages are formed by a leading byte that indicates the message type, followed by a payload of message specific size.
 */

typedef enum {
    /*
     * Direction: Arduino -> Client
     *
     * First byte: 0
     * Second byte: Length of stored data (max 256).
     * Following bytes: Data, to be interpreted double byte values.
     */
    SaveButtonData = 0
} MessageTypes;