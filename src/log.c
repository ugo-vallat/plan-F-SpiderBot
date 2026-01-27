#include <stdarg.h>
#include <tinyprintf.h>
#include <log.h>


#if COLOR_ENABLE == 1
    #define YELLOW "\033[38;5;184m"
    #define ORANGE "\033[38;5;208m"
    #define RED "\033[38;5;160m"
    #define RSTC "\033[0m"
#else
    #define YELLOW  ""
    #define ORANGE  ""
    #define RED     ""
    #define RSTC    ""
#endif


void init_module_log(void) {
}

void printl(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

void warnl(const char *file_name, const char *fun_name, const char *format, ...) {
    va_list args;
    va_start(args, format);
    printf(YELLOW);
    printf("[warnl] %s > %s : ", file_name, fun_name);
    vprintf(format, args);
    printf(RSTC);
    printf("\n");
    va_end(args);
}

bool assertl(bool assert, const char *file_name, const char *fun_name, const char *format, ...) {
    if(!assert) {
        va_list args;
        va_start(args, format);
        printf(ORANGE);
        printf("[assertl] %s > %s : ", file_name, fun_name);
        vprintf(format, args);
        printf(RSTC);
        printf("\n");
        va_end(args);
    }
    return assert;
}

