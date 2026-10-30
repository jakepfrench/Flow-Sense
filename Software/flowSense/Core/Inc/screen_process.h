/*
 * screen_process.h
 *
 *  Created on: Sep 27, 2026
 *      Author: jake
 */

#ifndef SCREEN_PROCESS_H
#define SCREEN_PROCESS_H
#include "programming.h"
typedef enum
{
    SCREEN_OPENING_TITLE,
    SCREEN_FLOW,
    SCREEN_TEMPERATURE,
    SCREEN_STATUS,
    SCREEN_QR,
	SCREEN_SLEEP,
} ScreenType;


extern volatile uint32_t buttonPressTime;
extern volatile uint8_t buttonPending;
extern volatile uint8_t buttonPressed;
extern volatile uint8_t buttonLocked;

//void Screen_Task(void);
//void Button_Process(void);
void Display_Splash(void);
void Process_Screen(ScreenType currentScreen);
void Display_Icon(const IconConfig *icon);


#endif




