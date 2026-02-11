#ifndef __STM32F4_DMA_H__
#define __STM32F4_DMA_H__

#include <stdint.h>

/**
 * @brief Structure représentant les registres d'un Stream DMA
 */
typedef struct {
    volatile uint32_t CR;     /*!< DMA stream x configuration register      */
    volatile uint32_t NDTR;   /*!< DMA stream x number of data register     */
    volatile uint32_t PAR;    /*!< DMA stream x peripheral address register */
    volatile uint32_t M0AR;   /*!< DMA stream x memory 0 address register   */
    volatile uint32_t M1AR;   /*!< DMA stream x memory 1 address register   */
    volatile uint32_t FCR;    /*!< DMA stream x FIFO control register       */
} DMA_Stream_TypeDef;


/**
 * @brief Adresses de base des contrôleurs DMA
 */
#define DMA1_BASE             (0x40026000UL)
#define DMA2_BASE             (0x40026400UL)

/**
 * @brief Calcul des adresses des Streams pour le DMA2
 * Les streams commencent à l'offset 0x10.
 * Chaque stream occupe 0x18 octets (24 bytes).
 */

#define DMA2_Stream0_BASE     (DMA2_BASE + 0x010UL)
#define DMA2_Stream1_BASE     (DMA2_BASE + 0x028UL)
#define DMA2_Stream2_BASE     (DMA2_BASE + 0x040UL)
#define DMA2_Stream3_BASE     (DMA2_BASE + 0x058UL)
#define DMA2_Stream4_BASE     (DMA2_BASE + 0x070UL)
#define DMA2_Stream5_BASE     (DMA2_BASE + 0x088UL)
#define DMA2_Stream6_BASE     (DMA2_BASE + 0x0A0UL)
#define DMA2_Stream7_BASE     (DMA2_BASE + 0x0B8UL)

/**
 * @brief Pointeurs d'accès aux structures DMA
*/
#define DMA2_Stream0          ((DMA_Stream_TypeDef *) DMA2_Stream0_BASE)
#define DMA2_Stream1          ((DMA_Stream_TypeDef *) DMA2_Stream1_BASE)
#define DMA2_Stream2          ((DMA_Stream_TypeDef *) DMA2_Stream2_BASE)
#define DMA2_Stream3          ((DMA_Stream_TypeDef *) DMA2_Stream3_BASE)
#define DMA2_Stream4          ((DMA_Stream_TypeDef *) DMA2_Stream4_BASE)
#define DMA2_Stream5          ((DMA_Stream_TypeDef *) DMA2_Stream5_BASE)
#define DMA2_Stream6          ((DMA_Stream_TypeDef *) DMA2_Stream6_BASE)
#define DMA2_Stream7          ((DMA_Stream_TypeDef *) DMA2_Stream7_BASE)

/**
 * @brief Registres globaux d'interruption (Communs à tous les streams du contrôleur)
 */
#define DMA2_LISR             (*(volatile uint32_t *)(DMA2_BASE + 0x00)) // Low Interrupt Status Register
#define DMA2_HISR             (*(volatile uint32_t *)(DMA2_BASE + 0x04)) // High Interrupt Status Register
#define DMA2_LIFCR            (*(volatile uint32_t *)(DMA2_BASE + 0x08)) // Low Interrupt Flag Clear Register
#define DMA2_HIFCR            (*(volatile uint32_t *)(DMA2_BASE + 0x0C)) // High Interrupt Flag Clear Register

#endif // __STM32F4_DMA_H__