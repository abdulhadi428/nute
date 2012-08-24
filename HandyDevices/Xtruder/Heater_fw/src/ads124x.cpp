/*
 * ads124x.cpp
 *
 *  Created on: 21.08.2012
 *      Author: kreyl
 *
 *      24-bit ADC
 */

#include "ads124x.h"
#include "stm32f10x_spi.h"

// Commands
#define RDATA       0b00000001
#define RREG        0b00010000
#define WREG        0b01010000
#define SELFCAL     0b11110000

Ads124x_t Ads;

void Ads124x_t::Init() {
    // ==== GPIO ====
    klGpioSetupByMsk(GPIOA, GPIO_Pin_2 | GPIO_Pin_4, GPIO_Mode_Out_PP); // PWDN and CS are outputs
    klGpioSetupByMsk(GPIOA, GPIO_Pin_5 | GPIO_Pin_7, GPIO_Mode_AF_PP);  // MOSI & SCK as Alternate Function Push Pull
    klGpioSetupByMsk(GPIOA, GPIO_Pin_6, GPIO_Mode_IN_FLOATING);         // MISO as Input Floating
    CsHi();

    // ==== SPI init ====    MSB first, master, SCK idle low, falling edge, no faster than (4.9 / 4 = 1) MHz
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    SPI_InitTypeDef SPI_InitStructure;
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode      = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize  = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL      = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA      = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS       = SPI_NSS_Soft;
    SPI_InitStructure.SPI_FirstBit  = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;  // For 8 MHz CPU clock
    SPI_Init(SPI1, &SPI_InitStructure);
    SPI_Cmd(SPI1, ENABLE);

    PwdnHi();   // Enable operation
    // Setup ADC
    Reset();
    SetGain(ADS_GAIN_1);
    SetChannel(ADS_CH_1);
    /* Analog settings: unipolar, speed=1 (Fosc==4.9152MHz), buffer disabled,
     * Data output is MSB, Range is +-Vref, Data Rate=7.5Hz  */
    //WriteReg(ADS_REG_ACR, 0b01100001);
    WriteReg(ADS_REG_ACR, 0b01100010);  // Data rate = 3Hz
    // Variables
    NewData = false;
    // ==== IRQ ====
    Drdy.Init(GPIOA, 3, GPIO_Mode_IPD);
    Drdy.IrqSetup(EXTI_Trigger_Falling);
    Drdy.IrqEnable();
    // Calibrate
    SelfCalibrate();
}

void Ads124x_t::IrqHandler() {
    // Read conversion result
    uint32_t Value;
    CsLo();
    WriteReadByte(RDATA);
    IReplyDelay();
    Value  = WriteReadByte(0);
    Value <<= 8;
    Value += WriteReadByte(0);
    Value <<= 8;
    Value += WriteReadByte(0);
    CsHi();
    // Convert to temperature
    Temperature[IChIndx] = (int32_t)(Value / AdsChs[IChIndx].a + AdsChs[IChIndx].b);
    // Set new channel
    if(++IChIndx == ADS_CH_COUNT) {
        IChIndx = 0;
        NewData = true; // all channels was measured
    }
    SetChannel(AdsChs[IChIndx].Mux);
    //Uart.Printf(">");
}

void Ads124x_t::SelfCalibrate(void) {
    NewData = false;
    WriteStrobe(SELFCAL);
}

//void Ads124x_t::ReadReg(uint8_t AAddress, uint8_t ACount, uint8_t *PData) {
//    CsLo();
//    WriteReadByte(RREG | AAddress);
//    WriteReadByte(ACount-1);        // Will be replied ACount bytes
//    IReplyDelay();
//    for(uint8_t i=0; i<ACount; i++) *PData = WriteReadByte(0);
//    CsHi();
//}
void Ads124x_t::WriteReg(uint8_t AAddress, uint8_t AData) {
    //__disable_irq();
    CsLo();
    WriteReadByte(WREG | AAddress);
    WriteReadByte(0);   // Write single byte
    WriteReadByte(AData);
    CsHi();
    //__enable_irq();
}

void Ads124x_t::WriteStrobe(uint8_t AStrobe) {
    //__disable_irq();
    CsLo();
    WriteReadByte(AStrobe);
    CsHi();
    //__enable_irq();
}

uint8_t Ads124x_t::WriteReadByte(uint8_t AByte) {
    SPI1->DR = AByte;
    while (!(SPI1->SR & SPI_I2S_FLAG_RXNE));  // Wait for SPI transmission to complete
    return SPI1->DR;
}

// ============================= Interrupts ====================================
void EXTI3_IRQHandler(void) {
    if(EXTI_GetITStatus(EXTI_Line3) != RESET) {
        EXTI_ClearITPendingBit(EXTI_Line3);
        Ads.IrqHandler();
    }
}
