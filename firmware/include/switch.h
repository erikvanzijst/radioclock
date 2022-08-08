#ifndef FIRMWARE_SWITCH_H
#define FIRMWARE_SWITCH_H

/**
 * Registers callbacks for the switch.
 *
 * @param short_press   invoked after a short press
 * @param long_press    invoked after a long press
 * @return
 */
int32_t switch_init(void (* short_press)(void), void (* long_press)(void));

#endif //FIRMWARE_SWITCH_H
