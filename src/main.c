#include "time.h"
#include <tinyprintf.h>
#include <log.h>

/**
 * @brief Call init function of each module
 * 
 */
void init(void) {
    init_log();
    init_time();
}

void main_loop(void) {
    while(true) {
        
    }
}

int main(void) {
    init();

    main_loop();

    return 0;
}