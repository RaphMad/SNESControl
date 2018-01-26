#include <Arduino.h>
#include "ReadController/ReadController.h"
#include "WriteToConsole/WriteToConsole.h"
#include "StoreButtonData/StoreButtonData.h"

volatile bool isAfterLatch = false;

void handleFallingLatchPulse(){
    // keep interrupt handler short, actual code will be executed in main loop
    isAfterLatch = true;
}

void setup() {
    Serial.begin(9600);
    ReadController::begin();
    WriteToConsole::begin();
    StoreButtonData::begin();

    attachInterrupt(digitalPinToInterrupt(PIN_LATCH), handleFallingLatchPulse, FALLING);
}

void loop() {
    if (isAfterLatch) {
        isAfterLatch = false;

        StoreButtonData::storeData(WriteToConsole::getLatestData());
    }

    WriteToConsole::addData(ReadController::getData());
}