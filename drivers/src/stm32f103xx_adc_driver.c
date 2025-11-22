/*
 * stm32f103xx_adc_driver.c
 *
 *  Created on: Nov 21, 2025
 *      Author: Rana Samir
 */

#include "stm32f103xx_adc_driver.h"


void ADC_PeriClockControl(ADC_TypeDef *pADCx, uint8_t EnOrDi)
{
    if (EnOrDi == ENABLE)
    {
        if (pADCx == ADC1) RCC->APB2ENR |= (1 << 9);
        else if (pADCx == ADC2) RCC->APB2ENR |= (1 << 10);
    }
    else
    {
        if (pADCx == ADC1) RCC->APB2ENR &= ~(1 << 9);
        else if (pADCx == ADC2) RCC->APB2ENR &= ~(1 << 10);
    }
}


void ADC_Init(ADC_Handle_t *pADCHandle)
{
    // 1. Enable ADC clock
    ADC_PeriClockControl(pADCHandle->pADCx, ENABLE);

    // 2. Set ADC prescaler
    RCC->CFGR &= ~(3 << 14);
    RCC->CFGR |=  (pADCHandle->ADC_Config.ADC_Prescaler << 14);

    // 3. Enable/Disable scan mode
    ADC_EnableScanMode(pADCHandle->pADCx, pADCHandle->ADC_Config.scanMode);

    // 4. Data alignment
    if (pADCHandle->ADC_Config.dataAlignment)
        pADCHandle->pADCx->CR2 |=  (1 << 11);   // Left
    else
        pADCHandle->pADCx->CR2 &= ~(1 << 11);   // Right

    pADCHandle->pADCx->CR2 &= ~(1 << 20);

    // 5. Configure sampling times for all channels used
    // (single or multi)
    uint8_t smp = pADCHandle->ADC_Config.ADC_SampleTime;

    if (!pADCHandle->ADC_Config.scanMode)
    {

        // Single Channel

        uint8_t ch = pADCHandle->ADC_Config.ADC_Channel;

        if (ch <= 9)
        {
            pADCHandle->pADCx->SMPR2 &= ~(7 << (ch * 3));
            pADCHandle->pADCx->SMPR2 |=  (smp << (ch * 3));
        }
        else
        {
            uint8_t c = ch - 10;
            pADCHandle->pADCx->SMPR1 &= ~(7 << (c * 3));
            pADCHandle->pADCx->SMPR1 |=  (smp << (c * 3));
        }

        // Sequence = 1 conversion
        pADCHandle->pADCx->SQR1 &= ~(0xF << 20); // L = 0
        pADCHandle->pADCx->SQR3 &= ~0x1F;
        pADCHandle->pADCx->SQR3 |=  ch;
    }
    else
    {

        // Multi-channel Scan Mode


        // Clear previous sequence
        pADCHandle->pADCx->SQR1 &= ~(0x000FFFFF);
        pADCHandle->pADCx->SQR2 = 0;
        pADCHandle->pADCx->SQR3 = 0;

        // Fill sequence using function
        ADC_SetSequence(
            pADCHandle->pADCx,
            pADCHandle->ADC_Config.sequence,
            pADCHandle->ADC_Config.numOfConversions
        );

        // Set sample times for all channels in sequence
        for (uint8_t i = 0; i < pADCHandle->ADC_Config.numOfConversions; i++)
        {
            uint8_t ch = pADCHandle->ADC_Config.sequence[i];

            if (ch <= 9)
            {
                pADCHandle->pADCx->SMPR2 &= ~(7 << (ch * 3));
                pADCHandle->pADCx->SMPR2 |=  (smp << (ch * 3));
            }
            else
            {
                uint8_t c = ch - 10;
                pADCHandle->pADCx->SMPR1 &= ~(7 << (c * 3));
                pADCHandle->pADCx->SMPR1 |=  (smp << (c * 3));
            }
        }
    }

    // 6. Continuous mode
    if (pADCHandle->ADC_Config.ADC_ContinuousConv)
        pADCHandle->pADCx->CR2 |=  (1 << 1);
    else
        pADCHandle->pADCx->CR2 &= ~(1 << 1);


}



void ADC_Enable(ADC_TypeDef *pADCx)
{
    pADCx->CR2 |= (1 << 0);  // ADON = 1
    for (volatile int i = 0; i < 1000; i++);
}


void ADC_StartConversion(ADC_TypeDef *pADCx)
{
    pADCx->CR2 |= (1 << 22); // SWSTART
}


uint16_t ADC_ReadValue(ADC_TypeDef *pADCx)
{
    // Added timeout to prevent infinite hang if clock is missing
    uint32_t timeout = 50000;
    while (!(pADCx->SR & (1 << 1)) && timeout > 0)
    {
        timeout--;
    }

    // Clear the Flag (Stops weird loops in Proteus)
    pADCx->SR &= ~(1 << 1);

    return (uint16_t)pADCx->DR;
}

void ADC_EnableScanMode(ADC_TypeDef *pADCx, uint8_t enable)
{
    if (enable)
        pADCx->CR1 |= (1 << 8);
    else
        pADCx->CR1 &= ~(1 << 8);
}
void ADC_SetSequence(ADC_TypeDef *pADCx, uint8_t *channels, uint8_t length)
{
    // length must be 1 → 16
    if (length == 0 || length > 16)
        return;

    // Set number of conversions in SQR1[L]
    pADCx->SQR1 &= ~(0xF << 20);
    pADCx->SQR1 |= ((length - 1) << 20);

    // Clear all SQR registers
    pADCx->SQR1 &= ~(0xFFF);
    pADCx->SQR2 = 0;
    pADCx->SQR3 = 0;

    for (uint8_t i = 0; i < length; i++)
    {
        if (i < 6)
        {
            pADCx->SQR3 |= (channels[i] << (5 * i));
        }
        else if (i < 12)
        {
            pADCx->SQR2 |= (channels[i] << (5 * (i - 6)));
        }
        else
        {
            pADCx->SQR1 |= (channels[i] << (5 * (i - 12)));
        }
    }
}


void ADC_Calibrate(ADC_TypeDef *pADCx)
{

    pADCx->CR2 |= (1 << 3);   // Reset calibration
    while (pADCx->CR2 & (1 << 3));

    pADCx->CR2 |= (1 << 2);   // Start calibration
    while (pADCx->CR2 & (1 << 2));
}


