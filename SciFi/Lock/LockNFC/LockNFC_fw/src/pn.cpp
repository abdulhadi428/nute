/*
 * pn.cpp
 *
 *  Created on: 07.09.2011
 *      Author: g.kruglov
 */

#include "pn.h"
#include <string.h>
#include <stdarg.h>

//#define PN_PRINT_EXCHANGE
//#define PN_PRINT_ACK
//#define PN_PRINT_IN_OUT_DATA
//#define PN_PRINT_TAGS

PN_t PN;
Card_t Card;

// ============================ Implementation ================================
void PN_t::Init() {
    HardwareInit();

    // Time to wake-up
    Delay.ms(4);   // Let the reset to act
    ResetHi();     // Remove reset
    Delay.ms(9);   // Allow PN to wakeup.

    // ==== Setup PN ====
    // Read status until ready
    uint8_t r=0xFF;
    do {
        NssLo();
        Delay.us(999);
        WriteReadByte(0x02);
        r = WriteReadByte(0x00);
        NssHi();
        Delay.ms(1);
        //klPrintf("R=%X\r", r);
    } while (r == 0xFF);

    // Initial commands
    Cmd(PN_CMD_GET_FIRMWARE_VERSION, 0);    // First Cmd will be discarded
    Cmd(PN_CMD_SAM_CONFIGURATION, 1, 0x01); // Disable SAM to calm PN: Normal mode, the SAM is not used

    // ======= Card =======
    Card.State = csCardOut;
    Card.Data = &Buf[2];
}

void PN_t::Task() {
    if (Delay.Elapsed(&Timer, 450)) {
        if (Card.State != csCardOk) {
            if (CardIsAppeared()) {
                //klPrintf("CardOk\r");
                Card.State = csCardOk;
                if(Evt_CardAppeared != 0) Evt_CardAppeared();
            }
        }
        else {
            if (!CheckIfCardStillNear()) {
                //klPrintf("Card Out\r");
                Card.State = csCardOut;
            }
        } // if Card is ok
    } // if delay
}

// ============================= General ======================================
/*
 * Send Cmd to PN and get reply.
 * ALength is count of bytes to send excluding ACmd, i.e. ALength == count of arguments
 */
PN_PktRslt_t PN_t::Cmd(uint8_t ACmd, uint32_t ALength, ...) {
    Length = ALength + 1;
    Buf[0] = ACmd;
    if (ALength != 0) {
        va_list Arg;
        va_start(Arg, ALength);    // Set pointer to last argument
        for(uint32_t i=0; i<ALength; i++) Buf[i+1] = (uint8_t)va_arg(Arg, int);
        va_end(Arg);
    }
    return WriteAndWaitData();
}


// ============================ Initiator =====================================
uint8_t PN_t::InSelect(uint8_t ATagID) {
    if (Cmd(PN_CMD_IN_SELECT, 1, ATagID) != pnPktOK) return 0x01;   // timeout
    else return (Buf[1]);
}
uint8_t PN_t::InATR(uint8_t ATagID) {
    if (Cmd(PN_CMD_IN_ATR, 2, ATagID, 0) != pnPktOK) return 0x01;   // timeout
    else return (Buf[1]);
}

// ============================== Card ========================================
/*
 * Check if new cards appeared
 */
bool PN_t::CardIsAppeared() {
    uint16_t NFCIDLength, ATSLength=0, NumberOfHistoricalBytes=0;
    uint8_t *ATS = 0;
    // Setup RF config: Max retries, ATR: once, PSL: once, Activation: 5
    Cmd(PN_CMD_RF_CONFIGURATION, 4, 0x05, 0x00, 0x00, 0x05);
    FieldOn();
    // Check if something is near
    if ( // Max number of targets = 1, Baudrate: 106kbps type A
        (Cmd(PN_CMD_IN_LIST_PASSIVE_TARGET, 2, 0x01, 0x00) != pnPktOK) ||   // }
        (Buf[0] != PN_RPL_IN_LIST_PASSIVE_TARGET) ||                        // } Incorrect reply
        (Buf[1] == 0)) {                                                    // Nothing found
        FieldOff();
        return false;
    }
    // ==== Tag is found ====
#ifdef PN_PRINT_TAGS
    klPrintf("Tag found\r");
    for (uint8_t i=2; i<Length; i++) klPrintf("%X ", Buf[i]);
    klPrintf("\r");

    klPrintf("Tag1:\r");
    klPrintf("SensRes: %X %X\r", Buf[3], Buf[4]);
    klPrintf("SelRes: %X\r");
    NFCIDLength = Buf[6];
    klPrintf("NFCIDLength: %X\r", NFCIDLength);
    klPrintf("NFCID: ");
    for (uint8_t i=7; i<(7+NFCIDLength); i++) klPrintf("%X ", Buf[i]);
    klPrintf("\r");
    if (Length > (7+NFCIDLength)) {
        ATSLength = Buf[7+NFCIDLength];
        klPrintf("ATSLength: %X\r", ATSLength);
        klPrintf("ATS: ");
        for (uint8_t i=7+NFCIDLength+1; i<(7+NFCIDLength+1+ATSLength-1); i++) klPrintf("%X ", Buf[i]);
        klPrintf("\r");
    }
#endif
    // ==== Parse RATS ====
    NFCIDLength = Buf[6];
    if (Length > (7+NFCIDLength)) { // ATS exists
        ATS = &Buf[7+NFCIDLength];
        ATSLength = ATS[0];
        if (ATSLength != 0) {
            NumberOfHistoricalBytes = ATSLength-2;  // if T0, but none of TA, TB & TC will be found there.
            uint8_t T0 = ATS[1];
            if (T0 & 0b01000000) NumberOfHistoricalBytes--; // TC1 presents
            if (T0 & 0b00100000) NumberOfHistoricalBytes--; // TB1 presents
            if (T0 & 0b00010000) NumberOfHistoricalBytes--; // TA1 presents
        } // if ATS length
    } // if ATS
    return true;
}

