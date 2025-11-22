/*
 * stm32f103xx.h
 *
 *  Created on: Oct 21, 2025
 *      Author: Rana Samir
 */

#ifndef INC_STM32F103XX_H_
#define INC_STM32F103XX_H_

#include <stdint.h>
/* base addresses of FLASH and SRAM memories */

#define FLASH_BASEADDR         0x08000000U
#define SRAM1_BASEADDR         0x20000000U
#define ROM_BASEADDR           0x1FFFF000U
#define SRAM                   SRAM1_BASEADDR

/* AHBx and APBx Bus Peripheral base addresses */

#define PERIPH_BASE            0x40000000U
#define APB1PERIPH_BASE        PERIPH_BASE
#define APB2PERIPH_BASE        0x40010000U
#define AHBPERIPH_BASE         0x40020000U

/* base addresses of GPIOx Peripherals */

#define GPIOA_BASEADDR        (APB2PERIPH_BASE+0x0800)
#define GPIOB_BASEADDR        (APB2PERIPH_BASE+0x0C00)
#define GPIOC_BASEADDR        (APB2PERIPH_BASE+0x1000)
#define GPIOD_BASEADDR        (APB2PERIPH_BASE+0x1400)

/* base addresses of NVIC and EXTI */

#define NVIC_ISER0          ((volatile uint32_t*)0xE000E100)
#define NVIC_ICER0          ((volatile uint32_t*)0xE000E180)
#define NVIC_IPR_BASE_ADDR  ((volatile uint32_t*)0xE000E400)

#define EXTI_BASEADDR       (0x40010400UL)
#define AFIO_BASEADDR       (0x40010000UL)



/* base addresses of RCC Peripheral */

#define RCC_BASEADDR          (AHBPERIPH_BASE+0x1000)

/* base addresses of ADC Peripheral */

#define ADC1_BASEADDR        (APB2PERIPH_BASE+0x2400)
#define ADC2_BASEADDR        (APB2PERIPH_BASE+0x2800)

/* peripheral register definitions structure  */

typedef struct
{
  volatile uint32_t CRL;    // offset 0x00
  volatile uint32_t CRH;    // offset 0x04
  volatile uint32_t IDR;    // offset 0x08
  volatile uint32_t ODR;    // offset 0x0C
  volatile uint32_t BSRR;   // offset 0x10
  volatile uint32_t BRR;    // offset 0x14
  volatile uint32_t LCKR;   // offset 0x18
} GPIO_TypeDef;

typedef struct
{
	volatile uint32_t CR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t APB2RSTR;
	volatile uint32_t APB1RSTR;
	volatile uint32_t AHBENR;
	volatile uint32_t APB2ENR;
	volatile uint32_t APB1ENR;
	volatile uint32_t BDCR;
	volatile uint32_t CSR;
	volatile uint32_t AHBSTR;

} RCC_TypeDef;

typedef struct
{
    volatile uint32_t EVCR;
    volatile uint32_t MAPR;
    volatile uint32_t EXTICR[4]; // EXTICR1..4 at 0x08..0x14
    volatile uint32_t MAPR2;
} AFIO_TypeDef;

#define AFIO   ((AFIO_TypeDef*)AFIO_BASEADDR)


typedef struct
{
  volatile uint32_t IMR;    // Interrupt Mask Register
  volatile uint32_t EMR;    // Event Mask Register
  volatile uint32_t RTSR;   // Rising Trigger Selection
  volatile uint32_t FTSR;   // Falling Trigger Selection
  volatile uint32_t SWIER;  // Software Interrupt Event Register
  volatile uint32_t PR;     // Pending Register
} EXTI_TypeDef;

#define EXTI   ((EXTI_TypeDef*)EXTI_BASEADDR)

