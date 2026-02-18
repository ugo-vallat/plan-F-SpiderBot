#ifndef __DEFINES_H__
#define __DEFINES_H__


/* _____ BOOL _____ */

typedef unsigned char bool;
#define true 1
#define false 0


/* _____ OTHERS _____ */

#define null ((void*) 0)

/* _____ MAIN _____ */
#define MAIN_SM_CALL_DURATION   1000000

/* _____ COMMANDS _____ */
typedef enum {
    CMD_STOP            = 0,
    CMD_FORWARD         = 1,
    CMD_REVERSE         = 2,
    CMD_ROTATE_LEFT     = 3,
    CMD_ROTATE_RIGHT    = 4,
    CMD_INIT_Z_AXIS   = 5,
    CMD_INIT_X_AXIS      = 6,
    CMD_MAX             = 7
} cmd_t;


#endif // __DEFINES_H__