#ifndef FIRMWARE_UTILS_H
#define FIRMWARE_UTILS_H

#include <stdint.h>

/**
 * Computes the parity bit for the given 32-bit value.
 */
uint32_t parity(uint32_t v);

uint8_t crc8(uint8_t *ptr, uint8_t len);

const char* get_filename(const char* path);

#endif //FIRMWARE_UTILS_H