typedef struct
{
    volatile uint32_t SR;    // Status register, offset 0x00
    volatile uint32_t CR1;   // Control register 1, offset 0x04
    volatile uint32_t CR2;   // Control register 2, offset 0x08
    volatile uint32_t SMPR1; // Sample time register 1, offset 0x0C
    volatile uint32_t SMPR2; // Sample time register 2, offset 0x10
    volatile uint32_t JOFR1; // Injected channel data offset register 1, offset 0x14
    volatile uint32_t JOFR2; // Injected channel data offset register 2, offset 0x18
    volatile uint32_t JOFR3; // Injected channel data offset register 3, offset 0x1C
    volatile uint32_t JOFR4; // Injected channel data offset register 4, offset 0x20
    volatile uint32_t HTR;   // Watchdog higher threshold register, offset 0x24
    volatile uint32_t LTR;   // Watchdog lower threshold register, offset 0x28
    volatile uint32_t SQR1;  // Regular sequence register 1, offset 0x2C
    volatile uint32_t SQR2;  // Regular sequence register 2, offset 0x30
    volatile uint32_t SQR3;  // Regular sequence register 3, offset 0x34
    volatile uint32_t JSQR;  // Injected sequence register, offset 0x38
    volatile uint32_t JDR1;  // Injected data register 1, offset 0x3C
    volatile uint32_t JDR2;  // Injected data register 2, offset 0x40
    volatile uint32_t JDR3;  // Injected data register 3, offset 0x44
    volatile uint32_t JDR4;  // Injected data register 4, offset 0x48
    volatile uint32_t DR;    // Regular data register, offset 0x4C
} ADC_TypeDef;

#define ADC1 ((ADC_TypeDef *) ADC1_BASEADDR)
#define ADC2 ((ADC_TypeDef *) ADC2_BASEADDR)

/* Peripheral definitions  */

#define GPIOA ((GPIO_TypeDef *) GPIOA_BASEADDR)
#define GPIOB ((GPIO_TypeDef *) GPIOB_BASEADDR)
#define GPIOC ((GPIO_TypeDef *) GPIOC_BASEADDR)
#define GPIOD ((GPIO_TypeDef *) GPIOD_BASEADDR)

#define RCC   ((RCC_TypeDef *) RCC_BASEADDR)

/* clock enable Macros for GPIOx peripherals */

#define GPIOA_PCLK_EN()            ( RCC->APB2ENR |=(1<<2))
#define GPIOB_PCLK_EN()            ( RCC->APB2ENR |=(1<<3))
#define GPIOC_PCLK_EN()            ( RCC->APB2ENR |=(1<<4))
#define GPIOD_PCLK_EN()            ( RCC->APB2ENR |=(1<<5))

#define AFIO_PCLK_EN()             ( RCC->APB2ENR |=(1<<0))
/* clock disable Macros for GPIOx peripherals */

#define GPIOA_PCLK_DI()            ( RCC->APB2ENR &=~(1<<2))
#define GPIOB_PCLK_DI()            ( RCC->APB2ENR &=~(1<<3))
#define GPIOC_PCLK_DI()            ( RCC->APB2ENR &=~(1<<4))
#define GPIOD_PCLK_DI()            ( RCC->APB2ENR &=~(1<<5))

/* macros to reset GPIOx peripherals */

#define GPIOA_REG_RESET()         do{RCC->APB2RSTR |=(1<<2); RCC->APB2RSTR &=~(1<<2);}while(0)
#define GPIOB_REG_RESET()         do{RCC->APB2RSTR |=(1<<3); RCC->APB2RSTR &=~(1<<3);}while(0)
#define GPIOC_REG_RESET()         do{RCC->APB2RSTR |=(1<<4); RCC->APB2RSTR &=~(1<<4);}while(0)
#define GPIOD_REG_RESET()         do{RCC->APB2RSTR |=(1<<5); RCC->APB2RSTR &=~(1<<5);}while(0)


#define GPIO_PORTCODE(x)  ((x == GPIOA)?0:\
                           (x == GPIOB)?1:\
                           (x == GPIOC)?2:\
                           (x == GPIOD)?3:0)

//some generic macros

#define ENABLE            1
#define DISABLE           0
#define SET             ENABLE
#define RESET           DISABLE
#define GPIO_PIN_SET    SET
#define GPIO_PIN_RESET  RESET








#endif /* INC_STM32F103XX_H_ */
