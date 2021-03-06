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
 * Indicates whether we are right after a latch.
 */
static volatile bool isAfterLatch = false;

static void handleFallingLatchPulse();
static void saveButtonData();
static void prepareNextReplayFrame();
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

    // disable Timer0 overflow interrupt (renders millis(), micros() and delay() unusable)
    noInterrupts();
    bitClear(TIMSK0, TOIE0);
    interrupts();

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

        saveButtonData();
        prepareNextReplayFrame();

        if (!appInfo.isInReplayMode) {
            pollController();
        }
    }

    MessageProcessor.pollForMessage();
}

static void saveButtonData() {
    if (appInfo.isInSaveMode) {
        ButtonDataStorage.storeData(ConsoleWriter.getLatestData());
    }
}

static void prepareNextReplayFrame() {
    if (appInfo.isInReplayMode && ButtonDataLoader.hasData()) {
        ConsoleWriter.prepareData(ButtonDataLoader.getData());
    }
}

static void pollController() {
    ConsoleWriter.addData(ControllerReader.getData());
}

static void handleEnableSaveMessage(const uint8_t* const payload, const uint8_t size) {
    appInfo.isInSaveMode = true;

    // discard data from a possible previous save session
    ButtonDataStorage.reset();
}

static void handleEnableLoadMessage(const uint8_t* const payload, const uint8_t size) {
    appInfo.isInReplayMode = true;

    // pre-fill internal buffers
    // so data it is already available when first latch is received
    ButtonDataLoader.loadInitialData();
}

static void handlePingMessage(const uint8_t* const payload, const uint8_t size) {
    MessageProcessor.sendMessage(PONG, payload, size);
}

static void handleRequestStatusMessage(const uint8_t* const payload, const uint8_t size) {
    uint8_t bytesToSend[sizeof(AppInfo)];
    appInfoToBytes(&appInfo, bytesToSend);

    MessageProcessor.sendMessage(INFO_RESPONSE, bytesToSend, sizeof(AppInfo));
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
    appInfo.isInSaveMode = false;

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
