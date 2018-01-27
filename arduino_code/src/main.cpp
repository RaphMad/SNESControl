#include <Arduino.h>
#include "tools\tools.h"
#include "ReadController/ReadController.h"
#include "WriteToConsole/WriteToConsole.h"
#include "StoreButtonData/StoreButtonData.h"
#include "LoadButtonData/LoadButtonData.h"
#include "Communication/Messenger.h"

/*
 * Expected frame length in ms.
 * Should be 20ms for PAL, 16ms for NTSC consoles.
 */
const byte FRAME_LENGTH = 20;

volatile bool isAfterLatch = false;

bool isInReplayMode = false;
bool isInSaveMode = false;

void handleFallingLatchPulse() {
    // keep interrupt handler short, actual code will be executed in main loop
    isAfterLatch = true;
}

void setup() {
    Serial.begin(9600);

    ReadController::begin();
    WriteToConsole::begin();

    attachInterrupt(digitalPinToInterrupt(PIN_LATCH), handleFallingLatchPulse, FALLING);
}

/*
 * Poll controller twice per frame.
 */
int lastPoll = 0;
int pollDelta = FRAME_LENGTH / 2;

void pollController() {
    int timeNow = millis();

    if (timeNow - lastPoll > pollDelta) {
       WriteToConsole::addData(ReadController::getData());
       lastPoll = timeNow;
    }
}

void loop() {
    if (isAfterLatch) {
        isAfterLatch = false;

        if (isInSaveMode) {
            StoreButtonData::storeData(WriteToConsole::getLatestData());
        }

        if (isInReplayMode) {
            WriteToConsole::prepareData(LoadButtonData::getData());
        }
    }

    pollController();
    Messenger::checkForData();
}
