#include "sd_lowlevel.h"

#include "stm32f10x_sdio.h"
#include "stm32f10x_dma.h"


// DeInitializes the SDIO interface
void SD_LowLevel_DeInit(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    SDIO_ClockCmd(DISABLE);                     // Disable SDIO Clock
    SDIO_SetPowerState(SDIO_PowerState_OFF);    // Set Power State to OFF
    SDIO_DeInit();                              // DeInitializes the SDIO peripheral
    // Disable the SDIO AHB Clock
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SDIO, DISABLE);
    // Configure PC.08, PC.09, PC.10, PC.11, PC.12 pin: D0, D1, D2, D3, CLK pin
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    // Configure PD.02 CMD line
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

// Initializes the SD Card and put it into StandBy State (Ready for data transfer)
void SD_LowLevel_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    // GPIOC and GPIOD Periph clock enable
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
    // Configure PC.08, PC.09, PC.10, PC.11, PC.12 pin: D0, D1, D2, D3, CLK pin
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    // Configure PD.02 CMD line
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    // Enable the SDIO AHB Clock
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SDIO, ENABLE);
    // Enable the DMA2 Clock
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
}

// Configures the DMA2 Channel4 for SDIO Tx request.
void SD_LowLevel_DMA_TxConfig(uint32_t *BufferSRC, uint32_t BufferSize) {
    DMA_InitTypeDef DMA_InitStructure;
    DMA_ClearFlag(DMA2_FLAG_TC4 | DMA2_FLAG_TE4 | DMA2_FLAG_HT4 | DMA2_FLAG_GL4);
    // DMA2 Channel4 disable
    DMA_Cmd(DMA2_Channel4, DISABLE);
    // DMA2 Channel4 Config
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) SDIO_FIFO_ADDRESS;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) BufferSRC;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = BufferSize / 4;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA2_Channel4, &DMA_InitStructure);
    // DMA2 Channel4 enable
    DMA_Cmd(DMA2_Channel4, ENABLE);
}

// Configures the DMA2 Channel4 for SDIO Rx request.
void SD_LowLevel_DMA_RxConfig(uint32_t *BufferDST, uint32_t BufferSize) {
    DMA_InitTypeDef DMA_InitStructure;
    DMA_ClearFlag(DMA2_FLAG_TC4 | DMA2_FLAG_TE4 | DMA2_FLAG_HT4 | DMA2_FLAG_GL4);
    // DMA2 Channel4 disable
    DMA_Cmd(DMA2_Channel4, DISABLE);
    // DMA2 Channel4 Config
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) SDIO_FIFO_ADDRESS;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) BufferDST;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = BufferSize / 4;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA2_Channel4, &DMA_InitStructure);
    // DMA2 Channel4 enable
    DMA_Cmd(DMA2_Channel4, ENABLE);
}

// Returns the DMA End Of Transfer Status
uint32_t SD_DMAEndOfTransferStatus(void) {
    return (uint32_t) DMA_GetFlagStatus(DMA2_FLAG_TC4);
}

