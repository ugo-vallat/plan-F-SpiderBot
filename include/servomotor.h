#ifndef __SERVOMOTOR_H__
#define __SERVOMOTOR_H__

#include <stm32f4/rcc.h>
#include <stm32f4/gpio.h>
#include <stm32f4/tim.h>
#include <time.h>

#define SM_UPDATE_PERIOD    50000  // Time in us between each update of position
#define SM_MOVE_DIVIDER     50      // Splitting a 90° angle

typedef enum {
    SM_STOP         = 0,
    SM_FORWARD      = 1,
    SM_REVERSE      = 2,
    SM_ROTATE_LEFT  = 3,
    SM_ROTATE_RIGH  = 4
} sm_move_t;

/**
 * @brief Init servomotor module
 * 
 */
void init_module_servomotor(void);

/**
 * @brief Define the next move
 * 
 * @param m Next move
 */
void sm_set_next_move(sm_move_t m);

/**
 * @brief Move the robot during t us
 * 
 * @param t     Duration of movment in us
 */
void sm_move(time_t t);

#endif // __SERVOMOTOR_H__