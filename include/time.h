#ifndef __TIME_H__
#define __TIME_H__

#include <defines.h>

typedef unsigned long time_t;

/**
 * @brief Init time module
 * 
 */
void init_time(void);

/**
 * @brief Get the time in us
 * 
 * @return time 
 */
time_t get_time(void);

/**
 * @brief Test if the deadline is reached
 * 
 * @param deadline deadline to test
 * @return true if reached, false else
 */
bool deadline_is_reached(time_t deadline);

#endif // __TIME_H__