bool PN_t::CheckIfCardStillNear() {
    // Try to read data from Mifare to determine if it still near
    if (MifareRead(0, 0)) return true;
    // Else nothing left near
    FieldOff();
    return false;
}

bool PN_t::MifareRead(uint8_t *ABuf, uint32_t AAddr) {
    if (Cmd(PN_CMD_IN_DATA_EXCHANGE, 3, 0x01, MIFARE_CMD_READ, AAddr) == pnPktOK) {
        //klPrintf("PN reply: %H\r", Buf, Length);
        if ((Buf[0] == 0x41) && (Buf[1] == 0x00)) { // Correct reply & errorcode == 0
            if (ABuf != 0) memcpy(ABuf, &Buf[2], 16);
            return true;
        }
    }
    return false;
}


// ============================ Private =======================================
PN_PktRslt_t PN_t::WriteAndWaitReply(void) {
    WritePkt();
    // Now wait for ACK for about 27 ms
    Delay.us(10);
    uint32_t i=0;
    while (!ReplyIsReady()) {
        Delay.ms(1);
        // Check timeout, get out if fired
        if (++i > 27) {
            klPrintf("Reply Timeout\r");
            return pnPktFail;
        }
    }
    // Some reply is ready
    PN_PktRslt_t Response = ReadPkt();
    return Response;
}

PN_PktRslt_t PN_t::WriteAndWaitData(void) {
    PN_PktRslt_t Response = WriteAndWaitReply();
    if (Response == pnPktACK) {    // ACK => proceed with getting reply packet
        Delay.us(10);
        while (!ReplyIsReady());
        // Some reply is ready
        Response = ReadPkt();
    } // if ACK
    return Response;
}


// ****************************************************************************
// * Transmits data from Buf of length Length                                 *
// * Always uses extended frame format - for simplicity                       *
// ****************************************************************************
void PN_t::WritePkt() {
#ifdef PN_PRINT_IN_OUT_DATA
    klPrintf("OutData:");
    for (uint32_t i=0; i<Length; i++) klPrintf(" %X", Buf[i]);
    klPrintf("\r");
#endif
    uint8_t DCS = PN_TFI_TRANSMIT;
    // Compute length bytes. +1 due to TFI byte
    uint8_t ILngHi = (uint8_t)(0x00FF & ((Length+1) >> 8));
    uint8_t ILngLo = (uint8_t)(0x00FF &  (Length+1));
    NssLo();
    WriteReadByte(PN_PRE_DATA_WRITE);   // DW: following operation is writing (p. 42 of user manual)
    WriteReadByte(0x00);    // PREAMBLE
    WriteReadByte(0x00);    // }
    WriteReadByte(0xFF);    // } START CODE: 0x00 0xFF - Start of packet code
    WriteReadByte(0xFF);    // }
    WriteReadByte(0xFF);    // } Extended frame identifier
    WriteReadByte(ILngHi);  // Upper byte of Length
    WriteReadByte(ILngLo);  // Lower byte of Length
    WriteReadByte(-(ILngHi + ILngLo));     // LCS: Length checksum: (LCS+LEN) = 0
    WriteReadByte(PN_TFI_TRANSMIT); // TFI: frame identifier
    // Payload data
    for (uint32_t i=0; i<Length; i++) {
        WriteReadByte (Buf[i]);
        DCS += Buf[i];
    } // for
    WriteReadByte (-DCS);   // DCS: packet data checksum: (TFI + Cmd+ AData[0] + AData[1] +...+ AData[n] + DCS) = 0
    WriteReadByte (0x00);   // POSTAMBLE
    NssHi();
}

