/*
 * stm32f103xx_adc_driver.h
 *
 *  Created on: Nov 21, 2025
 *      Author: Rana Samir
 */

#ifndef INC_STM32F103XX_ADC_DRIVER_H_
#define INC_STM32F103XX_ADC_DRIVER_H_


#include "stm32f103xx.h"
#include <stdint.h>

/*
 * ADC Configuration structure
 */
typedef struct
{
    uint8_t   ADC_Channel;        // Channel number (0 → 17)
    uint8_t   ADC_SampleTime;     // 1.5, 7.5, 13.5 ... cycles
    uint8_t   ADC_ContinuousConv; // 0 = single, 1 = continuous
    uint8_t   ADC_Prescaler;      // 2,4,6,8
    uint8_t   scanMode;          // 0 = OFF, 1 = ON
    uint8_t  dataAlignment;       // 0 = right, 1 = left
    uint8_t  numOfConversions; // Number of channels in regular sequence
    uint8_t   sequence[16];       //for scan mode
} ADC_Config_t;


/*
 * ADC Handle
 */
typedef struct
{
    ADC_TypeDef   *pADCx;        // ADC1 or ADC2
    ADC_Config_t   ADC_Config;
} ADC_Handle_t;

// ADC channel numbers
#define ADC_CHANNEL_0      0
#define ADC_CHANNEL_1      1
#define ADC_CHANNEL_2      2
#define ADC_CHANNEL_3      3
#define ADC_CHANNEL_4      4
#define ADC_CHANNEL_5      5
#define ADC_CHANNEL_6      6
#define ADC_CHANNEL_7      7
#define ADC_CHANNEL_8      8
#define ADC_CHANNEL_9      9
#define ADC_CHANNEL_10     10
#define ADC_CHANNEL_11     11
#define ADC_CHANNEL_12     12
#define ADC_CHANNEL_13     13
#define ADC_CHANNEL_14     14
#define ADC_CHANNEL_15     15
#define ADC_CHANNEL_16     16
#define ADC_CHANNEL_17     17

// ADC sampling times (3-bit values for SMPRx)
#define ADC_SAMPLE_1_CYCLE      0x0   // 1.5 cycles
#define ADC_SAMPLE_7_CYCLES     0x1   // 7.5 cycles
#define ADC_SAMPLE_13_CYCLES    0x2   // 13.5 cycles
#define ADC_SAMPLE_28_CYCLES    0x3   // 28.5 cycles
#define ADC_SAMPLE_41_CYCLES    0x4   // 41.5 cycles
#define ADC_SAMPLE_55_CYCLES    0x5   // 55.5 cycles
#define ADC_SAMPLE_71_CYCLES    0x6   // 71.5 cycles
#define ADC_SAMPLE_239_CYCLES   0x7   // 239.5 cycles

#define ADC_PRESCALER_2   0
#define ADC_PRESCALER_4   1
#define ADC_PRESCALER_6   2
#define ADC_PRESCALER_8   3


#define ADC_SCAN_DISABLE 0
#define ADC_SCAN_ENABLE 1

#define ADC_Single_Conversion 0
#define ADC_Contin_Conversion 1

#define ADC_Right_D_ALignment 0
#define ADC_Left_D_ALignment  1

void ADC_PeriClockControl(ADC_TypeDef *pADCx, uint8_t EnOrDi);
void ADC_Init(ADC_Handle_t *pADCHandle);

void ADC_StartConversion(ADC_TypeDef *pADCx);
uint16_t ADC_ReadValue(ADC_TypeDef *pADCx);

void ADC_Enable(ADC_TypeDef *pADCx);
void ADC_Calibrate(ADC_TypeDef *pADCx);

void ADC_EnableScanMode(ADC_TypeDef *pADCx, uint8_t enable);
void ADC_SetSequence(ADC_TypeDef *pADCx, uint8_t *channels, uint8_t length);

#endif /* INC_STM32F103XX_ADC_DRIVER_H_ */
