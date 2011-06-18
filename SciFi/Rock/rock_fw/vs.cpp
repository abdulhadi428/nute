#include "vs.h"

#include "uart.h"
#include "i2c_mgr.h"
#include "acc_mma.h"
#include "media.h"

VS_t Vs;

void VS_t::WriteData(uint8_t* ABuf, uint16_t ACount) {
    if(ACount == 0) return;
    if(BusyWait() != VS_OK) return;     // Get out in case of timeout
    if(ACount == 1) {                   // Do not use DMA
        XDCS_Lo();                      // Start transmission
        ReadWriteByte(ABuf[0]);         // Send data
        XDCS_Hi();                      // End transmission
    }
    else {
        XDCS_Lo();
        for(uint8_t i=0; i<ACount; i++)
            ReadWriteByte(ABuf[i]);
        XDCS_Hi();

        // Setup DMA
//        DMA_Busy = true;
//        DMA_DeInit(DMA1_Channel3);
//        DMA_InitTypeDef DMA_InitStructure;
//        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;
//        DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ABuf;
//        DMA_InitStructure.DMA_BufferSize = ACount;
//        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
//        DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//        DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // 8 bit
//        DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;   // 8 bit
//        DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
//        DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
//        DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
//        DMA_Init(DMA1_Channel3, &DMA_InitStructure);
//        //Enable SPI request
//        SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);        // Enable DMA1 Channel3 Transfer Complete interrupt
//        DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);
//        // Start transmission
//        XDCS_Lo();
//        DMA_Cmd(DMA1_Channel3, ENABLE);
    }
}
void VS_t::WriteTrailingZeroes() {
    if(BusyWait() != VS_OK) return;     // Get out in case of timeout
    XDCS_Lo();                          // Start transmission
    for (uint8_t i=0; i<VS_TRAILING_0_COUNT; i++)
        ReadWriteByte(0);               // Send data
    XDCS_Hi();                          // End transmission
}

// ============================ Init etc. ======================================
void VS_t::Init() {
    // ==== Clocks init ====
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1 | RCC_APB2Periph_AFIO, ENABLE);
    // ==== GPIO init ====
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    // Configure VS_XCS, VS_XDCS, VS_RST as Push-Pull output
    GPIO_InitStructure.GPIO_Pin  = VS_XCS | VS_XDCS | VS_RST;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    this->Rst_Lo();
    this->XCS_Hi();
    this->XDCS_Hi();
    // Configure VS_DREQ as floating input
    GPIO_InitStructure.GPIO_Pin  = VS_DREQ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // Configure VS_SI & VS_XCLK as Alternate Function Push Pull
    GPIO_InitStructure.GPIO_Pin  = VS_XCLK | VS_SI;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // Configure MISO as Input Floating
    GPIO_InitStructure.GPIO_Pin  = VS_SO;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // ==== SPI init ====    MSB first, master, SCK idle low
    SPI_InitTypeDef SPI_InitStructure;
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode      = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize  = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL      = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA      = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS       = SPI_NSS_Soft;
    SPI_InitStructure.SPI_FirstBit  = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_Init(SPI1, &SPI_InitStructure);
    SPI_Cmd(SPI1, ENABLE);

    // ==== NVIC configuration ====
    NVIC_InitTypeDef NVIC_InitStructure;
    // Enable DMA1 channel3 IRQ Channel
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    DMA_Busy = false;
}

void VS_t::Enable() {
    Rst_Hi();
    // Setup clock
    //CmdWrite(VS_REG_CLOCKF, 0xC430);    // x4, XTALI = 12.288 MHz
//    CmdWrite(VS_REG_MODE, (VS_SM_SDINEW | VS_SM_RESET));    // Perform software reset
    // Setup registers
    CmdWrite(VS_REG_MODE, (VS_SM_DIFF | VS_ICONF_ADSDI | VS_SM_SDINEW));  // Normal mode
    CmdWrite(VS_REG_MIXERVOL, (VS_SMV_ACTIVE | VS_SMV_GAIN2));
    CmdWrite(VS_REG_RECCTRL, VS_SARC_DREQ512);
    SetVolume(20);
    // Setup variables
    IZero = 0;
}
void VS_t::Disable() {
    AmplifierOff();
    Rst_Lo();
}

