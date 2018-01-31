#include "ReadController.h"

/*
 * These constants are used to set the ports directly via the registers.
 */
static const uint8_t CLOCK_MASK = B00001000;
static const uint8_t LATCH_MASK = B00010000;
static const uint8_t DATA_MASK = B00100000;

static void pulseLatch();
static bool sampleButton();
static void pulseClock();

void ReadController::begin() const {
    // set pins A3-A4 as output
    DDRC |= (LATCH_MASK | CLOCK_MASK);

    // equivalent to
    //pinMode(PIN_CONTROLLER_CLOCK, OUTPUT);
    //pinMode(PIN_CONTROLLER_LATCH, OUTPUT);

    // set pin A5 to INPUT_PULLUP
    // having this pin configured as PULLUP is important to get consistent HIGH states when no controller is connected
    PORTC |= DATA_MASK;

    // equivalent to
    //pinMode(PIN_CONTROLLER_DATA, INPUT_PULLUP);
}

ButtonData ReadController::getData() const {
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
    PORTC |= LATCH_MASK;

    // latch has a duration of 12us in the SNES controller protocol
    delayMicroseconds(12);

    PORTC &= ~LATCH_MASK;

    // equivalent to
    //digitalWrite(PIN_CONTROLLER_LATCH, HIGH);
    //digitalWrite(PIN_CONTROLLER_LATCH, LOW);
}

static bool sampleButton() {
    pulseClock();

    return PINC & DATA_MASK;
}

static void pulseClock() {
    // delay between clock cycles is 6us in the SNES controller protocol
    delayMicroseconds(6);
    PORTC |= CLOCK_MASK;

    // time the clock is high is 6us in the SNES controller protocol
    delayMicroseconds(6);

    PORTC &= ~CLOCK_MASK;

    // equivalent to
    //digitalWrite(PIN_CONTROLLER_CLOCK, HIGH);
    //digitalWrite(PIN_CONTROLLER_CLOCK, LOW);
}

ReadController ControllerReader;
