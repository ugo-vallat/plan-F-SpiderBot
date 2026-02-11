#ifndef __BLUETOOTH_H__
#define __BLUETOOTH_H__
#include "defines.h"


// Initialise le module Bluetooth et la réception DMA
void init_module_bluetooth(void);

// Vérifie le buffer DMA pour voir si de nouvelles commandes sont arrivées
void bluetooth_receive_cmd(void);

// Renvoie la dernière commande valide reçue
cmd_t bluetooth_get_last_cmd(void);

// Fonctions d'envoi de données 
void bluetooth_send_char(char c);
void bluetooth_send_string(const char* str);

void bluetooth_print_dma_debug(void);

// Vérifie si un appareil est connecté au Bluetooth
bool bluetooth_is_connected(void);

#endif // __BLUETOOTH_H__