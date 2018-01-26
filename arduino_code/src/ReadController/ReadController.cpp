#include <Arduino.h>
#include "ReadController.h"

void ReadController::begin() {
    pinMode(PIN_CONTROLLER_LATCH, OUTPUT);
    pinMode(PIN_CONTROLLER_CLOCK, OUTPUT);
}

void pulse(uint8_t pin) {
    digitalWrite(pin, HIGH);
    digitalWrite(pin, LOW);
}


ButtonData ReadController::getData() {
    ButtonData sampledData;

    // pulse latch to tell the controller to sample the current button states
    pulse(PIN_CONTROLLER_LATCH);

    // first button is already available, read it
    sampledData.B = digitalRead(PIN_CONTROLLER_DATA);

    // shift following buttons
    pulse(PIN_CONTROLLER_CLOCK);
    sampledData.Y = digitalRead(PIN_CONTROLLER_DATA);

    pulse(PIN_CONTROLLER_CLOCK);
    sampledData.SELECT = digitalRead(PIN_CONTROLLER_DATA);

    pulse(PIN_CONTROLLER_CLOCK);
    sampledData.START = digitalRead(PIN_CONTROLLER_DATA);

    pulse(PIN_CONTROLLER_CLOCK);
    sampledData.UP = digitalRead(PIN_CONTROLLER_DATA);

    pulse(PIN_CONTROLLER_CLOCK);
    sampledData.DOWN = digitalRead(PIN_CONTROLLER_DATA);

    pulse(PIN_CONTROLLER_CLOCK);
    sampledData.LEFT = digitalRead(PIN_CONTROLLER_DATA);

    pulse(PIN_CONTROLLER_CLOCK);
    sampledData.RIGHT = digitalRead(PIN_CONTROLLER_DATA);

    pulse(PIN_CONTROLLER_CLOCK);
    sampledData.A = digitalRead(PIN_CONTROLLER_DATA);

    pulse(PIN_CONTROLLER_CLOCK);
    sampledData.X = digitalRead(PIN_CONTROLLER_DATA);

    pulse(PIN_CONTROLLER_CLOCK);
    sampledData.SHOULDER_LEFT = digitalRead(PIN_CONTROLLER_DATA);

    pulse(PIN_CONTROLLER_CLOCK);
    sampledData.SHOULDER_RIGHT = digitalRead(PIN_CONTROLLER_DATA);

    return sampledData;
}