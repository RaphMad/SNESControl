#include <Arduino.h>
#include "ReadController.h"

void ReadController::begin() {
    pinMode(PIN_CONTROLLER_LATCH, OUTPUT);
    pinMode(PIN_CONTROLLER_CLOCK, OUTPUT);

    // having this pin configured as PULLUP is important to get consistent HIGH states when no controller is connected
    pinMode(PIN_CONTROLLER_DATA, INPUT_PULLUP);
}

void pulse(byte pin) {
    digitalWrite(pin, HIGH);
    digitalWrite(pin, LOW);
}

bool sampleButton() {
    pulse(PIN_CONTROLLER_CLOCK);
    return digitalRead(PIN_CONTROLLER_DATA);
}

ButtonData ReadController::getData() {
    ButtonData sampledData;

    // pulse latch to tell the controller to sample the current button states
    pulse(PIN_CONTROLLER_LATCH);

    // first button is already available, read it
    sampledData.B = digitalRead(PIN_CONTROLLER_DATA);

    // sample the following buttons
    sampledData.Y = sampleButton();
    sampledData.SELECT = sampleButton();
    sampledData.START = sampleButton();
    sampledData.UP = sampleButton();
    sampledData.DOWN = sampleButton();
    sampledData.LEFT = sampleButton();
    sampledData.RIGHT = sampleButton();
    sampledData.A = sampleButton();
    sampledData.X = sampleButton();
    sampledData.SHOULDER_LEFT = sampleButton();
    sampledData.SHOULDER_RIGHT = sampleButton();

    return sampledData;
}
