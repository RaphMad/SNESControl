#include "ReadController.h"

static void pulseLatch();
static bool sampleButton();
static void pulseClock();

void ReadController::begin() {
    // set pins A3-A4 as output
    DDRC |= B00011000;

    // equivalent to
    //pinMode(PIN_CONTROLLER_CLOCK, OUTPUT);
    //pinMode(PIN_CONTROLLER_LATCH, OUTPUT);

    // set pin A5 to INPUT_PULLUP
    // having this pin configured as PULLUP is important to get consistent HIGH states when no controller is connected
    PORTC |= B00100000;

    // equivalent to
    //pinMode(PIN_CONTROLLER_DATA, INPUT_PULLUP);
}

const ButtonData ReadController::getData() {
    ButtonData sampledData;

    // pulse latch to tell the controller to sample the current button states
    pulseLatch();

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

static void pulseLatch() {
    PORTC |= B00010000;
    PORTC &= B11101111;

    // equivalent to
    //digitalWrite(PIN_CONTROLLER_LATCH, HIGH);
    //digitalWrite(PIN_CONTROLLER_LATCH, LOW);
}

static bool sampleButton() {
    pulseClock();

    // give PINC some time to react
    // 12us is the time per sampled button data on a real SNES.
    delayMicroseconds(12);

    return PINC & B00100000;
}

static void pulseClock() {
    PORTC |= B00001000;
    PORTC &= B11110111;

    // equivalent to
    //digitalWrite(PIN_CONTROLLER_CLOCK, HIGH);
    //digitalWrite(PIN_CONTROLLER_CLOCK, LOW);
}

ReadController ControllerReader;
