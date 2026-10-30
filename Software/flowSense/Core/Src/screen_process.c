/*
 * screen_process.c
 *
 *  Created on: Sep 27, 2026
 *      Author: jake
 */
#include "screen_process.h"
#include "LCD_1in28.h"
#include "GUI_Paint.h"
#include "temperature.h"
#include "phase.h"
#include "programming.h"
#include "W25Q128.h"
#include <string.h>

extern SPI_HandleTypeDef hspi1;
extern volatile ScreenType currentScreen;

void Display_Splash(void)
{
    LCD_1IN28_Clear(BLACK);
    Display_Icon(&IconConfigscreens[0]);
    HAL_Delay(3000);
}


void Display_Icon(const IconConfig *icon)
{
    uint16_t w = icon->width;
    uint16_t h = icon->height;
    uint16_t x = icon->x;
    uint16_t y = icon->y;

    uint32_t startAddress = icon->iconAddress;
    uint8_t row[480];

    for (uint16_t i = 0; i < h; i++)
    {
        uint32_t address = startAddress + (uint32_t)i * w * 2;

        if (W25Q_readData_DMA(address, row, w * 2) == HAL_OK)
        {
            // Wait for flash SPI2 DMA
            while (W25Q_DMA_IsBusy())
            {}

            // Send row to LCD using SPI1 DMA
            LCD_1IN28_DisplayRGB565_DMA(x, y + i, w, 1, row);

            // Wait before reusing row buffer
            while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY)
            {}
        }
    }
}

/*
void Button_Process(void)
{
    if (buttonPending && (HAL_GetTick() - buttonPressTime >= 50))
    {
        buttonPending = 0;
        if (HAL_GPIO_ReadPin(BTN_GPIO_Port, BTN_Pin) == GPIO_PIN_RESET)
            buttonPressed = 1;
    }

    if (!buttonPending && HAL_GPIO_ReadPin(BTN_GPIO_Port, BTN_Pin) == GPIO_PIN_SET)
    {
        buttonLocked = 0;
    }
}



void Screen_Task(void)
{
    if (buttonPressed)
    {
        buttonPressed = 0;
        currentScreen++;
        if (currentScreen >= SCREEN_COUNT)
            currentScreen = SCREEN_FLOW;

        Process_Screen(currentScreen);
    }
}
*/

void Process_Screen(ScreenType currentScreen)
{
	IconConfig *ptr = &IconConfigscreens[15];  // contingency
    char txt[32] = "----";
    switch(currentScreen)
    {
        case SCREEN_FLOW:
        	if( Phase_Read(500) )
        	{
        		ptr = &IconConfigscreens[1];
        		strcpy(txt, "Flow");
        	}
        	else
        	{
        		ptr = &IconConfigscreens[2];
        		strcpy(txt, "----");
        	}
            break;

        case SCREEN_TEMPERATURE:
        	ptr = &IconConfigscreens[3];
        	float temperature = Temperature_ReadValue();
        	if (temperature == 1000.0f)
        		snprintf(txt, sizeof(txt), "Error");
        	else
        	{
        	    int temp10 = (int)(temperature * 10.0f);
        	    int whole = temp10 / 10;
        	    int decimal = temp10 % 10;
        	    snprintf(txt, sizeof(txt), "%d.%d%cC", whole, decimal, 0x7F);
        	}
            break;

        case SCREEN_STATUS:
        	uint8_t idx = Battery_Temporary();
        	ptr = &IconConfigscreens[idx];
            snprintf(txt, sizeof(txt), "%d %%", (idx - 3) * 10);
            break;

        case SCREEN_QR:
        	ptr = &IconConfigscreens[14];
        	snprintf(txt, sizeof(txt), "Help");
        	break;

        case SCREEN_SLEEP:
            ptr = &IconConfigscreens[16];
            snprintf(txt, sizeof(txt), "Sleep");
            break;
        default:
            break;
    }
	LCD_1IN28_Clear(BLACK);
	Display_Icon(ptr);
    Paint_DrawString_EN(
        80,             // X
        150,            // Y - below the 120x120 icon
        txt,
        &Font24,
        BLACK,
        WHITE
    );
}