bool VS_t::IsBusy (void) {
    return ((GPIO_ReadInputDataBit(GPIOA, VS_DREQ) == Bit_RESET) || DMA_Busy);
}

uint8_t VS_t::BusyWait(void) {
    uint32_t Timeout = VS_TIMEOUT;
    while (IsBusy()) {
        Timeout--;
        if (Timeout == 0) {
            UART_PrintString("VS timeout\r");
            return VS_TIMEOUT_ER;
        }
    }
    return VS_OK;
}

// ==== Amplifier ====
void VS_t::AmplifierOn() {
    // Setup GPIO0 as output
    CmdWrite(VS_REG_WRAMADDR, 0xC017);
    CmdWrite(VS_REG_WRAM,     0x0001);
    // Set output high
    CmdWrite(VS_REG_WRAMADDR, 0xC019);
    CmdWrite(VS_REG_WRAM,     0x0001);
}
void VS_t::AmplifierOff() {
    // Setup GPIO0 as output
    CmdWrite(VS_REG_WRAMADDR, 0xC017);
    CmdWrite(VS_REG_WRAM,     0x0001);
    // Set output low
    CmdWrite(VS_REG_WRAMADDR, 0xC019);
    CmdWrite(VS_REG_WRAM,     0x0000);
}

// ==== Commands ====
uint8_t VS_t::CmdRead(uint8_t AAddr, uint16_t* AData) {
    uint8_t IReply;
    uint16_t IData;
    // Wait until ready
    if ((IReply = BusyWait()) != VS_OK) return IReply; // Get out in case of timeout
    XCS_Lo();   // Start transmission
    ReadWriteByte(VS_READ_OPCODE);  // Send operation code
    ReadWriteByte(AAddr);           // Send addr
    *AData = ReadWriteByte(0);      // Read upper byte
    *AData <<= 8;
    IData = ReadWriteByte(0);       // Read lower byte
    *AData += IData;
    XCS_Hi();   // End transmission
    return VS_OK;
}
uint8_t VS_t::CmdWrite(uint8_t AAddr, uint16_t AData) {
    uint8_t IReply;
    // Wait until ready
    if ((IReply = BusyWait()) != VS_OK) return IReply; // Get out in case of timeout
    XCS_Lo();                       // Start transmission
    ReadWriteByte(VS_WRITE_OPCODE); // Send operation code
    ReadWriteByte(AAddr);           // Send addr
    ReadWriteByte(AData >> 8);      // Send upper byte
    ReadWriteByte(0x00FF & AData);  // Send lower byte
    XCS_Hi();                       // End transmission
    return VS_OK;
}

uint8_t VS_t::ReadWriteByte(uint8_t AByte) {
    SPI1->DR = AByte;
    while (!(SPI1->SR & SPI_I2S_FLAG_RXNE));
    return (uint8_t)(SPI1->DR);
}

// ============================ Interrupts =====================================
// DMA interrupt to check if music is over
void DMA1_Channel3_IRQHandler(void) {
    // Test on DMA1 Channel3 Transfer Complete interrupt
    if(DMA_GetITStatus(DMA1_IT_TC3)) {
        // Clear DMA1 Channel3 Half Transfer, Transfer Complete and Global interrupt pending bits
        DMA_ClearITPendingBit(DMA1_IT_GL3);
        while (!(SPI1->SR & SPI_I2S_FLAG_TXE));
        while (SPI1->SR & SPI_I2S_FLAG_BSY);

        //Delay.Loop(500);

        //while (!(SPI1->SR & SPI_I2S_FLAG_RXNE));
        // End transmission
        Vs.XDCS_Hi();
        Vs.DMA_Busy = false;
        DMA_DeInit(DMA1_Channel3);
        SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, DISABLE);
        //UART_StrUint("Tick ", Delay.TickCounter);
    } // if IT_TC3
}
