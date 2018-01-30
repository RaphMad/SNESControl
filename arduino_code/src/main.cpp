#include <Arduino.h>
#include "tools\tools.h"
#include "Communication/Messenger.h"
#include "LoadButtonData/LoadButtonData.h"
#include "ReadController/ReadController.h"
#include "StoreButtonData/StoreButtonData.h"
#include "WriteToConsole/WriteToConsole.h"

/*
 * Holds global information about the application.
 */
AppInfo appInfo;

/*
 * Expected frame length in ms.
 * Should be 20ms for PAL, 16ms for NTSC consoles.
 */
static const uint8_t FRAME_LENGTH = 20;

/*
 * Indicates whether we are right after a latch.
 */
static volatile bool isAfterLatch = false;

void handleFallingLatchPulse();
void calculateLatchInfo();
void checkButtonTiming(uint16_t);
void pollController();
void calculateLoopDuration();

void setup() {
    attachInterrupt(digitalPinToInterrupt(PIN_LATCH), handleFallingLatchPulse, FALLING);

    Serial.begin(115200);

    ReadController::begin();
    WriteToConsole::begin();
}

void handleFallingLatchPulse() {
    // keep interrupt handler short, actual code will be executed in main loop
    isAfterLatch = true;
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
            checkButtonTiming(buttonData.pressedAt);

            WriteToConsole::prepareData(buttonData);
        }
    }

    pollController();
    Messenger::checkForData();
    calculateLoopDuration();
}

void calculateLatchInfo() {
    static uint16_t lastLatch;

    uint16_t timeNow = millis();
    uint16_t latchDuration = timeNow - lastLatch;

    appInfo.lastLatchDuration = latchDuration;

    if (latchDuration < (FRAME_LENGTH / 2)) appInfo.shortLatches++;
    if (latchDuration > (FRAME_LENGTH + (FRAME_LENGTH / 2))) appInfo.longLatches++;

    if (appInfo.firstLatch == 0) {
        appInfo.firstLatch = millis();
    }

    lastLatch = timeNow;
}

void checkButtonTiming(uint16_t pressedAt) {
    uint16_t timeNow = millis();
    uint16_t timeFromFirstLatch = timeNow - appInfo.firstLatch;

    int8_t buttonDelay = pressedAt - timeFromFirstLatch - FRAME_LENGTH;

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

void pollController() {
    // Controller will be polled twice per frame.
    static uint16_t lastPoll;
    static uint8_t pollDelta = FRAME_LENGTH / 2;

    uint16_t timeNow = millis();

    if (timeNow - lastPoll > pollDelta) {
       WriteToConsole::addData(ReadController::getData());
       lastPoll = timeNow;
    }
}

void calculateLoopDuration() {
    static uint16_t lastLoop;

    uint16_t timeNow = millis();
    uint16_t loopDuration = timeNow - lastLoop;

    if (loopDuration > appInfo.maxLoopDuration) {
        appInfo.maxLoopDuration = loopDuration;
    }

    lastLoop = timeNow;
}
