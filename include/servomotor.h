#ifndef __SERVOMOTOR_H__
#define __SERVOMOTOR_H__

#include <stm32f4/rcc.h>
#include <stm32f4/gpio.h>
#include <stm32f4/tim.h>
#include <time.h>


void init_servomotor(void);

void sm_move(time_t t, int step);

#endif // __SERVOMOTOR_H__