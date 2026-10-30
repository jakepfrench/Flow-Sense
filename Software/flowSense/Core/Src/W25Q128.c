/*
 * W25Q128.c
 *
 *  Created on: Oct 12th 2026
 *      Author: J. French
 */
#include "main.h"
#include "W25Q128.h"
#include <string.h>

extern SPI_HandleTypeDef hspi2;
static volatile uint8_t w25q_dmaBusy = 0;
static volatile uint8_t w25q_dmaWritePending = 0;
static uint8_t w25q_dmaTxBuffer[260];


void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI2 && w25q_dmaWritePending)
    {
        W25Q_DESELECT();
        w25q_dmaWritePending = 0;
        w25q_dmaBusy = 0;
    }
}


void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI2 && w25q_dmaBusy)
    {
        W25Q_DESELECT();
        w25q_dmaBusy = 0;
    }
}

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI2 && w25q_dmaBusy)
    {
        W25Q_DESELECT();
        w25q_dmaWritePending = 0;
        w25q_dmaBusy = 0;
    }
}

uint8_t W25Q_DMA_IsBusy(void)
{
    return w25q_dmaBusy;
}




// Wake up
void W25Q_WakeUp(void)
{
    uint8_t cmd = 0xAB;  // Release from Power-Down (RES) command
    W25Q_SELECT();
    HAL_SPI_Transmit(&hspi2, &cmd, 1, HAL_MAX_DELAY);
    W25Q_DESELECT();
}

// Bedtime
void W25Q_Sleep(void)
{
    uint8_t cmd = 0xB9;
    W25Q_SELECT();
    HAL_SPI_Transmit(&hspi2, &cmd, 1, HAL_MAX_DELAY);
    W25Q_DESELECT();
}

// Read the chip ID of W25Q64J, should always return 0xef
uint8_t W25Q_readID(void)
{
	uint8_t rtn[3] = {0xff, 0xff, 0xff};
	uint8_t cmd = 0x9f;
	W25Q_SELECT();
	HAL_SPI_Transmit(&hspi2, &cmd, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi2, (uint8_t *)&rtn, 3, HAL_MAX_DELAY);
    W25Q_DESELECT();
    return rtn[0];
}



void W25Q_reset(void)
{
    uint8_t cmd;

    // Enable reset
    cmd = 0x66;
    W25Q_SELECT();
    HAL_SPI_Transmit(&hspi2, &cmd, 1, HAL_MAX_DELAY);
    W25Q_DESELECT();

    // Execute reset
    cmd = 0x99;
    W25Q_SELECT();
    HAL_SPI_Transmit(&hspi2, &cmd, 1, HAL_MAX_DELAY);
    W25Q_DESELECT();
    HAL_Delay(100);
}


// Erase the entire chip
void W25Q_eraseChip(void)
{
    uint8_t cmd = 0xC7;
    W25Q_writeEnable();
    W25Q_SELECT();
    HAL_SPI_Transmit(&hspi2, &cmd, 1, HAL_MAX_DELAY);
    W25Q_DESELECT();
    W25Q_waitForWrite();
}


// Reads the status register (0x05) and waits for it to clear, that's when the operation completes
void W25Q_waitForWrite(void)
{
    uint8_t cmd = 0x05;
    uint8_t status = 0;

    W25Q_SELECT();

    // Send Read Status Register command once
    HAL_SPI_Transmit(&hspi2, &cmd, 1, HAL_MAX_DELAY);

    // Keep reading until BUSY clears
    do {
        HAL_SPI_Receive(&hspi2, &status, 1, HAL_MAX_DELAY);
    } while (status & 0x01);

    W25Q_DESELECT();
}


// Enable the write process
void W25Q_writeEnable(void) {
	// Enable write command 0x06
	uint8_t cmd = 0x06;
	W25Q_SELECT();
	HAL_SPI_Transmit(&hspi2, &cmd, sizeof(cmd), HAL_MAX_DELAY);
    W25Q_DESELECT();
}



