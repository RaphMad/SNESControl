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
static AppInfo appInfo;

/*
 * Expected frame length in ms.
 * Should be 20ms for PAL, 16ms for NTSC consoles.
 */
static const uint8_t FRAME_LENGTH = 20;

/*
 * Timestamp of the last (most current) latch.
 */
static uint16_t lastLatch;

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
static void pollController();
static void calculateLoopDuration();

static void handleEnableSaveMessage(const uint8_t* payload, uint8_t size);
static void handleEnableLoadMessage(const uint8_t* payload, uint8_t size);
static void handlePingMessage(const uint8_t* payload, const uint8_t size);
static void handleRequestStatusMessage(const uint8_t* payload, uint8_t size);
static void handleDisableSaveMessage(const uint8_t* payload, uint8_t size);
static void handleDisableLoadMessage(const uint8_t* payload, uint8_t size);
static void handleResetDataMessage(const uint8_t* payload, uint8_t size);
static void handleLoadResponseMessage(const uint8_t* payload, uint8_t size);

void setup() {
    // stop the built in led from lightning up by setting it to output
    DDRB |= B00100000;

    // equivalent to
    //pinMode(LED_BUILTIN, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(PIN_LATCH), handleFallingLatchPulse, FALLING);

    // A baud rate of 125k results in a nice integer ratio to a clock speed of 16 Mhz
    // http://wormfood.net/avrbaudcalc.php?bitrate=125k&clock=16&databits=8
    Serial.begin(125000);

    ControllerReader.begin();
    ConsoleWriter.begin();

    MessageProcessor.registerMessageHandler(ENABLE_SAVE, handleEnableSaveMessage);
    MessageProcessor.registerMessageHandler(ENABLE_LOAD, handleEnableLoadMessage);
    MessageProcessor.registerMessageHandler(PING, handlePingMessage);
    MessageProcessor.registerMessageHandler(REQUEST_STATUS, handleRequestStatusMessage);
    MessageProcessor.registerMessageHandler(DISABLE_SAVE, handleDisableSaveMessage);
    MessageProcessor.registerMessageHandler(DISABLE_LOAD, handleDisableLoadMessage);
    MessageProcessor.registerMessageHandler(RESET_DATA, handleResetDataMessage);
    MessageProcessor.registerMessageHandler(LOAD_RESPONSE, handleLoadResponseMessage);
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
    MessageProcessor.pollForMessage();

    calculateLoopDuration();
}

static uint16_t firstLatchForSave;
static uint16_t firstLatchForLoad;

static void calculateLatchInfo() {
    const uint16_t timeNow = millis();
    const uint16_t latchDuration = timeNow - lastLatch;

    if (lastLatch != 0) {
        // a short latch is a latch less than 1/2 a frame
        if (latchDuration < (FRAME_LENGTH / 2)) appInfo.shortLatches++;

        // a long latch is a latch longer than 3/2 a frame
        if (latchDuration > (FRAME_LENGTH + (FRAME_LENGTH / 2))) appInfo.longLatches++;
    }

    if (firstLatchForSave == 0) {
        firstLatchForSave = timeNow;
    }

    if (firstLatchForLoad == 0) {
        firstLatchForLoad = timeNow;
    }

    lastLatch = timeNow;
}

static void saveButtonData() {
    if (appInfo.isInSaveMode) {
        ButtonData buttonData = ConsoleWriter.getLatestData();
        buttonData.pressedAt = lastLatch - firstLatchForSave;

        ButtonDataStorage.storeData(buttonData);
    }
}

static void prepareNextReplayFrame() {
    if (appInfo.isInReplayMode) {
        ConsoleWriter.prepareData(ButtonDataLoader.getData());
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

static void handleEnableSaveMessage(const uint8_t* const payload, const uint8_t size) {
    appInfo.isInSaveMode = true;
    firstLatchForSave = 0;

    // discard data from a possible previous save session
    ButtonDataStorage.reset();
}

static void handleEnableLoadMessage(const uint8_t* const payload, const uint8_t size) {
    appInfo.isInReplayMode = true;
    firstLatchForLoad = 0;

    // pre-fill internal buffers
    // so data it is already available when first latch is received
    ButtonDataLoader.loadInitialData();
}

static void handlePingMessage(const uint8_t* const payload, const uint8_t size) {
    MessageProcessor.sendMessage(PONG, payload, size);
}

static void handleRequestStatusMessage(const uint8_t* const payload, const uint8_t size) {
    const uint8_t dataBufferSize = sizeof(AppInfo) + sizeof(uint16_t);
    uint8_t bytesToSend[dataBufferSize];

    appInfoToBytes(&appInfo, bytesToSend);
    intToBytes(getFreeRam(), bytesToSend + sizeof(AppInfo));

    MessageProcessor.sendMessage(INFO_RESPONSE, bytesToSend, dataBufferSize);
}

static void handleDisableSaveMessage(const uint8_t* const payload, const uint8_t size) {
    appInfo.isInSaveMode = false;
}

static void handleDisableLoadMessage(const uint8_t* const payload, const uint8_t size) {
    appInfo.isInReplayMode = false;

    // set prepared button data to a "no buttons pressed" state
    ConsoleWriter.reset();
}

static void handleResetDataMessage(const uint8_t* const payload, const uint8_t size) {
    appInfo.isInReplayMode = false;

    appInfo.longLatches = 0;
    appInfo.shortLatches = 0;

    appInfo.isInSaveMode = false;
    appInfo.maxLoopDuration = 0;

    // reset all ongoing replay actions
    ButtonDataLoader.reset();
    ButtonDataStorage.reset();
    ConsoleWriter.reset();
}

static void handleLoadResponseMessage(const uint8_t* const payload, const uint8_t size) {
    bool wasInitialData = ButtonDataLoader.processIncomingData(payload, size);

    // prepare initial button states
    if (wasInitialData) {
        ConsoleWriter.prepareData(ButtonDataLoader.getData());
    }
}
