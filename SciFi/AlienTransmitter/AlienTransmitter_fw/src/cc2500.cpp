/*
 * File:   cc1101.c
 * Author: Laurelindo
 *
 * Created on 06/03/2010 г., 2:08
 */
#include <inttypes.h>
#include "cc2500.h"
#include "cc2500_rf_settings.h"
#include "delay_util.h"

//#include "uart.h"

// ============================ Variables ======================================
CC_t CC;

// ========================== Implementation ===================================
void CC_t::Task(void) {
    uint8_t FifoSize;
    // Do with CC what needed
    GetState();
    switch (State){
        case CC_STB_RX_OVF:  FlushRxFIFO(); break;
        case CC_STB_TX_UNDF: FlushTxFIFO(); break;

        case CC_STB_IDLE:
//            UART_PrintString("\rIDLE");
            EnterRX();
//            if (Delay.Elapsed(&Timer, 400)) {
//                UART_PrintString("\rTX");
//                // Prepare packet to send
//                TX_Pkt.CommandID = 0x7C;
//                TX_Pkt.ChargeCount = 4;
//                TX_Pkt.ArtType = 27;
//                WriteTX(TX_PktArray, CC_PKT_LEN);
//                //CC.EnterTXAndWaitToComplete();
//                EnterTX();
//            }
            break;

        case CC_STB_RX:
            //UART_PrintString("\rRX");
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
//    RCC_APB2PeriphClockCmd(CC_GPIO_CLK | CC_AFIO_CLK, ENABLE);
//    RCC_APB1PeriphClockCmd(CC_SPI_CLK, ENABLE);
//    // ==== GPIO init ====
//    GPIO_InitTypeDef GPIO_InitStructure;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    // Configure CC_CS as Push-Pull output
//    GPIO_InitStructure.GPIO_Pin  = CC_CS;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//    GPIO_Init(CC_GPIO, &GPIO_InitStructure);
//    this->CS_Hi();
//    // Configure MOSI & SCK as Alternate Function Push Pull
//    GPIO_InitStructure.GPIO_Pin  = CC_SCLK | CC_MOSI;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//    GPIO_Init(CC_GPIO, &GPIO_InitStructure);
//    // Configure MISO as Input Floating
//    GPIO_InitStructure.GPIO_Pin  = CC_MISO;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//    GPIO_Init(CC_GPIO, &GPIO_InitStructure);
//    // ==== IRQ ====
//    // Configure CC_GDO as Input Pull-up
//    GPIO_InitStructure.GPIO_Pin  = CC_GDO0;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//    GPIO_Init(CC_GPIO, &GPIO_InitStructure);
//    // ==== SPI init ====    MSB first, master, SCK idle low
//    SPI_InitTypeDef SPI_InitStructure;
//    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
//    SPI_InitStructure.SPI_Mode      = SPI_Mode_Master;
//    SPI_InitStructure.SPI_DataSize  = SPI_DataSize_8b;
//    SPI_InitStructure.SPI_CPOL      = SPI_CPOL_Low;
//    SPI_InitStructure.SPI_CPHA      = SPI_CPHA_1Edge;
//    SPI_InitStructure.SPI_NSS       = SPI_NSS_Soft;
//    SPI_InitStructure.SPI_FirstBit  = SPI_FirstBit_MSB;
//    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
//    SPI_Init(CC_SPI, &SPI_InitStructure);
//    SPI_Cmd(CC_SPI, ENABLE);
//    // ******* Firmware init section *******
//    GDO0_WasHi = false;
//    Reset();
//    FlushRxFIFO();
//    RfConfig();
}

void CC_t::SetChannel(uint8_t AChannel) {
    // CC must be in IDLE mode
    while (CC.State != CC_STB_IDLE) this->EnterIdle();
    // Now set channel
    this->WriteRegister(CC_CHANNR, AChannel);
}

void CC_t::EnterTXAndWaitToComplete(void) {
    EnterTX();
    // Wait for packet to transmit completely
    while (!GDO0_IsHi());   // After this, SYNC word is transmitted
    while (GDO0_IsHi());    // After this, packet is transmitted
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
//    CC_SPI->DR = AByte;
//    while (!(CC_SPI->SR & SPI_I2S_FLAG_RXNE));  // Wait for SPI transmission to complete
//    return CC_SPI->DR;
    return 0;
}


