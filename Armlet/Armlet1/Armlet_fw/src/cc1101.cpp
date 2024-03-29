/*
 * File:   cc1101.c
 * Author: Laurelindo
 *
 * Created on 06/03/2010 г., 2:08
 */
#include <inttypes.h>
#include "cc1101.h"
#include "kl_lib.h"

//CC_t CC;

// ========================== Implementation ===================================
void CC_t::Task(void) {
//    static uint32_t Tmr;
//    if(!Delay.Elapsed(&Tmr, 99)) return;
    GetState();
    switch (IState) {
        case CC_STB_RX_OVF:
            FlushRxFIFO();
            Uart.Printf("RX ovf\r");
            break;
        case CC_STB_TX_UNDF:
            FlushTxFIFO();
            Uart.Printf("TX ovf\r");
            break;
        case CC_STB_IDLE:   // Reenter RX if packet was jammed
            if (Aim == caRx) EnterRX();
            break;
        default: // Just get out in other cases
            break;
    } //Switch
    //if ((IState != CC_STB_IDLE) or (IState != CC_STB_RX)) klPrintf("State: %X\r", IState);
}

void CC_t::Init(void) {
    // ******** Hardware init section *******
    // ==== SPI GPIO init ====
    klGpioSetupByMsk(GPIOA, GPIO_Pin_0, GPIO_Mode_Out_PP);              // Configure CC_CS as Push-Pull output
    klGpioSetupByMsk(GPIOA, GPIO_Pin_5 | GPIO_Pin_7, GPIO_Mode_AF_PP);  // Configure MOSI & SCK as Alternate Function Push Pull
    klGpioSetupByMsk(GPIOA, GPIO_Pin_6, GPIO_Mode_IN_FLOATING);         // Configure MISO as Input Floating
    CsHi();
    // ==== SPI init ====    MSB first, master, SCK idle low
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    SPI_InitTypeDef SPI_InitStructure;
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode      = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize  = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL      = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA      = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS       = SPI_NSS_Soft;
    SPI_InitStructure.SPI_FirstBit  = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_Init(SPI1, &SPI_InitStructure);
    SPI_Cmd(SPI1, ENABLE);

    Reset();
    FlushRxFIFO();
    RfConfig();
    // ==== IRQs ====
    IrqPin0.Init(GPIOA, 4, GPIO_Mode_IPD);
    IrqPin0.IrqSetup(EXTI_Trigger_Falling);
    IrqPin0.IrqEnable();
}

void CC_t::SetChannel(uint8_t AChannel) {
    // CC must be in IDLE mode
    while (IState != CC_STB_IDLE) EnterIdle();
    // Now set channel
    WriteRegister(CC_CHANNR, AChannel);
}

void CC_t::WaitUntilChannelIsBusy() {
    uint8_t b;
    for(uint32_t i=0; i<207; i++) {
        b = ReadRegister(CC_PKTSTATUS);
        if(b & 0b00010000) break;
        //Uart.Printf("C");
    }
    //Uart.Printf("\r");
}

void CC_t::Transmit(void) {
    do {
        EnterIdle();
    } while(IState != CC_STB_IDLE);

    WriteTX((uint8_t*)(&PktTx), CC_PKT_LEN);
    Aim = caTx;
    EnterTX();
}
void CC_t::TransmitAndWaitIdle(void) {
    WaitUntilChannelIsBusy();   // If this is not done, time after time FIFO is destroyed
    EnterIdle();
    WriteTX((uint8_t*)(&PktTx), CC_PKT_LEN);
    EnterTX();
    while(IrqPin0 == 0); // Wait until sync word is sent
    while(IrqPin0 == 1); // Wait until transmission completed
}

void CC_t::Receive(void) {
    Aim = caRx;
    EnterRX();  // After that, some time will be wasted to recalibrate
}

// Return RSSI in dBm
int16_t CC_t::RSSI_dBm(uint8_t ARawRSSI) {
    int16_t RSSI = ARawRSSI;
    if (RSSI >= 128) RSSI -= 256;
    RSSI = (RSSI / 2) - 74;    // now it is in dBm
    return RSSI;
}

