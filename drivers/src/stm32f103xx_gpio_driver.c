/*
 * stm32f103xx_gpio_driver.c
 *
 *  Created on: Oct 30, 2025
 *      Author: Rana Samir
 */
#include "stm32f103xx.h"
#include "stm32f103xx_gpio_driver.h"


/*Peripheral clock setup*/
void GPIO_PeriClockControl(GPIO_TypeDef *pGPIOx,uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pGPIOx == GPIOA)
		{
			GPIOA_PCLK_EN();
		}
		else if(pGPIOx == GPIOB){  GPIOB_PCLK_EN();}
		else if(pGPIOx == GPIOC){  GPIOC_PCLK_EN();	}
		else if(pGPIOx == GPIOD){  GPIOD_PCLK_EN();	}
	}else
	{
		if(pGPIOx == GPIOA)
		{
			GPIOA_PCLK_DI();
		}
		else if(pGPIOx == GPIOB){  GPIOB_PCLK_DI();}
		else if(pGPIOx == GPIOC){  GPIOC_PCLK_DI();	}
		else if(pGPIOx == GPIOD){  GPIOD_PCLK_DI();	}
	}
}

/* Init and DeInit*/

void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
    uint32_t pos = pGPIOHandle->PinConfig.PinNumber;
    uint32_t temp = 0;
    volatile uint32_t *configReg;

    /* 1. Select CRL or CRH */
    if (pos < 8)
        configReg = &pGPIOHandle->pGPIOx->CRL;
    else {
        configReg = &pGPIOHandle->pGPIOx->CRH;
        pos -= 8;
    }

    /* 2. Clear existing 4 bits */
    *configReg &= ~(0xF << (pos * 4));

    /* 3. Determine MODE and CNF bits */
    uint8_t mode = pGPIOHandle->PinConfig.Mode;
    uint8_t cnf = 0;

    /* If pin is used as EXTI interrupt → enable AFIO clock */
    if (pGPIOHandle->PinConfig.InterruptPin == ENABLE)
    {
           AFIO_PCLK_EN();

           /* Configure mapping in AFIO->EXTICR registers */
           uint8_t exti_index = pGPIOHandle->PinConfig.PinNumber / 4;
           uint8_t exti_pos   = (pGPIOHandle->PinConfig.PinNumber % 4) * 4;

           AFIO->EXTICR[exti_index] &= ~(0xF << exti_pos);   // clear
           AFIO->EXTICR[exti_index] |=  (GPIO_PORTCODE(pGPIOHandle->pGPIOx) << exti_pos);

           // unmask the EXTI line
           EXTI->IMR |= (1 << pGPIOHandle->PinConfig.PinNumber);

           if (pGPIOHandle->PinConfig.Trigger == GPIO_TRIGGER_FALLING)
           {
               EXTI->FTSR |= (1 << pGPIOHandle->PinConfig.PinNumber);
               EXTI->RTSR &= ~(1 << pGPIOHandle->PinConfig.PinNumber);
           }
           else if (pGPIOHandle->PinConfig.Trigger == GPIO_TRIGGER_RISING)
           {
               EXTI->RTSR |= (1 << pGPIOHandle->PinConfig.PinNumber);
               EXTI->FTSR &= ~(1 << pGPIOHandle->PinConfig.PinNumber);
           }

    }

    if (mode == GPIO_MODE_INPUT)
    {
        /* Input Mode */
        if (pGPIOHandle->PinConfig.CNF == GPIO_CNF_IN_ANALOG)
            cnf = GPIO_CNF_IN_ANALOG;
        else if (pGPIOHandle->PinConfig.CNF == GPIO_CNF_IN_FLOATING)
            cnf = GPIO_CNF_IN_FLOATING;
        else if (pGPIOHandle->PinConfig.CNF == GPIO_CNF_IN_PUPD)
            cnf = GPIO_CNF_IN_PUPD;
    }
    else
    {
        /* Output or Alternate Mode */
        if (pGPIOHandle->PinConfig.AltFunc)
            cnf = pGPIOHandle->PinConfig.AltFunc;
        else if (pGPIOHandle->PinConfig.OutputType == GPIO_OUTPUT_OPENDRAIN)
            cnf = GPIO_CNF_OUT_OD;
        else
            cnf = GPIO_CNF_OUT_PP;
    }

    /* 4. Combine MODE[1:0] + CNF[1:0] → 4 bits */
    temp = ((cnf << 2) | (mode & 0x03)) << (pos * 4);
    *configReg |= temp;

    /* 5. Configure pull-up / pull-down if input mode */
    if (mode == GPIO_MODE_INPUT && pGPIOHandle->PinConfig.CNF == GPIO_CNF_IN_PUPD)
    {
        if (pGPIOHandle->PinConfig.Pull == GPIO_PULL_UP)
            pGPIOHandle->pGPIOx->ODR |= (1 << pGPIOHandle->PinConfig.PinNumber);
        else if (pGPIOHandle->PinConfig.Pull == GPIO_PULL_DOWN)
            pGPIOHandle->pGPIOx->ODR &= ~(1 << pGPIOHandle->PinConfig.PinNumber);
    }
}


