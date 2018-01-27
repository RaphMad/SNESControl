#ifndef tools_h
#define tools_h

#include "types.h"

/*
 * Converts a ButtonData structure into its byte representation.
 * Will always return an array of two bytes.
 */
byte* buttonDataToBytes(ButtonData buttonData);

/*
 * Converts bytes into the corresponding ButtonData structure.
 * Expects an array of two bytes.
 */
ButtonData bytesToButtonData(byte* bytes);

/*
 * Converts a ButtonData structure into a human readable format.
 */
String formatButtonData(ButtonData buttonData);

#endif
