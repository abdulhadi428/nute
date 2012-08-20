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

Ads124x_t Ads;

void Ads124x_t::Init() {
    // ==== GPIO ====
    klGpioSetupByMsk(GPIOA, GPIO_Pin_2 | GPIO_Pin_4, GPIO_Mode_Out_PP); // PWDN and CS are outputs
    klGpioSetupByMsk(GPIOA, GPIO_Pin_3, GPIO_Mode_IPU);                 // DRDY is input
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
}

uint8_t Ads124x_t::ReadWriteByte(uint8_t AByte) {
    SPI1->DR = AByte;
    while (!(SPI1->SR & SPI_I2S_FLAG_RXNE));  // Wait for SPI transmission to complete
    return SPI1->DR;
}

// ============================= Interrupts ====================================
//void EXTI3_IRQHandler(void) {
//    if(EXTI_GetITStatus(EXTI_Line3) != RESET) {
//        EXTI_ClearITPendingBit(EXTI_Line3);
//        CC.IRQHandler();
//    }
//}
