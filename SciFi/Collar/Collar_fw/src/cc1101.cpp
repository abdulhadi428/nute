/*
 * File:   cc1101.c
 * Author: Laurelindo
 *
 * Created on 06/03/2010 г., 2:08
 */
#include <inttypes.h>
#include "cc1101.h"
#include "kl_lib.h"
#include "nute.h"

// ============================ Variables ======================================
CC_t CC;

// ========================== Implementation ===================================
void CC_t::Task(void) {
    // Handle state
    static uint32_t Tmr;
    if (!Delay.Elapsed(&Tmr, 99)) return;
    GetState();
    switch (IState) {
        case CC_STB_RX_OVF:
            FlushRxFIFO();
            klPrintf("RX ovf\r");
            break;
        case CC_STB_TX_UNDF:
            FlushTxFIFO();
            klPrintf("TX ovf\r");
            break;
        default: // Just get out in other cases
            break;
    } //Switch
}

void CC_t::IRQHandler() {
    if (IsTransmitting) {  // Switch to RX (RX is entered automatically as TX->RX)
        IsTransmitting = false;
        //klPrintf("2\r");
        Receive();
    }
    else { // Was receiving
        //uint8_t PktState = ReadRegister(CC_PKTSTATUS);
        //klPrintf("State: %X; ", PktState);
        if (ReadRX()) {
            Nute.NewPktRcvd = true;
            //klPrintf("RX: %A\r", (uint8_t*)&RX_Pkt, CC_PKT_LEN+2);
//            klPrintf("ID: %u; RSSI: %i\r", RX_Pkt.PktID, RSSI_dBm());
        }
        FlushRxFIFO();
    } // if ITransmitting
}


void CC_t::Init(void) {
    // ******** Hardware init section *******
    // ==== GPIO init ====
    klGpioSetupByMsk(CC_GPIO, CC_CS, GPIO_Mode_Out_PP);             // Configure CC_CS as Push-Pull output
    klGpioSetupByMsk(CC_GPIO, CC_SCLK | CC_MOSI, GPIO_Mode_AF_PP);  // Configure MOSI & SCK as Alternate Function Push Pull
    klGpioSetupByMsk(CC_GPIO, CC_MISO, GPIO_Mode_IN_FLOATING);      // Configure MISO as Input Floating
    //klGpioSetupByMsk(CC_GPIO, CC_GDO0, GPIO_Mode_IPU);              // Configure CC_GDO as Input Pull-up
    this->CS_Hi();

    // ==== IRQ ====
    IrqPin.Init(CC_GPIO, CC_GDO0_N, GPIO_Mode_IPD);
    IrqPin.IrqSetup(EXTI_Trigger_Rising);
    IrqPin.IrqDisable();

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
    // ******* Firmware init section *******
    Reset();
    FlushRxFIFO();
    RfConfig();
    cc1190.Init();
    cc1190.SetHighGainMode();
}

void CC_t::SetChannel(uint8_t AChannel) {
    // CC must be in IDLE mode
    while (IState != CC_STB_IDLE) this->EnterIdle();
    // Now set channel
    this->WriteRegister(CC_CHANNR, AChannel);
}

void CC_t::Transmit(void) {
    // Switch IRQ to 0x06 to signal end of transmission
    IrqPin.IrqDisable();
    WriteRegister(CC_IOCFG0, 0x06); // Asserts when sync word has been sent / received, and de-asserts at the end of the packet.
    IrqPin.IrqSetup(EXTI_Trigger_Falling);  // End of pkt
    IrqPin.IrqEnable();
    WriteTX();
    cc1190.PaEnable();
    IsTransmitting = true;
    EnterTX();
}
void CC_t::Receive(void) {
    cc1190.LnaEnable();
    // Switch IRQ to 0x07 to signal end of reception
    IrqPin.IrqDisable();
    WriteRegister(CC_IOCFG0, 0x07); // Asserts when a packet has been received with CRC OK. De-asserts when the first byte is read.
    IrqPin.IrqSetup(EXTI_Trigger_Rising);  // FIFO ready
    IrqPin.IrqEnable();
    FlushRxFIFO();
    IsTransmitting = false;
    EnterRX();
    //klPrintf("3\r");
}

// Return RSSI in dBm
int16_t CC_t::RSSI_dBm(uint8_t ARawRSSI) {
    int16_t RSSI = ARawRSSI;
    if (RSSI >= 128) RSSI -= 256;
    RSSI = (RSSI / 2) - 74;    // now it is in dBm
    return RSSI;
}

// ============================= Inner use =====================================
void CC_t::WriteTX() {
    uint8_t *p = (uint8_t*)(&Nute.TX_Pkt);
    CS_Lo();                 // Start transmission
    BusyWait();              // Wait for chip to become ready
    ReadWriteByte(CC_FIFO|CC_WRITE_FLAG|CC_BURST_FLAG);         // Address with write & burst flags
    for (uint8_t i=0; i<CC_PKT_LEN; i++) ReadWriteByte(*p++);   // Write bytes themselves
    CS_Hi();                                                    // End transmission
}
//#define CC_PRINT_RX
bool CC_t::ReadRX() {
    uint8_t *p = (uint8_t*)(&Nute.RX_Pkt);
    uint8_t b;
    uint8_t FifoSize = ReadRegister(CC_RXBYTES);    // Get number of bytes in FIFO
    FifoSize &= 0x7F;                               // Remove MSB
#ifdef CC_PRINT_RX
    klPrintf("Size: %u    ", FifoSize);
#endif
    if(FifoSize == 0) return false;
    if (FifoSize > (CC_PKT_LEN+2)) FifoSize = CC_PKT_LEN+2;
    CS_Lo();                                        // Start transmission
    BusyWait();                                     // Wait for chip to become ready
    ReadWriteByte(CC_FIFO|CC_READ_FLAG|CC_BURST_FLAG);              // Address with read & burst flags
    for (uint8_t i=0; i<FifoSize; i++) {    // Read bytes
        b = ReadWriteByte(0);
        *p++ = b;
#ifdef CC_PRINT_RX
        klPrintf("%X ", b);
#endif
    }
    CS_Hi();    // End transmission
#ifdef CC_PRINT_RX
    klPrintf("\r");
#endif
    return true;
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
    IState = ReadWriteByte(AStrobe);  // Write strobe
    CS_Hi();      // End transmission
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
void EXTI4_IRQHandler(void) {
    if(EXTI_GetITStatus(EXTI_Line4) != RESET) {
        EXTI_ClearITPendingBit(EXTI_Line4);
        CC.IRQHandler();
    }
}
