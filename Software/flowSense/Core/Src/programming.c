/*
 * programming.c
 *
 *  Created on: Oct 11, 2026
 *      Author: jake
 */
#include "programming.h"
#include "GUI_Paint.h"
#include "fonts.h"
#include "W25Q128.h"
#include "usbd_cdc_if.h"
#include "LCD_1in28.h"
#include "screen_process.h"

volatile uint8_t demo_mode = 0;
volatile ProgrammingState programming_state = PROGRAM_START;
volatile uint8_t programming_trigger = 0;
volatile uint8_t program_buffer_ready = 0;
volatile uint16_t program_buffer_count = 0;
uint8_t program_buffer[PROGRAM_BUFFER_SIZE];
IconConfig current_icon;




IconConfig IconConfigscreens[17] =
{
    { 0,  ICON_SPLASH_ADD,        "Opening title", 240, 240,  0,  0 },
    { 1,  ICON_FLOW_ON_ADD,       "Flow ON",       120, 120, 60, 20 },
    { 2,  ICON_FLOW_OFF_ADD,      "Flow OFF",      120, 120, 60, 20 },
    { 3,  ICON_TEMP_ADD,          "Temperature",   120, 120, 60, 20 },
    { 4,  ICON_STATUS_10PC_ADD,   "Battery 10%",   200, 120, 20, 25 },
    { 5,  ICON_STATUS_20PC_ADD,   "Battery 20%",   200, 120, 20, 25 },
    { 6,  ICON_STATUS_30PC_ADD,   "Battery 30%",   200, 120, 20, 25 },
    { 7,  ICON_STATUS_40PC_ADD,   "Battery 40%",   200, 120, 20, 25 },
    { 8,  ICON_STATUS_50PC_ADD,   "Battery 50%",   200, 120, 20, 25 },
    { 9,  ICON_STATUS_60PC_ADD,   "Battery 60%",   200, 120, 20, 25 },
    { 10, ICON_STATUS_70PC_ADD,   "Battery 70%",   200, 120, 20, 25 },
    { 11, ICON_STATUS_80PC_ADD,   "Battery 80%",   200, 120, 20, 25 },
    { 12, ICON_STATUS_90PC_ADD,   "Battery 90%",   200, 120, 20, 25 },
    { 13, ICON_STATUS_100PC_ADD,  "Battery 100%",  200, 120, 20, 25 },
    { 14, ICON_HELP_ADD,          "Help site",     120, 120, 60, 25 },
    { 15, ICON_FLASH_ADD,         "Flash memory",  120, 120, 60, 25 },
    { 16, ICON_SLEEP_ADD,         "Sleep",  	   120, 120, 60, 25 }
};






void Programming_Process(uint8_t *Buf, uint32_t *Len)
{
    if (HAL_GPIO_ReadPin(BTN_PROG_GPIO_Port, BTN_PROG_Pin) == GPIO_PIN_SET)
    {
        CDC_Transmit_FS((uint8_t *)"X", 1);
        return;
    }

	switch (programming_state)
	{
	    case PROGRAM_START:
	        if (*Len == 1)
	        {
	        	if( Buf[0] == 'p')
	        	{
	        		programming_trigger = 1;
	        		programming_state = PROGRAM_SELECT_ICON;
	        		CDC_Transmit_FS((uint8_t *)"R", 1);
	        	}
	        	else
	        	{
	        		demo_mode = 1;
	        		CDC_Transmit_FS((uint8_t *)"DEMO MODE", 10);
	        	}

	        }
	        else
	        	programming_state = PROGRAM_START;
	        break;

	    case PROGRAM_SELECT_ICON:
	        if (*Len == 1 && Buf[0] < 18)
	        {
	            current_icon = IconConfigscreens[Buf[0]];
	            programming_trigger = 1;
	            program_buffer_ready = 0;
	            program_buffer_count = 0;
	            programming_state = PROGRAM_RECEIVE;
	            CDC_Transmit_FS((uint8_t *)"R", 1);
	        }
	        else
	        {
	            programming_state = PROGRAM_START;
	            CDC_Transmit_FS((uint8_t *)"X", 1);
	        }
	        break;

	    case PROGRAM_RECEIVE:
	        if (!program_buffer_ready)
	        {
	            for (uint32_t i = 0; i < *Len; i++)
	            {
	                if (program_buffer_count < PROGRAM_BUFFER_SIZE)
	                	program_buffer[program_buffer_count++] = Buf[i];

	                if (program_buffer_count == PROGRAM_BUFFER_SIZE)
	                {
	                    program_buffer_ready = 1;
	                    break;
	                }
	            }
	        }
	        break;

	    case PROGRAM_STOP:
	    default:
	        break;
	}
}





void Programming_Task(void)
{
    static uint16_t page = 0;
    static uint32_t startAddress = 0;
    static uint16_t totalPages = 0;

    if (programming_trigger)
    {
        programming_trigger = 0;

        if (programming_state == PROGRAM_RECEIVE)
        {
            page = 0;
            startAddress = current_icon.iconAddress;
            totalPages = ((uint32_t)current_icon.width * current_icon.height * 2 + 255) / 256;
        }

        if (programming_state == PROGRAM_RECEIVE)
        {
        	LCD_1IN28_Clear(BLACK);
  		    Display_Icon(&IconConfigscreens[15]);
            Paint_DrawString_EN(
                10, 130,
				current_icon.iconName,
                &Font24, BLACK, WHITE
            );
        }
    }

    if (program_buffer_ready)
    {
        // Erase flash before writing first page
        if (page == 0)
        {
            uint32_t imageBytes = (uint32_t)current_icon.width * current_icon.height * 2;
            for (uint32_t offset = 0; offset < imageBytes; offset += 32768)
            	W25Q_eraseBlock32K((startAddress + offset) / 32768);
        }

        // Flash driver expects a 256-byte page index
        uint32_t flashPage = (startAddress / 256) + page;

        if (W25Q_writeData_DMA(flashPage, program_buffer, 256) == HAL_OK)
        {
            // Wait for SPI2 DMA
            while (W25Q_DMA_IsBusy()) {}

            // Wait for W25Q128 internal programming
            W25Q_waitForWrite();

            page++;

            program_buffer_count = 0;
            program_buffer_ready = 0;

            if (page < totalPages)
            {
                CDC_Transmit_FS((uint8_t *)"R", 1);
            }
            else
            {
                programming_state = PROGRAM_START;
                CDC_Transmit_FS((uint8_t *)"S", 1);

                Paint_Clear(BLACK);
        		Display_Icon(&IconConfigscreens[15]);
                Paint_DrawString_EN(
                    30, 130,
                    "Completed",
                    &Font24, BLACK, WHITE
                );
                HAL_Delay(3000);
                LCD_1IN28_Clear(BLACK);
            }
        }
    }
}





