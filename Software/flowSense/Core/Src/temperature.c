/*
 * temperature.c
 *
 *  Created on: Oct 4, 2026
 *  Author: Dr French
 *  Reference: https://www.handsontec.com/dataspecs/sensor/NTC-10K-3950-HT.pdf
 */
#include "temperature.h"
#include "GUI_Paint.h"
#include <math.h>
#include <stdio.h>

extern ADC_HandleTypeDef hadc1;





float Temperature_ReadValue(void)
{
    // Variables
    uint32_t adc_value = 0;
    float resistance = 0.0f;
    float temperature = 0.0f;
    float ln_value = 0.0f;

    // Configure ADC1 to read temperature (PA0 / ADC_CHANNEL_0)
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = ADC_CHANNEL_0;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;

    // Apply ADC channel configuration
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        return 1000.0f;
    }

    // Start ADC
    if (HAL_ADC_Start(&hadc1) != HAL_OK)
    {
        HAL_ADC_Stop(&hadc1);
        return 1000.0f;
    }

    // Wait for ADC conversion
    if (HAL_ADC_PollForConversion(&hadc1, 100) != HAL_OK)
    {
        HAL_ADC_Stop(&hadc1);
        return 1000.0f;
    }

    // Read ADC value, from 0 to 4095 (2^12)
    adc_value = HAL_ADC_GetValue(&hadc1);

    // Stop ADC
    HAL_ADC_Stop(&hadc1);


    // Belt and braces
    if ((adc_value == 0) || (adc_value > 4000))
    {
        // Possible probe short circuit, open circuit or disconnected
        return 1000.0f;
    }

    // Convert ADC value to NTC resistance (ohms)
    // Fixed potential-divider resistor = 10k
    resistance = 10000.0f * ((float)adc_value / (4095.0f - (float)adc_value));

    // Natural log of resistance ratio: ln(R / R25)
    // R25 = 10k
    ln_value = logf(resistance / 10000.0f);
    //ln_value = FastLn(resistance / 10000.0f);

    // Polynomial fit: T = ax² + bx + c
    temperature = (1.71f * ln_value * ln_value) - (23.48f * ln_value) + 24.35f;
    return temperature;
}



void Temperature_Process()
{
	char temp_string[16];
	float temperature = Temperature_ReadValue();

	if (temperature == 1000.0f)
	{
	    snprintf(temp_string, sizeof(temp_string), "Error");
	}
	else
	{
	    int temp10 = (int)(temperature * 10.0f);
	    int whole = temp10 / 10;
	    int decimal = temp10 % 10;

	    snprintf(temp_string, sizeof(temp_string),
	             "%d.%d",
	             whole,
	             decimal);
	}

	// Number
	Paint_DrawString_EN(
	    65,
	    150,
	    temp_string,
	    &Font24,
	    BLACK,
	    WHITE
	);

	// Raised "o" to look like °
	Paint_DrawCircle(
	    157,        // move right
	    153,
	    2,          // smaller circle
	    WHITE,
	    DOT_PIXEL_1X1,
	    DRAW_FILL_EMPTY
	);

	Paint_DrawString_EN(
	    163,
	    150,
	    "C",
	    &Font24,
	    BLACK,
	    WHITE
	);
}
