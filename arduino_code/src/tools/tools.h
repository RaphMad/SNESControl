#ifndef tools_h
#define tools_h

#include "common.h"

/*
 * Converts a ButtonData structure into its byte representation.
 */
void buttonDataToBytes(ButtonData* buttonData, byte* buf);

/*
 * Converts bytes into the corresponding ButtonData structure.
 * Expects an array of two bytes.
 */
ButtonData bytesToButtonData(byte* bytes);

/*
 * Converts a ButtonData structure into a human readable format.
 * Only for debugging purposes, String will cause unpredictable
 * memory allocation results in production.
 */
String formatButtonData(ButtonData* buttonData);

/*
 * Gets the currently available free RAM.
 *
 * This is the amount of bytes between the stack and the heap.
 */
int getFreeRam();

/*
 * Converts an integer into its byte representation.
 */
void intToBytes(int value, byte* buf);

/*
 * Converts a AppInfo structure into its byte representation.
 */
void appInfoToBytes(AppInfo* appInfo, byte* buf);

#endif
