#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

// ============================= Defines =======================================
// SD Detect pin
#define SD_DETECT_PIN                    GPIO_Pin_6
#define SD_DETECT_GPIO_PORT              GPIOC
#define SD_DETECT_GPIO_CLK               RCC_APB2Periph_GPIOC


// SDIO Intialization Frequency (400KHz max)
#define SDIO_INIT_CLK_DIV                ((uint8_t)0xB2)
// SDIO Data Transfer Frequency (25MHz max)
#define SDIO_TRANSFER_CLK_DIV            ((uint8_t)4)

#define SDIO_FIFO_ADDRESS                ((uint32_t)0x40018080)


// ========================== Prototypes =======================================
void SD_LowLevel_DeInit(void);
void SD_LowLevel_Init(void);
void SD_LowLevel_DMA_TxConfig(uint32_t *BufferSRC, uint32_t BufferSize);
void SD_LowLevel_DMA_RxConfig(uint32_t *BufferDST, uint32_t BufferSize);
uint32_t SD_DMAEndOfTransferStatus(void);


