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

/*
 * Controller will be polled half a frame after a latch.
 */
static bool wasPolledThisLatch = false;

static void handleFallingLatchPulse();
static void saveButtonData();
static void prepareNextReplayFrame();
static void calculateLatchInfo();
static void fixButtonTiming(uint16_t);
static void pollController();
static void calculateLoopDuration();

void setup() {
    attachInterrupt(digitalPinToInterrupt(PIN_LATCH), handleFallingLatchPulse, FALLING);

    // A baud rate of 125k results in a nice integer ratio to a clock speed of 16 Mhz
    // http://wormfood.net/avrbaudcalc.php?bitrate=125k&clock=16&databits=8
    Serial.begin(125000);

    ControllerReader.begin();
    ConsoleWriter.begin();
}

static void handleFallingLatchPulse() {
    // keep interrupt handler short, actual code will be executed in main loop
    isAfterLatch = true;
}

void loop() {
    if (isAfterLatch) {
        isAfterLatch = false;
        wasPolledThisLatch = false;

        calculateLatchInfo();
        saveButtonData();
        prepareNextReplayFrame();
    }

    pollController();
    MessageProcessor.checkForData();
    calculateLoopDuration();
}

static uint16_t firstLatch;

static void saveButtonData() {
    if (appInfo.isInSaveMode) {
        ButtonData buttonData = ConsoleWriter.getLatestData();
        buttonData.pressedAt = millis() - firstLatch;

        ButtonDataStorage.storeData(buttonData);
    }
}

static void prepareNextReplayFrame() {
    if (appInfo.isInReplayMode) {
        ButtonData buttonData = ButtonDataLoader.getData();
        fixButtonTiming(buttonData.pressedAt);

        ConsoleWriter.prepareData(buttonData);
    }
}

static uint16_t lastLatch;

static void calculateLatchInfo() {
    const uint16_t timeNow = millis();
    const uint16_t latchDuration = timeNow - lastLatch;

    if (firstLatch == 0) {
        firstLatch = millis();
    }

    // a short latch is a latch less than 1/2 a frame
    if (latchDuration < (FRAME_LENGTH / 2)) appInfo.shortLatches++;

    // a long latch is a latch longer than 3/2 a frame
    if (lastLatch != 0 && latchDuration > (FRAME_LENGTH + (FRAME_LENGTH / 2))) appInfo.longLatches++;

    lastLatch = timeNow;
}

static void fixButtonTiming(const uint16_t pressedAt) {
    const uint16_t timeNow = millis();
    const uint16_t timeFromFirstLatch = timeNow - firstLatch;

    const int8_t buttonDelay = pressedAt - timeFromFirstLatch - FRAME_LENGTH;

    if (buttonDelay < -FRAME_LENGTH) {
        // loading the next button data effectively skips a frame
        ButtonDataLoader.getData();
        appInfo.skipCount++;
    } else if (buttonDelay > FRAME_LENGTH / 2) {
        // delay a bit so we can sync up
        delay(buttonDelay);
        appInfo.delayCount++;
    }
}

static void pollController() {
    if (!wasPolledThisLatch && millis() - lastLatch > FRAME_LENGTH / 2) {
       ConsoleWriter.addData(ControllerReader.getData());
       wasPolledThisLatch = true;
    }
}

static uint16_t lastLoop;

static void calculateLoopDuration() {
    const uint16_t timeNow = millis();
    const uint16_t loopDuration = timeNow - lastLoop;

    if (loopDuration > appInfo.maxLoopDuration) {
        appInfo.maxLoopDuration = loopDuration;
    }

    lastLoop = timeNow;
}
