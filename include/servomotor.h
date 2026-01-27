#ifndef __SERVOMOTOR_H__
#define __SERVOMOTOR_H__

#include <stm32f4/rcc.h>
#include <stm32f4/gpio.h>
#include <stm32f4/tim.h>
#include <time.h>

#define SM_UPDATE_PERIOD    50000  // Time in us between each update of position
#define SM_MOVE_DIVIDER     50      // Splitting a 90° angle

#define SM_TIMER_1  TIM3
#define SM_TIMER_2  TIM4

#define SM_FRS_GPIO GPIOB
#define SM_RRS_GPIO GPIOB
#define SM_RLS_GPIO GPIOB
#define SM_FLS_GPIO GPIOB
#define SM_FRE_GPIO GPIOB
#define SM_RRE_GPIO GPIOB
#define SM_RLE_GPIO GPIOB
#define SM_FLE_GPIO GPIOB

#define SM_FRS_PIN  0
#define SM_RRS_PIN  1
#define SM_RLS_PIN  4
#define SM_FLS_PIN  5
#define SM_FRE_PIN  6
#define SM_RRE_PIN  7
#define SM_RLE_PIN  8
#define SM_FLE_PIN  9

#define SM_FRS_AF   0x2
#define SM_RRS_AF   0x2
#define SM_RLS_AF   0x2
#define SM_FLS_AF   0x2
#define SM_FRE_AF   0x2
#define SM_RRE_AF   0x2
#define SM_RLE_AF   0x2
#define SM_FLE_AF   0x2

#define SM_FRS_CCR  &(SM_TIMER_1->CCR3)
#define SM_RRS_CCR  &(SM_TIMER_1->CCR4)
#define SM_RLS_CCR  &(SM_TIMER_1->CCR1)
#define SM_FLS_CCR  &(SM_TIMER_1->CCR2)
#define SM_FRE_CCR  &(SM_TIMER_2->CCR1)
#define SM_RRE_CCR  &(SM_TIMER_2->CCR2)
#define SM_RLE_CCR  &(SM_TIMER_2->CCR3)
#define SM_FLE_CCR  &(SM_TIMER_2->CCR4)

typedef enum {
    SM_FRS=0, // Front Right Shoulder
    SM_RRS=1, // Rear Right Shoulder
    SM_RLS=2, // Rear Lelft Shoulder
    SM_FLS=3, // Front Lelft Shoulder
    SM_FRE=4, // Front Right Elbow
    SM_RRE=5, // Rear Right Elbow
    SM_RLE=6, // Rear Lelft Elbow
    SM_FLE=7, // Front Lelft Elbow
} sm_id;

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