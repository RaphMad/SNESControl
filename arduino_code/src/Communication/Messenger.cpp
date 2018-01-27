#include "Messenger.h"

/*
 * Markers were chosen deliberately to have low values, since most of the transmitted data will be high values.
 * (HIGH means buttons are not pressed)
 * This will ensure a minimal encoding overhead for typical scenarios.
 */
const byte START_MARKER = 0;
const byte END_MARKER = 1;
const byte ENCODE_MARKER = 2;

/*
 * Due the encoding, in a worst case we may need double the content size plus 2 bytes for the message type.
 */
byte receivedBytes[MAX_CONTENT_SIZE * 2 + 2];

int receivedBytesIndex;
bool receiveInProgress;

void decodeReceivedMessage() {
    // 1 skips the start marker, -1 omits the end marker
    for (int i = 1, j = 0; i < receivedBytesIndex - 1; i++) {
        byte currentByte = receivedBytes[i];

        if (currentByte == ENCODE_MARKER) {
            // coding scheme: 0 <-> 2 3 , 1 <-> 2 4, 2 <-> 2 5
            i++;
            byte nextByte = receivedBytes[i];
            currentByte = nextByte - ENCODE_MARKER - 1;
        }

        // re-use the receive buffer for the decoded data
        receivedBytes[j] = currentByte;
        j++;
    }

    MessageType messageType = (MessageType)receivedBytes[0];

    switch(messageType) {
        case ENABLE_SAVE:
            Serial.println("Received ENABLE_SAVE command");
            break;
        case SAVE:
            Serial.println("Received SAVE command");
            break;
        case ENABLE_LOAD:
            Serial.println("Received ENABLE_LOAD command");
            break;
        case LOAD:
            Serial.println("Received LOAD command");
            break;
    }
}

void Messenger::checkForData() {
    if (Serial.available() > 0) {
        byte receivedByte = Serial.read();

        if (receivedByte == START_MARKER) {
            receivedBytesIndex = 0;
            receiveInProgress = true;
        }

        if (receiveInProgress) {
            receivedBytes[receivedBytesIndex] = receivedByte;
            receivedBytesIndex++;
        }

        if (receivedByte == END_MARKER) {
            receiveInProgress = false;

            decodeReceivedMessage();
        }
    }
}
