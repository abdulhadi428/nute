/*
 * File:   cc1101.c
 * Author: Laurelindo
 *
 * Created on 06/03/2010 г., 2:08
 */
#include <inttypes.h>
#include "cc1101.h"
#include "cc_rf_settings.h"
#include "delay_util.h"

#include "uart.h"

#include "rock_action.h"

// ============================ Variables ======================================
CC_t CC;
uint8_t OthersIDs[18];

// ========================== Implementation ===================================
void CC_t::Task(void) {
    // Reset others list
    if (Delay.Elapsed(&Timer, 40000))
        for (uint8_t i=0; i<18; i++)
            OthersIDs[i] = 0;

    uint8_t FifoSize;
    // Do with CC what needed
    GetState();
    switch (State){
        case CC_STB_RX_OVF:  FlushRxFIFO(); UART_PrintString("\rRX_OVF"); break;
        case CC_STB_TX_UNDF: FlushTxFIFO(); UART_PrintString("\rTX_OVF"); break;

        case CC_STB_IDLE:
            //Uart.PrintString("\rIDLE");
            //EnterRX();
//            if (Delay.Elapsed(&Timer, 400)) {
//                UART_PrintString("\rTX");
//                // Prepare packet to send
//                TX_Pkt.CommandID = 0x7C;
//                TX_Pkt.ChargeCount = ERock.ChargeCount;
//                TX_Pkt.ArtType = (uint8_t)ERock.Type;
//                WriteTX(TX_PktArray, CC_PKT_LEN);
//                //CC.EnterTXAndWaitToComplete();
//                EnterTX();
//            }
            break;

        case CC_STB_RX:
            //Uart.PrintString("\rRX");
            if (GDO0_IsHi()) GDO0_WasHi = true;
            // Check if GDO0 has fallen
            else if (GDO0_WasHi) {
                //UART_PrintString("\rIRQ\r");
                GDO0_WasHi = false;
                FifoSize = ReadRegister(CC_RXBYTES); // Get number of bytes in FIFO
                if (FifoSize != 0) {
                    ReadRX(RX_PktArray, (CC_PKT_LEN+2));    // Read two extra bytes of RSSI & LQI
                    EVENT_NewPacket();
                } // if size>0
            } // if falling edge
            break;

        case CC_STB_TX:
            //UART_PrintString("\rTX");
            break;

        default: // Just get out in other cases
            //Uart.PrintString("\rOther: ");
            //Uart.PrintUint(CC.State);
            break;
    }//Switch
}

void CC_t::Init(void) {
    // ******** Hardware init section *******
    // ==== Clocks init ====
    RCC_APB2PeriphClockCmd(CC_GPIO_CLK | CC_AFIO_CLK, ENABLE);
    RCC_APB1PeriphClockCmd(CC_SPI_CLK, ENABLE);
    // ==== GPIO init ====
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    // Configure CC_CS as Push-Pull output
    GPIO_InitStructure.GPIO_Pin  = CC_CS;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(CC_GPIO, &GPIO_InitStructure);
    this->CS_Hi();
    // Configure MOSI & SCK as Alternate Function Push Pull
    GPIO_InitStructure.GPIO_Pin  = CC_SCLK | CC_MOSI;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(CC_GPIO, &GPIO_InitStructure);
    // Configure MISO as Input Floating
    GPIO_InitStructure.GPIO_Pin  = CC_MISO;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(CC_GPIO, &GPIO_InitStructure);
    // ==== IRQ ====
    // Configure CC_GDO as Input Pull-up
    GPIO_InitStructure.GPIO_Pin  = CC_GDO0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(CC_GPIO, &GPIO_InitStructure);
    // ==== SPI init ====    MSB first, master, SCK idle low
    SPI_InitTypeDef SPI_InitStructure;
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode      = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize  = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL      = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA      = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS       = SPI_NSS_Soft;
    SPI_InitStructure.SPI_FirstBit  = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_Init(CC_SPI, &SPI_InitStructure);
    SPI_Cmd(CC_SPI, ENABLE);
    // ******* Firmware init section *******
    GDO0_WasHi = false;
    Reset();
    FlushRxFIFO();
    RfConfig();
}

void CC_t::SetChannel(uint8_t AChannel) {
    // CC must be in IDLE mode
    while (CC.State != CC_STB_IDLE) this->EnterIdle();
    // Now set channel
    this->WriteRegister(CC_CHANNR, AChannel);
}

void CC_t::EnterTXAndWaitToComplete(void) {
    //IRQDisable();
    EnterTX();
    // Wait for packet to transmit completely
    while (!GDO0_IsHi());   // After this, SYNC word is transmitted
    while (GDO0_IsHi());    // After this, packet is transmitted
    //IRQEnable();
}


