/**
 * @file battery.h
 * @brief Battery monitoring module for STM32F4.
 *
 * This module handles reading the battery voltage via ADC1 on pin PA1.
 * It is designed to calculate the remaining charge percentage for a 
 * 2-cell Li-Ion battery pack.
 */

#ifndef BATTERY_H
#define BATTERY_H

#include <stdint.h>

/* ========================================================================= *
 * INITIALIZATION                                                            *
 * ========================================================================= */

/**
 * @brief Initializes the battery monitoring module.
 * Configures ADC1 and the corresponding GPIO pin (PA1) in analog mode.
 */
void init_module_battery(void);


/* ========================================================================= *
 * MEASUREMENT                                                               *
 * ========================================================================= */

/**
 * @brief Performs an ADC measurement and calculates the battery charge level.
 * Calibrated for a 2-cell Li-Ion battery pack (Voltage range: 6.0V to 8.4V).
 * * @return The battery percentage (0 to 100), or -1 in case of a conversion error.
 */
int get_battery_percentage(void);

#endif // BATTERY_H