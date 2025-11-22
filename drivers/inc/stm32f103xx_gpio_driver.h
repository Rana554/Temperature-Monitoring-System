/*
 * stm32f103xx_gpio_driver.h
 *
 *  Created on: Oct 30, 2025
 *      Author: Rana Samir
 */

#ifndef INC_STM32F103XX_GPIO_DRIVER_H_
#define INC_STM32F103XX_GPIO_DRIVER_H_
#include "stm32f103xx.h"




typedef struct
{
    uint8_t PinNumber;      // 0 → 15
    uint8_t Mode;           // GPIO_MODE_...
    uint8_t CNF;            // GPIO_CNF_...
    uint8_t Pull;           // 0=None, 1=Pull-up, 2=Pull-down
    uint8_t OutputType;         // PP or OD
    uint8_t AltFunc;            // AF_PP or AF_OD if used
    uint8_t InterruptPin;       // pin is used to generate interrupt
    uint8_t Trigger;             // type of trigger falling /rising
} GPIO_PinConfig_t;

typedef struct
{
    GPIO_TypeDef *pGPIOx;          // GPIOA, GPIOB, etc.
    GPIO_PinConfig_t PinConfig;    // Configuration details
} GPIO_Handle_t;

/* pin number */
#define GPIO_PIN_0         0
#define GPIO_PIN_1         1
#define GPIO_PIN_2         2
#define GPIO_PIN_3         3
#define GPIO_PIN_4         4
#define GPIO_PIN_5         5
#define GPIO_PIN_6         6
#define GPIO_PIN_7         7
#define GPIO_PIN_8         8
#define GPIO_PIN_9         9
#define GPIO_PIN_10       10
#define GPIO_PIN_11       11
#define GPIO_PIN_12       12
#define GPIO_PIN_13       13
#define GPIO_PIN_14       14
#define GPIO_PIN_15       15

/*-------------------- Mode Definitions --------------------*/
/*
 * MODE[1:0]
 * 00 = Input mode
 * 01 = Output mode, max speed 10 MHz
 * 10 = Output mode, max speed 2 MHz
 * 11 = Output mode, max speed 50 MHz
 */
#define GPIO_MODE_INPUT         0x0
#define GPIO_MODE_OUT_10MHz     0x1
#define GPIO_MODE_OUT_2MHz      0x2
#define GPIO_MODE_OUT_50MHz     0x3

/*-------------------- CNF (Configuration) Bits --------------------*/
/*
 * For Input mode (MODE = 00)
 * 00: Analog mode
 * 01: Floating input
 * 10: Input with pull-up / pull-down
 * 11: Reserved
 */
#define GPIO_CNF_IN_ANALOG      0x0
#define GPIO_CNF_IN_FLOATING    0x1
#define GPIO_CNF_IN_PUPD        0x2

/*
 * For Output mode (MODE ≠ 00)
 * 00: General purpose output push-pull
 * 01: General purpose output open-drain
 * 10: Alternate function output push-pull
 * 11: Alternate function output open-drain
 */
#define GPIO_CNF_OUT_PP         0x0
#define GPIO_CNF_OUT_OD         0x1
#define GPIO_CNF_AF_PP          0x2
#define GPIO_CNF_AF_OD          0x3

/* Pull configuration */
#define GPIO_NO_PUPD            0
#define GPIO_PULL_UP            1
#define GPIO_PULL_DOWN          2

/* Output type */
#define GPIO_OUTPUT_PUSHPULL    0
#define GPIO_OUTPUT_OPENDRAIN   1

/* Alternate function type */
#define GPIO_AF_PUSHPULL        2
#define GPIO_AF_OPENDRAIN       3

/* Trigger Type */
#define GPIO_TRIGGER_RISING     0
#define GPIO_TRIGGER_FALLING    1

/*Peripheral clock setup*/
void GPIO_PeriClockControl(GPIO_TypeDef *pGPIOx,uint8_t EnorDi);

/* Init and DeInit*/

void GPIO_Init(GPIO_Handle_t* pGPIOHandle);
void GPIO_DeInit(GPIO_TypeDef *pGPIOx );


/*Data read and write*/

uint8_t GPIO_ReadFromInputPin(GPIO_TypeDef *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_TypeDef *pGPIOx);
void GPIO_WriteToOutputPin(GPIO_TypeDef *pGPIOx, uint8_t PinNumber ,uint8_t value);
void GPIO_WriteToOutputPort(GPIO_TypeDef *pGPIOx ,uint16_t value);
void GPIO_ToggleOutputPin(GPIO_TypeDef *pGPIOx, uint8_t PinNumber );

/* IRQ configuration and ISR handling*/

void GPIO_IRQConfig(uint8_t IRQNumber , uint8_t IRQPriority , uint8_t EnorDi);
void GPIO_IRQHandling(uint8_t PinNumber);

void enable_global_interrupts(void);

#endif /* INC_STM32F103XX_GPIO_DRIVER_H_ */