void CC_t::TimebaseInit() {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
    // Interrupt config
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    // Timebase config
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // Up-counter needed, nothing special
    TIM_TimeBaseStructure.TIM_Period = SUBCYCLE_DURATION;       // Auto-reload value
    TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) (SystemCoreClock / 1000) - 1; // Input clock divisor
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
    //TIM_PrescalerConfig(TIM2, PrescalerValue, TIM_PSCReloadMode_Immediate);
    // Output Compare Timing Mode configuration: Channel1
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
    TIM_OCInitStructure.TIM_Pulse = ((uint16_t)Addr) * PKT_DURATION;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM5, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Disable);
    // Interrupts config
    TIM_ITConfig(TIM5, TIM_IT_CC1 | TIM_IT_Update, ENABLE);
    // Enable timer
    TimerStart();
}

// ============================= Inner use =====================================
void CC_t::WriteTX (uint8_t *PData, uint8_t ALength) {
    CS_Lo();                 // Start transmission
    BusyWait();              // Wait for chip to become ready
    ReadWriteByte(CC_FIFO|CC_WRITE_FLAG|CC_BURST_FLAG);         // Address with write & burst flags
    for (uint8_t i=0; i<ALength; i++) ReadWriteByte(*PData++);  // Write bytes themselves
    CS_Hi();                 // End transmission
}
void CC_t::ReadRX  (uint8_t *PData, uint8_t ALength){
    CS_Lo();                 // Start transmission
    BusyWait();              // Wait for chip to become ready
    ReadWriteByte(CC_FIFO|CC_READ_FLAG|CC_BURST_FLAG);              // Address with read & burst flags
    for (uint8_t i=0; i<ALength; i++) *PData++ = ReadWriteByte(0);  // Write bytes themselves
    CS_Hi();                 // End transmission
}

uint8_t CC_t::ReadRegister (uint8_t ARegAddr){
    CS_Lo();      // Start transmission
    BusyWait();   // Wait for chip to become ready
    ReadWriteByte(ARegAddr | CC_READ_FLAG);     // Transmit header byte
    uint8_t FReply = this->ReadWriteByte(0);    // Read reply
    CS_Hi();      // End transmission
    return FReply;
}
void CC_t::WriteRegister (uint8_t ARegAddr, uint8_t AData){
    CS_Lo();                 // Start transmission
    BusyWait();              // Wait for chip to become ready
    ReadWriteByte(ARegAddr); // Transmit header byte
    ReadWriteByte(AData);    // Write data
    CS_Hi();                 // End transmission
}
void CC_t::WriteStrobe (uint8_t AStrobe){
    CS_Lo();      // Start transmission
    BusyWait();   // Wait for chip to become ready
    State = ReadWriteByte(AStrobe);  // Write strobe
    CS_Hi();      // End transmission
    State &= 0b01110000; // Mask needed bits
}

