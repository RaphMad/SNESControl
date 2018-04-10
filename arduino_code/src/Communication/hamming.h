#ifndef hamming_h
#define hamming_h

#include <Arduino.h>

/*
 * Calculates the parity for a single byte.
 *
 * The parity will be stored in the lower 4 bits of the returned value.
 */
uint8_t calculateParity128(uint8_t value);

/*
 * Calculates the parity for two bytes.
 *
 * Parity for first byte will be stored in the lower 4 bits,
 * parity for the second bytes in the upper 4 bits of the returned value.
 */
uint8_t calculateParity2416(uint8_t first, uint8_t second);

/*
 * Defines the result of a correction operation.
 */
typedef enum {
    NO_ERROR = 0,
    ONE_CORRECTED_ERROR = 1,
    TWO_CORRECTED_ERRORS = 2,
    CORRECTION_NOT_POSSIBLE = 2,
} CorrectionResult;

/*
 * Attempts to correct the value of a byte using the provided parity information.
 * Parity information is assumed to reside in the lower 4 bits.
 *
 * The byte will be corrected in-place (if possible).
 *
 * The result of the operation is returned.
 */
CorrectionResult correct128(uint8_t* value, uint8_t parity);

/*
 * Attempts to correct the values of two bytes using the provided parity information.
 *
 * Parity information is assumed to reside in the lower 4 bits for the first,
 * and in the upper 4 bits for the second byte.
 *
 * The bytes will be corrected in-place (if possible).
 *
 * The result of the operation is returned.
 */
CorrectionResult correct2416(uint8_t* first, uint8_t* second, uint8_t parity);

#endif
