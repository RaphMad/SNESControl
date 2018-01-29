#include <Arduino.h>
#include "tools\tools.h"
#include "Communication/Messenger.h"
#include "LoadButtonData/LoadButtonData.h"
#include "ReadController/ReadController.h"
#include "StoreButtonData/StoreButtonData.h"
#include "WriteToConsole/WriteToConsole.h"


/*
 * Expected frame length in ms.
 * Should be 20ms for PAL, 16ms for NTSC consoles.
 */
const int FRAME_LENGTH = 20;

/*
 * Controller will be polled twice per frame.
 */
int lastPoll;
int pollDelta = FRAME_LENGTH / 2;

/*
 * Timestamps of last loop and last latch - used to calculate performance info.
 */
int lastLoop;
int lastLatch;

/*
 * Holds global information about the application.
 */
AppInfo appInfo;

/*
 * Indicates whether we are right after a latch.
 */
volatile bool isAfterLatch = false;

void handleFallingLatchPulse() {
    // keep interrupt handler short, actual code will be executed in main loop
    isAfterLatch = true;
}

void setup() {
    attachInterrupt(digitalPinToInterrupt(PIN_LATCH), handleFallingLatchPulse, FALLING);

    Serial.begin(115200);

    ReadController::begin();
    WriteToConsole::begin();
}

void calculateLoopDuration() {
    int timeNow = millis();
    int loopDuration = timeNow - lastLoop;

    if (loopDuration > appInfo.maxLoopDuration) {
        appInfo.maxLoopDuration = loopDuration;
    }

    lastLoop = timeNow;
}

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
