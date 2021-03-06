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
ButtonData bytesToButtonData(const uint8_t* bytes);

/*
 * Converts an AppInfo structure into its byte representation.
 */
void appInfoToBytes(const AppInfo* appInfo, uint8_t* buf);

/*
 * Converts an integer into its byte representation.
 */
void intToBytes(uint16_t value, uint8_t* buf);

/*
 * Converts bytes to their integer representation.
 */
uint16_t bytesToInt(const uint8_t* bytes);

#endif
