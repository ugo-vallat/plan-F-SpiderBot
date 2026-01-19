#ifndef __LOG__H__
#define __LOG__H__

#include <defines.h>

#define COLOR_ENABLE 1 // 1 for colorfull messages, 0 else

#define PRINTL(format, ...) printl(__func__, format, ##__VA_ARGS__)
#define WARNL(format, ...) warnl(__file__, __func__, format, ##__VA_ARGS__)
#define ASSERTL(assert, format, ...) assertl(assert, __file__, __func__, format, ##__VA_ARGS__)

/**
 * @brief Initializes the logger by setting its output to a given path
 */
void init_log(void);

/**
 * @brief Writes to the logger output
 *
 * @param[in] format Message format string followed by varargs
 *                   (same syntax as printf)
 */
void printl(const char *format, ...);

/**
 * @brief Logs an error message to the logger
 * @remarks The error message may take a form depending on the current
 *          logger output, e.g. red text for stdout or specific characters
 *          when written to a file
 *
 * @param[in] file_name Name of the calling file
 * @param[in] fun_name Name of the calling function
 * @param[in] format Message format string followed by varargs
 *                   (same syntax as printf)
 */
void warnl(const char *file_name, const char *fun_name, const char *format, ...);

/**
 * @brief if assertion is false, display warning
 *
 * @param[in] assert Assertion to test
 * @param[in] file_name Caller file name
 * @param[in] fun_name Caller function name
 * @param[in] exit_value Value returned at exit
 * @param[in] format Format of the message
 *
 * @return value of the assert
 */
bool assertl(bool assert, const char *file_name, const char *fun_name, const char *format, ...);

/**
 * @brief Ferme le logger
 * @remark Ne fais rien si la sortie est stdout
 */
void close_logger(void);

#endif // __LOG__H__