// ============================= Inner use =====================================
void CC_t::WriteTX(uint8_t* PArr, uint8_t ALen) {
    CsLo();                                                 // Start transmission
    BusyWait();                                             // Wait for chip to become ready
    ReadWriteByte(CC_FIFO|CC_WRITE_FLAG|CC_BURST_FLAG);     // Address with write & burst flags
    uint8_t b;
    //Uart.Printf("TX: ");
    for (uint8_t i=0; i<ALen; i++) {
        b = *PArr++;
        ReadWriteByte(b);  // Write bytes
      //  Uart.Printf("%X ", b);
    }
    CsHi();    // End transmission
    //Uart.Printf("\r");
}
//#define CC_PRINT_RX
bool CC_t::ReadRX(uint8_t* PArr) {
    uint8_t b;
    bool result=false;
    //b = ReadRegister(CC_RXBYTES);
    //Uart.Printf("Sz: %X  ", b);
    // Get pkt status
    b = ReadRegister(CC_PKTSTATUS);
//    Uart.Printf("St: %X  ", b);
    if(b & 0x80) {  // CRC OK
//    if(b) {  // FIFO not empty
        CsLo();                                            // Start transmission
        BusyWait();                                         // Wait for chip to become ready
        ReadWriteByte(CC_FIFO|CC_READ_FLAG|CC_BURST_FLAG);  // Address with read & burst flags
        for (uint8_t i=0; i<CC_PKT_LEN+2; i++) {            // Read bytes
            b = ReadWriteByte(0);
            *PArr++ = b;
      //      Uart.Printf(" %X", b);
        }
        CsHi();    // End transmission
        result = true;
    }
    //Uart.Printf("\r");
    return result;
}

uint8_t CC_t::ReadRegister (uint8_t ARegAddr){
    CsLo();      // Start transmission
    BusyWait();   // Wait for chip to become ready
    ReadWriteByte(ARegAddr | CC_READ_FLAG);     // Transmit header byte
    uint8_t FReply = ReadWriteByte(0);    // Read reply
    CsHi();      // End transmission
    return FReply;
}
void CC_t::WriteRegister (uint8_t ARegAddr, uint8_t AData){
    CsLo();                 // Start transmission
    BusyWait();              // Wait for chip to become ready
    ReadWriteByte(ARegAddr); // Transmit header byte
    ReadWriteByte(AData);    // Write data
    CsHi();                 // End transmission
}
void CC_t::WriteStrobe (uint8_t AStrobe){
    CsLo();      // Start transmission
    BusyWait();   // Wait for chip to become ready
    IState = ReadWriteByte(AStrobe);  // Write strobe
    CsHi();      // End transmission
    IState &= 0b01110000; // Mask needed bits
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
    WriteRegister(CC_TEST2,    CC_TEST2_VALUE);      // Various test settings.
    WriteRegister(CC_TEST1,    CC_TEST1_VALUE);      // Various test settings.
    WriteRegister(CC_TEST0,    CC_TEST0_VALUE);      // Various test settings.
    WriteRegister(CC_FIFOTHR,  CC_FIFOTHR_VALUE);    // fifo threshold
    WriteRegister(CC_IOCFG2,   CC_IOCFG2_VALUE);     // GDO2 output pin configuration.
    WriteRegister(CC_IOCFG0,   CC_IOCFG0_VALUE);     // GDO0 output pin configuration.
    WriteRegister(CC_PKTCTRL1, CC_PKTCTRL1_VALUE);   // Packet automation control.
    WriteRegister(CC_PKTCTRL0, CC_PKTCTRL0_VALUE);   // Packet automation control.
    WriteRegister(CC_PKTLEN,   CC_PKT_LEN);          // Packet length.

    WriteRegister(CC_PATABLE, CC_PATABLE_INITIAL);

    WriteRegister(CC_MCSM2, CC_MCSM2_VALUE);
    WriteRegister(CC_MCSM1, CC_MCSM1_VALUE);
}

uint8_t CC_t::ReadWriteByte(uint8_t AByte) {
    SPI1->DR = AByte;
    while (!(SPI1->SR & SPI_I2S_FLAG_RXNE));  // Wait for SPI transmission to complete
    return SPI1->DR;
}

// ============================= Interrupts ====================================
//void EXTI3_IRQHandler(void) {
//    if(EXTI_GetITStatus(EXTI_Line3) != RESET) {
//        EXTI_ClearITPendingBit(EXTI_Line3);
//        IRQ2Handler();
//    }
//}
//void EXTI4_IRQHandler(void) {
//    if(EXTI_GetITStatus(EXTI_Line4) != RESET) {
//        EXTI_ClearITPendingBit(EXTI_Line4);
//        IRQ0Handler();
//    }
//}