PN_PktRslt_t PN_t::ReadPkt(void) {
    uint8_t tmp;
    NssLo();
    WriteReadByte(PN_PRE_DATA_READ);    // Write a byte indicating that the following operation is reading
    // Skip all zero bytes
    do {
        tmp = WriteReadByte(0);
    } while (tmp == 0x00);

    // First non-zero byte must be 0xFF; get out if not
    if (tmp != 0xFF) {
        NssHi();
        return (pnPktFail);
    }

    // Recieve next 3 bytes to determine frame type
    Buf[0] = WriteReadByte(0);
    Buf[1] = WriteReadByte(0);
    Buf[2] = WriteReadByte(0);

    uint8_t FCheckSum;
    // Check if ACK: 00_FF_00
    if ((Buf[0] == 0x00) && (Buf[1] == 0xFF) && (Buf[2] == 0x00)) {
        NssHi();
#ifdef PN_PRINT_ACK
        UARTSendString("ACK\r");
#endif
        return (pnPktACK);
    }

    // Check if Error Frame: 01_FF_7F...
    else if ((Buf[0] == 0x01) && (Buf[1] == 0xFF) && (Buf[2] == 0x7F)) {
        Buf[3] = WriteReadByte(0); // must be 0x81
        Buf[4] = WriteReadByte(0); // must be 0x00
        NssHi();
#ifdef PN_PRINT_ACK
        UARTSendString("ERR\r");
#endif
        // Check if syntax error
        if ((Buf[3] != 0x81) || (Buf[4] != 0x00)) return (pnPktFail);
        else return (pnPktErrFrame);
    }

    // Check if Extended Frame: FF_FF...
    else if ((Buf[0] == 0xFF) && (Buf[1] == 0xFF)) {
        // R[2] is LENm, already read
        Buf[3] = WriteReadByte(0);   // R[3] is LENl
        Buf[4] = WriteReadByte(0);   // R[4] is LCS
        Buf[5] = WriteReadByte(0);   // TFI
        // Check Length checksum & TFI
        FCheckSum = Buf[2] + Buf[3] + Buf[4];
        if ((FCheckSum != 0) || (Buf[5] != PN_TFI_RECIEVE)) {
            NssHi();
            return (pnPktCRC);
        }
        // Calculate Big Length
        Length = Buf[2];
        Length <<= 8;
        Length += Buf[3];
        Length--; // as LEN includes TFI
    }

    // If normal frame
    else {
        // Buf[0] is LEN, Buf[1] is LCS, Buf[2] is TFI
        // Check Length checksum & TFI
        FCheckSum = Buf[0] + Buf[1];
        if ((FCheckSum != 0) || (Buf[2] != PN_TFI_RECIEVE)) {
            NssHi();
            return (pnPktCRC);
        }
        // CRC ok
        Length = Buf[0] - 1;  // -1 as LEN includes TFI
    }
#ifdef PN_PRINT_ACK
    UARTSendString("RPL: ");
    UARTSendUint(Length);
    UARTNewLine();
#endif
    // Receive remainder of packet, discarding received start of packet
    FCheckSum = PN_TFI_RECIEVE;
    for (uint32_t i=0; i<Length; i++) {
        Buf[i] = WriteReadByte(0);
        FCheckSum += Buf[i];
    }
    // Recieve DCS
    FCheckSum += WriteReadByte(0);
    // Check for FCheckSum == 0: TFI+Data[0]+Data[1]+...+DCS must be equal to 0
    if (FCheckSum != 0) {
        NssHi();
        return (pnPktCRC);
    }
    // Recieve postamble
    WriteReadByte(0);
    NssHi();
    // DEBUG
#ifdef PN_PRINT_ACK
    UARTSendString("PktOK\r");
#endif
#ifdef PN_PRINT_IN_OUT_DATA
    klPrintf("InData:");
    for (uint32_t i=0; i<Length; i++) klPrintf(" %X", Buf[i]);
    klPrintf("\r");
#endif
    return (pnPktOK);
}

// ============================== Low-level ===================================
uint8_t PN_t::WriteReadByte(uint8_t AByte) {
    SPI1->DR = AByte;
    while (!(SPI1->SR & SPI_I2S_FLAG_RXNE));
    uint8_t Response = (uint8_t)(SPI1->DR);
#ifdef PN_PRINT_EXCHANGE
    klPrintf(">%X  <%X\r", AByte, Response);
#endif
    return Response;
}

void PN_t::HardwareInit(void) {
    // Clocks init
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1 | RCC_APB2Periph_AFIO, ENABLE);
    // ==== GPIO init ====
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    // Configure RST & NSS as Push-Pull output
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    ResetLo();
    NssHi();
    // Configure IRQ as input Pull-Up
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // Configure CLK & MOSI as Alternate Function Push Pull
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // Configure MISO as Input Floating
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // ==== SPI init ====    LSB first, master, SCK idle low, First edge
    SPI_InitTypeDef SPI_InitStructure;
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode      = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize  = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL      = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA      = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS       = SPI_NSS_Soft;
    SPI_InitStructure.SPI_FirstBit  = SPI_FirstBit_LSB;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_Init(SPI1, &SPI_InitStructure);
    SPI_Cmd(SPI1, ENABLE);
}
