#ifndef FIRMWARE_LDR_H
#define FIRMWARE_LDR_H

#include <stdint.h>

extern volatile  uint8_t ldr[1];

int32_t ldr_init();

#endif //FIRMWARE_LDR_H
