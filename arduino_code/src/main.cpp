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
const uint8_t PIN_BTN_SHOULDER_LEFT = A2;

// clock line is attached to an interrupt pin
const uint8_t PIN_CLOCK = 2;

uint8_t clockCounter = 0;

void setup() {
    pulseCounter++;
}

void loop() {
    // put your main code here, to run repeatedly:
}