#include "misc.h"

#include "sd.h"
#include "uart.h"

#include "sd_lowlevel.h"

#define BLOCK_SIZE            512 /* Block Size in Bytes */
#define NUMBER_OF_BLOCKS      4  /* For Multi Blocks operation (Read/Write) */
#define MULTI_BUFFER_SIZE   1024

uint8_t Buffer_Rx[MULTI_BUFFER_SIZE];

sd_t SD;

void sd_t::Init() {
    // NVIC configuration
//    NVIC_InitTypeDef NVIC_InitStructure;
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
//    NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);

    // Init SD
    Status = SD_Init();
    if (Status != SD_OK) {
        UART_StrInt("SD init error: ", Status);
        return;
    }
    else UART_PrintString("Crd on\r");

//    UART_StrInt("DIV: ", SDIO_TRANSFER_CLK_DIV);

    Status = SD_ReadBlock(Buffer_Rx, 0x00, 512);
    UART_StrInt("Read State: ", Status);
//    Uart.PrintString("SD2\r");
//    Uart.PrintUint(Status);
//    Uart.NewLine();
//
//    Status = SD_WaitReadOperation();
//    Uart.PrintString("SD3\r");
//    Uart.PrintUint(Status);
//    Uart.NewLine();

//    SDTransferState tst;
//    do {
//        tst = SD_GetStatus();
//    }
//    while(tst != SD_TRANSFER_OK);
//
//    Uart.PrintString("SD4\r");
//    Uart.PrintArr(Buffer_Rx, BLOCK_SIZE);

//    Uart.PrintString("SD1\r");
//    Status = SD_ReadMultiBlocks(Buffer_MultiBlock_Rx, 0x00, BLOCK_SIZE, NUMBER_OF_BLOCKS);
//    Uart.PrintString("SD2\r");
//    Uart.PrintUint(Status);
//    Uart.NewLine();
//    Status = SD_WaitReadOperation();
//    Uart.PrintString("SD3\r");
//    Uart.PrintUint(Status);
//    Uart.NewLine();
//    while(SD_GetStatus() != SD_TRANSFER_OK);
//    Uart.PrintString("SD4\r");
//    Uart.PrintArr(Buffer_MultiBlock_Rx, MULTI_BUFFER_SIZE);
}

// ============================= Interrupts ====================================
void SDIO_IRQHandler(void) {
    UART_PrintString("SDIO IRQ\r");
  /* Process All SDIO Interrupt Sources */
  SD_ProcessIRQSrc();
}