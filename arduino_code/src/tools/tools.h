#ifndef tools_h
#define tools_h

#include "common.h"

/*
 * Converts a ButtonData structure into its byte representation.
 */
void buttonDataToBytes(const ButtonData* buttonData, byte* buf);

/*
 * Converts bytes into the corresponding ButtonData structure.
 * Expects an array of two bytes.
 */
ButtonData bytesToButtonData(const byte* bytes);

/*
 * Converts an integer into its byte representation.
 */
void intToBytes(uint16_t value, byte* buf);

/*
 * Converts bytes to their integer representation.
 */
uint16_t bytesToInt(const byte* bytes);

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
String formatButtonData(const ButtonData& buttonData);

#endif
