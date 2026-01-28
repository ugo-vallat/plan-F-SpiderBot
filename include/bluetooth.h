#ifndef __BLUETOOTH_H__
#define __BLUETOOTH_H__

#include <servomotor.h>

void init_module_bluetooth(void);

void bluetooth_receive_cmd(void);

sm_move_t bluetooth_get_last_cmd(void);


#endif // __BLUETOOTH_H__