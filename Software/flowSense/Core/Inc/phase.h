/*
 * phase.h
 *
 *  Created on: Oct 9, 2026
 *      Author: J. French
 */

#ifndef INC_PHASE_H_
#define INC_PHASE_H_


#ifndef PHASE_H
#define PHASE_H

#include "main.h"

uint32_t Read_ADC(void);
void Phase_Process(void);
uint8_t Phase_Read(uint16_t threshold);
uint8_t Battery_Temporary(void);


#endif
#endif /* INC_PHASE_H_ */
