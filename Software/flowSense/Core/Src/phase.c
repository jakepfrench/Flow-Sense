/*
 * temperature.c
 *
 *  Created on: Oct 4, 2026
 *  Author: Dr French
 *  Reference: https://www.handsontec.com/dataspecs/sensor/NTC-10K-3950-HT.pdf
 */
#include "phase.h"
#include "GUI_Paint.h"
#include <math.h>
#include <stdio.h>

extern ADC_HandleTypeDef hadc1;



uint32_t Read_ADC(void)
{
    // Variables
    uint32_t adc_value = 0;

    // Configure ADC1 to read phase (PA1 / ADC_CHANNEL_1)
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = ADC_CHANNEL_1;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;

    // Apply ADC channel configuration
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        return 0xFFFFFFFF;
    }

    // Start ADC
    if (HAL_ADC_Start(&hadc1) != HAL_OK)
    {
        HAL_ADC_Stop(&hadc1);
        return 0xFFFFFFFF;
    }

    // Wait for ADC conversion
    if (HAL_ADC_PollForConversion(&hadc1, 100) != HAL_OK)
    {
        HAL_ADC_Stop(&hadc1);
        return 0xFFFFFFFF;
    }

    // Read ADC value, from 0 to 4095 (2^12)
    adc_value = HAL_ADC_GetValue(&hadc1);

    // Stop ADC
    HAL_ADC_Stop(&hadc1);


    return adc_value;
}


uint8_t Phase_Read(uint16_t threshold)
{
	uint8_t rtn;
	uint16_t adc_value = Read_ADC();
	if( adc_value < threshold)
		rtn = 0;
	else
		rtn = 1;
	return rtn;
}

uint8_t Battery_Temporary(void)
{
    uint32_t adc_value = Read_ADC();

    if (adc_value == 0xFFFFFFFF)
        return 15;

    if (adc_value <= 403)  return 4;
    if (adc_value <= 808)  return 5;
    if (adc_value <= 1212) return 6;
    if (adc_value <= 1616) return 7;
    if (adc_value <= 2020) return 8;
    if (adc_value <= 2424) return 9;
    if (adc_value <= 2828) return 10;
    if (adc_value <= 3232) return 11;
    if (adc_value <= 3636) return 12;

    return 13;
}
