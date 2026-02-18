/**
 * @file bluetooth.h
 * @brief Bluetooth module interface for STM32F4.
 * * This module handles UART communication using DMA for both 
 * transmitting and receiving commands via a Bluetooth module.
 */

#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "defines.h"
#include <stdbool.h>

/* ========================================================================= *
 * INITIALIZATION                                                            *
 * ========================================================================= */

/**
 * @brief Initializes the Bluetooth module hardware and DMA reception.
 * Configures USART, GPIOs, and the necessary DMA streams.
 */
void init_module_bluetooth(void);


/* ========================================================================= *
 * RECEPTION                                                                 *
 * ========================================================================= */

/**
 * @brief Polls the DMA buffer to check for newly arrived Bluetooth commands.
 * Should be called regularly in the main loop or a dedicated task.
 */
void bluetooth_receive_cmd(void);

/**
 * @brief Retrieves the last valid command received via Bluetooth.
 * @return The last command enumeration value (cmd_t).
 */
cmd_t bluetooth_get_last_cmd(void);


/* ========================================================================= *
 * TRANSMISSION                                                              *
 * ========================================================================= */

/**
 * @brief Sends a single character over Bluetooth (blocking mode).
 * @param c The character to send.
 * @note Remember to rename 'send_char' to 'bluetooth_send_char' in your .c file!
 */
void bluetooth_send_char(char c);

/**
 * @brief Sends a null-terminated string over Bluetooth using DMA.
 * @param str Pointer to the string to send.
 */
void bluetooth_send_string(const char* str);


/* ========================================================================= *
 * STATE & DEBUG                                                             *
 * ========================================================================= */

/**
 * @brief Checks if a device is currently connected to the Bluetooth module.
 * @return true if a device is connected, false otherwise.
 */
bool bluetooth_is_connected(void);

/**
 * @brief Prints the current DMA buffer state and pointers for debugging.
 */
void bluetooth_print_dma_debug(void);

/**
 * @brief Processes LED activity related to the Bluetooth status.
 * Updates LED indicators based on connection state or data transfer.
 */
void bluetooth_process_led_task(void);

/**
 * @brief Triggers a brief flicker on the Bluetooth status LED (Orange LED).
 * Creates a "negative pulse" effect to indicate data activity when connected.
 */
void bluetooth_trigger_activity(void);

#endif // BLUETOOTH_H