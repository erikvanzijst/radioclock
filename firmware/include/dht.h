#ifndef FIRMWARE_DHT_H
#define FIRMWARE_DHT_H

#include <stdint.h>

int32_t dht_init(void);

int32_t dht_deinit(void);

/**
 * Temperature is returned in Celsius times 10.
 * To get the temperature, divide by 10. Do modulo 10 for one decimal.
 *
 * @return
 */
int32_t get_temperature();

/**
 * Humidity is returned as a percentage.
 *
 * @return
 */
int32_t get_humidity();

#endif //FIRMWARE_DHT_H
