#ifndef tools_h
#define tools_h

#include "common.h"

/*
 * Converts a ButtonData structure into its byte representation.
 */
void buttonDataToBytes(const ButtonData* buttonData, uint8_t* buf);

/*
 * Converts bytes into the corresponding ButtonData structure.
 * Expects an array of two bytes.
 */
const ButtonData bytesToButtonData(const uint8_t* bytes);

/*
 * Converts an integer into its byte representation.
 */
void intToBytes(uint16_t value, uint8_t* buf);

/*
 * Converts bytes to their integer representation.
 */
uint16_t bytesToInt(const uint8_t* bytes);

/*
 * Gets the currently available free RAM.
 *
 * This is the amount of bytes between the stack and the heap.
 */
uint16_t getFreeRam();

/*
 * Converts a ButtonData structure into a human readable format.
 * Only for debugging purposes, String will cause unpredictable
 * memory allocation results in production.
 */
const String formatButtonData(const ButtonData& buttonData);

#endif
