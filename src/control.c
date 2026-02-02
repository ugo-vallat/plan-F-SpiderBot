#include "log.h"
#include "servomotor.h"
#include <defines.h>
#include <bluetooth.h>
#include <control.h>

void ctrl_make_decision(cmd_t cmd);

sm_move_t g_next_move = SM_STOP;

void init_module_control(void) {
    return;
}

void ctrl_control_legs(cmd_t cmd) {
    switch (cmd) {
        case CMD_STOP:
            g_next_move = SM_STOP;
            break;
        case CMD_FORWARD:
            g_next_move = SM_FORWARD;
            break;
        case CMD_REVERSE:
            g_next_move = SM_REVERSE;
            break;
        case CMD_ROTATE_LEFT:
            g_next_move = SM_ROTATE_LEFT;
            break;
        case CMD_ROTATE_RIGHT:
            g_next_move = SM_ROTATE_RIGHT;
            break;
        case CMD_INIT_SHOULDER:
            g_next_move = SM_INIT_SHOULDER;
            break;
        case CMD_INIT_ELBOW:
            g_next_move = SM_INIT_ELBOW;
            break;
        default:
            WARNL("undefined command : %d", cmd);
            break;
    }
}



void ctrl_update_robot(void) {
    ctrl_control_legs(bluetooth_get_last_cmd());
    sm_set_next_move(g_next_move);
}