void GPIO_DeInit(GPIO_TypeDef *pGPIOx )
{
	if(pGPIOx == GPIOA)
	{
		GPIOA_REG_RESET();
	}
	else if(pGPIOx == GPIOB){  GPIOB_REG_RESET();}
	else if(pGPIOx == GPIOC){  GPIOC_REG_RESET();	}
	else if(pGPIOx == GPIOD){  GPIOD_REG_RESET();	}

}


/*Data read and write*/

uint8_t GPIO_ReadFromInputPin(GPIO_TypeDef *pGPIOx, uint8_t PinNumber)
{
	uint8_t value;
	value = (uint8_t)((pGPIOx->IDR >> PinNumber) & 0x00000001);
	return value;
}
uint16_t GPIO_ReadFromInputPort(GPIO_TypeDef *pGPIOx)
{
	uint16_t value;
	value = (uint16_t)(pGPIOx->IDR );
	return value;

}
void GPIO_WriteToOutputPin(GPIO_TypeDef *pGPIOx, uint8_t PinNumber ,uint8_t value)
{
	if(value == GPIO_PIN_SET)
	{
		pGPIOx->ODR |= (1<< PinNumber);
	}else
	{
		pGPIOx->ODR &= ~(1<< PinNumber);
	}
}
void GPIO_WriteToOutputPort(GPIO_TypeDef *pGPIOx ,uint16_t value)
{
	pGPIOx->ODR = value;
}
void GPIO_ToggleOutputPin(GPIO_TypeDef *pGPIOx, uint8_t PinNumber )
{
	pGPIOx->ODR ^= (1<<PinNumber);
}

/* IRQ configuration and ISR handling*/

void GPIO_IRQConfig(uint8_t IRQNumber , uint8_t IRQPriority , uint8_t EnorDi)
{
	    uint8_t iprx, iprx_section;
	    uint8_t shift_amount;

	    // Enable or Disable interrupt
	    if (EnorDi == ENABLE)
	    {
	        if (IRQNumber <= 31)
	        {
	            *NVIC_ISER0 |= (1 << IRQNumber);
	        }
	        else if (IRQNumber < 64)
	        {
	            *(NVIC_ISER0 + 1) |= (1 << (IRQNumber % 32));
	        }
	    }
	    else
	    {
	        if (IRQNumber <= 31)
	        {
	            *NVIC_ICER0 |= (1 << IRQNumber);
	        }
	        else if (IRQNumber < 64)
	        {
	            *(NVIC_ICER0 + 1) |= (1 << (IRQNumber % 32));
	        }
	    }

	    // Configure IRQ priority
	    iprx = IRQNumber / 4;
	    iprx_section = IRQNumber % 4;

	    shift_amount = (8 * iprx_section) + (4); // STM32 uses only top 4 bits
	    *(NVIC_IPR_BASE_ADDR + iprx) &= ~(0xF << shift_amount);   //Clear the target field in the register
	    *(NVIC_IPR_BASE_ADDR + iprx) |= (IRQPriority << shift_amount);
}
void GPIO_IRQHandling(uint8_t PinNumber)
{
	 // Check if the pending register bit is set for that pin
	 if (EXTI->PR & (1 << PinNumber))
	 {
	      // Clear it by writing 1 to it
	      EXTI->PR |= (1 << PinNumber);
	 }
}
