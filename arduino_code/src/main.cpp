#include <Arduino.h>
#include "tools\tools.h"
#include "ReadController/ReadController.h"
#include "WriteToConsole/WriteToConsole.h"
#include "StoreButtonData/StoreButtonData.h"
#include "LoadButtonData/LoadButtonData.h"
#include "Communication/Messenger.h"

volatile bool isAfterLatch = false;

void handleFallingLatchPulse() {
    // keep interrupt handler short, actual code will be executed in main loop
    isAfterLatch = true;
}

/*
 * Keep some statistics.
 * Maximum loop duration should be well below the frame time (16-20ms).
 */
unsigned long lastLoop;
unsigned long maxLoopDuration;

void calculateLoopDuration() {
    unsigned long timeNow = millis();
    unsigned long loopDuration = timeNow - lastLoop;

    if (loopDuration > maxLoopDuration) {
        maxLoopDuration = loopDuration;
    }

    lastLoop = timeNow;
}

unsigned long getMaxLoopDuration() {
    return maxLoopDuration;
}

void setup() {
    Serial.begin(115200);

    ReadController::begin();
    WriteToConsole::begin();

    Messenger::setMaxLoopDurationFunction(getMaxLoopDuration);

    attachInterrupt(digitalPinToInterrupt(PIN_LATCH), handleFallingLatchPulse, FALLING);
}

/*
 * Expected frame length in ms.
 * Should be 20ms for PAL, 16ms for NTSC consoles.
 */
const byte FRAME_LENGTH = 20;

/*
 * Poll controller twice per frame.
 */
unsigned long lastPoll = 0;
unsigned long pollDelta = FRAME_LENGTH / 2;

void pollController() {
    unsigned long timeNow = millis();

    if (timeNow - lastPoll > pollDelta) {
       WriteToConsole::addData(ReadController::getData());
       lastPoll = timeNow;
    }
}

bool isInReplayMode = false;
bool isInSaveMode = false;

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
    calculateLoopDuration();
}
