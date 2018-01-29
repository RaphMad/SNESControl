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
int lastLoop;

void calculateLoopDuration() {
    int timeNow = millis();
    int loopDuration = timeNow - lastLoop;

    if (loopDuration > appInfo.maxLoopDuration) {
        appInfo.maxLoopDuration = loopDuration;
    }

    lastLoop = timeNow;
}

int lastLatch;

void calculateLatchInfo() {
    int timeNow = millis();
    int latchDuration = timeNow - lastLatch;

    appInfo.lastLatchDuration = latchDuration;

    if (latchDuration < (FRAME_LENGTH / 2)) appInfo.shortLatches++;
    if (latchDuration > (FRAME_LENGTH + (FRAME_LENGTH / 2))) appInfo.longLatches++;
    appInfo.numberOfLatches++;

    if (appInfo.firstLatch == 0) {
        appInfo.firstLatch = millis();
    }

    lastLatch = timeNow;
}

void setup() {
    attachInterrupt(digitalPinToInterrupt(PIN_LATCH), handleFallingLatchPulse, FALLING);

    Serial.begin(115200);

    ReadController::begin();
    WriteToConsole::begin();
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

void checkButtonTiming(ButtonData* buttonData) {
    int buttonTime = buttonData->pressedAt;
    int timeNow = millis();
    int timeFromFirstLatch = timeNow - appInfo.firstLatch;

    int buttonDelay = buttonTime - timeFromFirstLatch - FRAME_LENGTH;

    if (buttonDelay < -FRAME_LENGTH) {
        // loading the next button data effectively skips a frame
        LoadButtonData::getData();
        appInfo.skipCount++;
    } else if (buttonDelay > FRAME_LENGTH / 2) {
        // delay a bit so we can sync up
        delay(buttonDelay);
        appInfo.delayCount++;
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
            ButtonData buttonData = LoadButtonData::getData();
            checkButtonTiming(&buttonData);

            WriteToConsole::prepareData(buttonData);
        }
    }

    pollController();
    Messenger::checkForData();
    calculateLoopDuration();
}
