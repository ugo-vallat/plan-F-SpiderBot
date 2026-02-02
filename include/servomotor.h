#ifndef __SERVOMOTOR_H__
#define __SERVOMOTOR_H__

#include <stm32f4/rcc.h>
#include <stm32f4/gpio.h>
#include <stm32f4/tim.h>
#include <time.h>

#define SM_UPDATE_PERIOD    50000  // Time in us between each update of position
#define SM_MOVE_DIVIDER     50      // Splitting a 90° angle

#define SM_TIMER_1  TIM3    // Used for z axis
#define SM_TIMER_2  TIM4    // Used for x axis

#define SM_TIMER_1_CLK  APB1_CLK
#define SM_TIMER_2_CLK  APB1_CLK

#define SM_FRZ_GPIO GPIOB
#define SM_RRZ_GPIO GPIOB
#define SM_RLZ_GPIO GPIOB
#define SM_FLZ_GPIO GPIOB
#define SM_FRX_GPIO GPIOB
#define SM_RRX_GPIO GPIOB
#define SM_RLX_GPIO GPIOB
#define SM_FLX_GPIO GPIOB

#define SM_FRZ_PIN  0
#define SM_RRZ_PIN  1
#define SM_RLZ_PIN  4
#define SM_FLZ_PIN  5
#define SM_FRX_PIN  6
#define SM_RRX_PIN  7
#define SM_RLX_PIN  8
#define SM_FLX_PIN  9

#define SM_FRZ_AF   0x2
#define SM_RRZ_AF   0x2
#define SM_RLZ_AF   0x2
#define SM_FLZ_AF   0x2
#define SM_FRX_AF   0x2
#define SM_RRX_AF   0x2
#define SM_RLX_AF   0x2
#define SM_FLX_AF   0x2

// Example : &(SM_TIMER_1->CCR3)
#define SM_FRZ_CCR  &(SM_TIMER_1->CCR3)
#define SM_RRZ_CCR  &(SM_TIMER_1->CCR4)
#define SM_RLZ_CCR  &(SM_TIMER_1->CCR1)
#define SM_FLZ_CCR  &(SM_TIMER_1->CCR2)
#define SM_FRX_CCR  &(SM_TIMER_2->CCR1)
#define SM_RRX_CCR  &(SM_TIMER_2->CCR2)
#define SM_RLX_CCR  &(SM_TIMER_2->CCR3)
#define SM_FLX_CCR  &(SM_TIMER_2->CCR4)

typedef enum {
    SM_FRZ=0, // Front Right Z axis
    SM_RRZ=1, // Rear Right Z axis
    SM_RLZ=2, // Rear Lelft Z axis
    SM_FLZ=3, // Front Lelft Z axis
    SM_FRX=4, // Front Right X axis
    SM_RRX=5, // Rear Right X axis
    SM_RLX=6, // Rear Lelft X axis
    SM_FLX=7, // Front Lelft X axis
} sm_id;

typedef enum {
    SM_STOP         = 0,
    SM_FORWARD      = 1,
    SM_REVERSE      = 2,
    SM_ROTATE_LEFT  = 3,
    SM_ROTATE_RIGHT = 4,
    SM_INIT_Z_AXIS= 5,
    SM_INIT_X_AXIS   = 6,
    SM_MAX          = 7
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
 * @param t Duration of movement in us
 */
void sm_move(time_t t);

#endif // __SERVOMOTOR_H__