// **** Used to setup CC with needed values ****
void CC_t::RfConfig(void){
    WriteRegister(CC_FSCTRL1,  CC_FSCTRL1_VALUE);    // Frequency synthesizer control.
    WriteRegister(CC_FSCTRL0,  CC_FSCTRL0_VALUE);    // Frequency synthesizer control.
    WriteRegister(CC_FREQ2,    CC_FREQ2_VALUE);      // Frequency control word, high byte.
    WriteRegister(CC_FREQ1,    CC_FREQ1_VALUE);      // Frequency control word, middle byte.
    WriteRegister(CC_FREQ0,    CC_FREQ0_VALUE);      // Frequency control word, low byte.
    WriteRegister(CC_MDMCFG4,  CC_MDMCFG4_VALUE);    // Modem configuration.
    WriteRegister(CC_MDMCFG3,  CC_MDMCFG3_VALUE);    // Modem configuration.
    WriteRegister(CC_MDMCFG2,  CC_MDMCFG2_VALUE);    // Modem configuration.
    WriteRegister(CC_MDMCFG1,  CC_MDMCFG1_VALUE);    // Modem configuration.
    WriteRegister(CC_MDMCFG0,  CC_MDMCFG0_VALUE);    // Modem configuration.
    WriteRegister(CC_CHANNR,   CC_CHANNR_VALUE);     // Channel number.
    WriteRegister(CC_DEVIATN,  CC_DEVIATN_VALUE);    // Modem deviation setting (when FSK modulation is enabled).
    WriteRegister(CC_FREND1,   CC_FREND1_VALUE);     // Front end RX configuration.
    WriteRegister(CC_FREND0,   CC_FREND0_VALUE);     // Front end RX configuration.
    WriteRegister(CC_MCSM0,    CC_MCSM0_VALUE);      // Main Radio Control State Machine configuration.
    WriteRegister(CC_FOCCFG,   CC_FOCCFG_VALUE);     // Frequency Offset Compensation Configuration.
    WriteRegister(CC_BSCFG,    CC_BSCFG_VALUE);      // Bit synchronization Configuration.
    WriteRegister(CC_AGCCTRL2, CC_AGCCTRL2_VALUE);   // AGC control.
    WriteRegister(CC_AGCCTRL1, CC_AGCCTRL1_VALUE);   // AGC control.
    WriteRegister(CC_AGCCTRL0, CC_AGCCTRL0_VALUE);   // AGC control.
    WriteRegister(CC_FSCAL3,   CC_FSCAL3_VALUE);     // Frequency synthesizer calibration.
    WriteRegister(CC_FSCAL2,   CC_FSCAL2_VALUE);     // Frequency synthesizer calibration.
    WriteRegister(CC_FSCAL1,   CC_FSCAL1_VALUE);     // Frequency synthesizer calibration.
    WriteRegister(CC_FSCAL0,   CC_FSCAL0_VALUE);     // Frequency synthesizer calibration.
    WriteRegister(CC_FSTEST,   CC_FSTEST_VALUE);     // Frequency synthesizer calibration.
    WriteRegister(CC_TEST2,    CC_TEST2_VALUE);      // Various test settings.
    WriteRegister(CC_TEST1,    CC_TEST1_VALUE);      // Various test settings.
    WriteRegister(CC_TEST0,    CC_TEST0_VALUE);      // Various test settings.
    WriteRegister(CC_FIFOTHR,  CC_FIFOTHR_VALUE);    // fifo threshold
    WriteRegister(CC_IOCFG2,   CC_IOCFG2_VALUE);     // GDO2 output pin configuration.
    WriteRegister(CC_IOCFG0,   CC_IOCFG0_VALUE);     // GDO0 output pin configuration.
    WriteRegister(CC_PKTCTRL1, CC_PKTCTRL1_VALUE);   // Packet automation control.
    WriteRegister(CC_PKTCTRL0, CC_PKTCTRL0_VALUE);   // Packet automation control.
    WriteRegister(CC_PKTLEN,   CC_PKT_LEN);          // Packet length.

    WriteRegister(CC_PATABLE, CC_PATABLE0_VALUE);

    WriteRegister(CC_MCSM2, CC_MCSM2_VALUE);
    WriteRegister(CC_MCSM1, CC_MCSM1_VALUE);
}

uint8_t CC_t::ReadWriteByte(uint8_t AByte) {
    CC_SPI->DR = AByte;
    while (!(CC_SPI->SR & SPI_I2S_FLAG_RXNE));  // Wait for SPI transmission to complete
    return CC_SPI->DR;
}
void CC_t::BusyWait(void) {
    while (GPIO_ReadInputDataBit(CC_GPIO, CC_MISO));
}
bool CC_t::GDO0_IsHi(void) {
    return GPIO_ReadInputDataBit(CC_GPIO, CC_GDO0);
}


// ============================= Interrupts ====================================
void TIM5_IRQHandler(void) {
    if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET) {   // Compare IRQ
        TIM_ClearITPendingBit(TIM5, TIM_IT_CC1);
        //UART_StrUint("cmp: ", TIM_GetCounter(TIM5));
        // Time to yell
        //CC.TimerStop();
        // Prepare packet & transmit it
        CC.TX_Pkt.CommandID = 0x7C;
        CC.TX_Pkt.SenderAddr = CC.Addr;
        CC.TX_Pkt.SenderCycle = CC.CycleCounter;
        CC.TX_Pkt.SenderTime = TIM_GetCounter(TIM5);
        CC.TX_Pkt.ChargeCount = ERock.ChargeCount;
        CC.TX_Pkt.ArtType = (uint8_t)ERock.Type;
        CC.WriteTX(CC.TX_PktArray, CC_PKT_LEN); // Write bytes to FIFO
        CC.EnterIdle();
        CC.EnterTX();
    }
    else if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) {
        // Clear TIM5 update interrupt
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
        // Subcycle ends here
        // Handle cycle counter
        if (++CC.CycleCounter >= CYCLE_COUNT) {  // Zero cycle begins
            CC.CycleCounter = 0;
            // Enter RX-before-TX
            //CC.EnterRX();
        }
        //else CC_ENTER_IDLE();   // Non-zero cycle

    }
}