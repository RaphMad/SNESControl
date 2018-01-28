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
const int FRAME_LENGTH = 20;

volatile bool isAfterLatch = false;

void handleFallingLatchPulse() {
    // keep interrupt handler short, actual code will be executed in main loop
    isAfterLatch = true;
}

AppInfo appInfo;

/*
 * Keep some statistics.
 * Maximum loop duration should be well below the frame time (16-20ms).
 * Maximum latch duration should be exactly 16ms (NTSC) or 20ms (PAL).
 * There should be no short latches.
 * Number of long latches should correspond to lag frames.
 */
unsigned long lastLoop;

void calculateLoopDuration() {
    unsigned long timeNow = millis();
    unsigned long loopDuration = timeNow - lastLoop;

    if (loopDuration > appInfo.maxLoopDuration) {
        appInfo.maxLoopDuration = loopDuration;
    }

    lastLoop = timeNow;
}

unsigned long lastLatch;
int shortLatches;
int longLatches;

void calculateLatchInfo() {
    unsigned long timeNow = millis();
    unsigned long latchDuration = timeNow - lastLatch;

    appInfo.lastLatchDuration = latchDuration;

    if (latchDuration < (FRAME_LENGTH / 2)) shortLatches++;
    if (latchDuration > (FRAME_LENGTH + (FRAME_LENGTH / 2))) longLatches++;

    lastLatch = timeNow;
}

void setup() {
    attachInterrupt(digitalPinToInterrupt(PIN_LATCH), handleFallingLatchPulse, FALLING);

    Serial.begin(115200);

    ReadController::begin();
    WriteToConsole::begin();

    Messenger::setAppInfo(&appInfo);
}

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

void loop() {
    if (isAfterLatch) {
        isAfterLatch = false;
        calculateLatchInfo();

        if (appInfo.isInSaveMode) {
            StoreButtonData::storeData(WriteToConsole::getLatestData());
        }

        if (appInfo.isInReplayMode) {
            WriteToConsole::prepareData(LoadButtonData::getData());
        }
    }

    pollController();
    Messenger::checkForData();
    calculateLoopDuration();
}
