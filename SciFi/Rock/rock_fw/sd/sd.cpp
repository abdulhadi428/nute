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
        UART_StrUint("SD init error: ", Status);
        return;
    } else UART_PrintString("Crd on\r");


            Status = SD_ReadBlock(Buffer_Rx, (0x0083 * 512), 512);
            if (Status == SD_OK) {
                for (uint32_t i = 0; i < 512; i++) {
                    UART_PrintAsHex(Buffer_Rx[i]);
                    UART_Print(' ');
                }
                UART_NewLine();
            }
            else UART_PrintString("err");
}

// ============================= Interrupts ====================================

void SDIO_IRQHandler(void) {
    UART_PrintString("SDIO IRQ\r");
    /* Process All SDIO Interrupt Sources */
    SD_ProcessIRQSrc();
}