void W25Q_eraseBlock32K(uint32_t block)
{
    uint32_t memAddress = block * 32768;  // 32 KB per block

    uint8_t data[4];
    data[0] = 0x52;  // 32 KB block erase
    data[1] = (memAddress >> 16) & 0xFF;
    data[2] = (memAddress >> 8) & 0xFF;
    data[3] = memAddress & 0xFF;

    W25Q_writeEnable();
    W25Q_SELECT();
    HAL_SPI_Transmit(&hspi2, data, sizeof(data), HAL_MAX_DELAY);
    W25Q_DESELECT();
    W25Q_waitForWrite();
}


// W25Q_writeData function to write 256 bytes of data to a specific page - without DMA (blocking)
void W25Q_writeData(uint16_t pagenum, uint8_t *datatoWrite, uint32_t length)
{
    if (length == 0)
        return;

    if (length > 256)
        length = 256;

    uint32_t memAddress = (uint32_t)pagenum * 256;

    uint8_t data[260];

    data[0] = 0x02;  // Page Program
    data[1] = (memAddress >> 16) & 0xFF;
    data[2] = (memAddress >> 8) & 0xFF;
    data[3] = memAddress & 0xFF;

    memcpy(&data[4], datatoWrite, length);

    W25Q_writeEnable();

    W25Q_SELECT();
    HAL_SPI_Transmit(&hspi2, data, length + 4, HAL_MAX_DELAY);
    W25Q_DESELECT();

    W25Q_waitForWrite();
}


// W25Q_writeData function to write 256 bytes of data to a specific page - via DMA
HAL_StatusTypeDef W25Q_writeData_DMA(uint16_t pagenum, uint8_t *datatoWrite, uint16_t length)
{
    if (w25q_dmaBusy)
        return HAL_BUSY;

    if (datatoWrite == NULL || length == 0 || length > 256)
        return HAL_ERROR;

    uint32_t memAddress = (uint32_t)pagenum * 256;

    w25q_dmaTxBuffer[0] = 0x02;
    w25q_dmaTxBuffer[1] = (memAddress >> 16) & 0xFF;
    w25q_dmaTxBuffer[2] = (memAddress >> 8) & 0xFF;
    w25q_dmaTxBuffer[3] = memAddress & 0xFF;

    memcpy(&w25q_dmaTxBuffer[4], datatoWrite, length);

    W25Q_writeEnable();

    w25q_dmaBusy = 1;
    w25q_dmaWritePending = 1;

    W25Q_SELECT();

    HAL_StatusTypeDef status =
        HAL_SPI_Transmit_DMA(&hspi2, w25q_dmaTxBuffer, length + 4);

    if (status != HAL_OK)
    {
        W25Q_DESELECT();
        w25q_dmaWritePending = 0;
        w25q_dmaBusy = 0;
    }

    return status;
}




// read N bytes
void W25Q_readData(uint32_t memAddress, uint8_t *read_data, uint32_t length)
{
    uint8_t cmd[5] = {
        0x0B,
        (uint8_t)(memAddress >> 16),
        (uint8_t)(memAddress >> 8),
        (uint8_t)memAddress,
        0x00
    };

    W25Q_SELECT();

    HAL_SPI_Transmit(&hspi2, cmd, 5, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi2, read_data, length, HAL_MAX_DELAY);

    W25Q_DESELECT();
}


// read N bytes
HAL_StatusTypeDef W25Q_readData_DMA(uint32_t memAddress, uint8_t *read_data, uint16_t length)
{
    if (w25q_dmaBusy)
        return HAL_BUSY;

    if (read_data == NULL || length == 0)
        return HAL_ERROR;

    uint8_t cmd[5] = {
        0x0B,
        (uint8_t)(memAddress >> 16),
        (uint8_t)(memAddress >> 8),
        (uint8_t)memAddress,
        0x00
    };

    w25q_dmaBusy = 1;
    W25Q_SELECT();

    if (HAL_SPI_Transmit(&hspi2, cmd, 5, HAL_MAX_DELAY) != HAL_OK)
    {
        W25Q_DESELECT();
        w25q_dmaBusy = 0;
        return HAL_ERROR;
    }

    if (HAL_SPI_Receive_DMA(&hspi2, read_data, length) != HAL_OK)
    {
        W25Q_DESELECT();
        w25q_dmaBusy = 0;
        return HAL_ERROR;
    }

    return HAL_OK;
}
