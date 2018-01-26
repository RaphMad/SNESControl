#include <Arduino.h>

// pin to button mapping
const uint8_t PIN_BTN_B = 4;
const uint8_t PIN_BTN_Y = 5;
const uint8_t PIN_BTN_SELECT = 6;
const uint8_t PIN_BTN_START = 7;
const uint8_t PIN_BTN_UP = 8;
const uint8_t PIN_BTN_DOWN = 9;
const uint8_t PIN_BTN_L = 10;
const uint8_t PIN_BTN_R = 11;
const uint8_t PIN_BTN_A = 12;

const uint8_t PIN_BTN_X = A0;
const uint8_t PIN_BTN_SHOULDER_LEFT = A1;
const uint8_t PIN_BTN_SHOULDER_RIGHT = A2;

// latch line is attached to an interrupt pin
const uint8_t PIN_LATCH = 2;

volatile bool isAfterLatch = false;

uint32_t latchCounter = 0;

void handleFallingLatchPulse() {
    // keep interrupt handler short, actual code will be executed in main loop
    isAfterLatch = true;
}

void setup() {
    Serial.begin(9600);

    pinMode(PIN_BTN_B, OUTPUT);
    pinMode(PIN_BTN_Y, OUTPUT);
    pinMode(PIN_BTN_SELECT, OUTPUT);
    pinMode(PIN_BTN_START, OUTPUT);
    pinMode(PIN_BTN_UP, OUTPUT);
    pinMode(PIN_BTN_DOWN, OUTPUT);
    pinMode(PIN_BTN_L, OUTPUT);
    pinMode(PIN_BTN_R, OUTPUT);
    pinMode(PIN_BTN_A, OUTPUT);
    pinMode(PIN_BTN_X, OUTPUT);
    pinMode(PIN_BTN_SHOULDER_LEFT, OUTPUT);
    pinMode(PIN_BTN_SHOULDER_RIGHT, OUTPUT);

    // interrupt pint is set to input per default
    attachInterrupt(digitalPinToInterrupt(PIN_LATCH), handleFallingLatchPulse, FALLING);

    // provide first button states
    digitalWrite(PIN_BTN_B, LOW);
    digitalWrite(PIN_BTN_Y, HIGH);
    digitalWrite(PIN_BTN_SELECT, HIGH);
    digitalWrite(PIN_BTN_START, HIGH);
    digitalWrite(PIN_BTN_UP, HIGH);
    digitalWrite(PIN_BTN_DOWN, LOW);
    digitalWrite(PIN_BTN_L, HIGH);
    digitalWrite(PIN_BTN_R, HIGH);
    digitalWrite(PIN_BTN_A, HIGH);
    digitalWrite(PIN_BTN_X, HIGH);
    digitalWrite(PIN_BTN_SHOULDER_LEFT, HIGH);
    digitalWrite(PIN_BTN_SHOULDER_RIGHT, HIGH);
}

void loop() {
    if (isAfterLatch) {
        isAfterLatch = false;

        // do something here
        latchCounter++;
    }

    if (latchCounter % 60 == 0) {
        Serial.print("Current latchcounter: ");
        Serial.println(latchCounter, DEC);
    }
}