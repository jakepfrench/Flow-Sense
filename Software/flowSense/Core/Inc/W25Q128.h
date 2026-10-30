/*
 * W25Q128.h
 *
 *  Created on: Oct 12th 2026
 *      Author: J. French
 */

#ifndef __W25Q64JV_H
#define __W25Q64JV_H

#include "main.h"
#include <stdint.h>


// CS Macros
#define W25Q_SELECT()    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET)
#define W25Q_DESELECT()  HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET)


// Function prototypes
uint8_t W25Q_DMA_IsBusy(void);
uint8_t W25Q_readID(void);
void W25Q_WakeUp(void);
void W25Q_Sleep(void);
void W25Q_writeEnable(void);
void W25Q_waitForWrite(void);
void W25Q_eraseChip(void);
void W25Q_eraseBlock32K(uint32_t block);
void W25Q_writeData(uint16_t pagenum, uint8_t *datatoWrite, uint32_t length);
void W25Q_readData(uint32_t memAddress, uint8_t *read_data, uint32_t length);
HAL_StatusTypeDef W25Q_writeData_DMA(uint16_t pagenum, uint8_t *datatoWrite, uint16_t length);
HAL_StatusTypeDef W25Q_readData_DMA(uint32_t memAddress, uint8_t *read_data, uint16_t length);
#endif
