#ifndef BATTERY_H
#define BATTERY_H

#include <stdint.h>

/**
 * @brief Initialise le module battery.
 * Configure l'ADC1 et la broche PA1.
 */
void init_module_battery(void);

/**
 * @brief Effectue une mesure ADC et calcule le niveau de charge.
 * Calibration pour 2 piles Li-Ion (6.0V - 8.4V).
 * * @return int Pourcentage de battery (0 à 100), ou -1 en cas d'erreur.
 */
int get_battery_percentage(void);

#endif // BATTERY_H