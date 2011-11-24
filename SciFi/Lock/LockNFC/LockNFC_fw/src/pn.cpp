/*
 * pn.cpp
 *
 *  Created on: 07.09.2011
 *      Author: g.kruglov
 */

#include "pn.h"
#include <string.h>
#include "kl_util.h"

//#define PN_PRINT_EXCHANGE
//#define PN_PRINT_ACK
//#define PN_PRINT_IN_OUT_DATA
#define PN_PRINT_TAGS

PN_t PN;
Card_t Card;

void PN_t::Init() {
    // ==== Hardware init ====
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

    Card.Data = &Buf[2];
    // Time to wake-up
    Delay.ms(4);   // Let the reset to act
    ResetHi();     // Remove reset
    Delay.ms(9);   // Allow PN to wakeup.
    // ==== Setup PN ====
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

    // First pkt will be discarded
    Buf[0] = PN_CMD_GET_FIRMWARE_VERSION;
    Length = 1;
    WriteAndWaitReply();
    Delay.ms(40);

    // Disable SAM to calm PN
    Buf[0] = PN_CMD_SAM_CONFIGURATION;
    Buf[1] = 0x01;    // Normal mode, the SAM is not used
    Length = 2;
    WriteAndWaitData();
    Delay.ms(40);

    Buf[0] = PN_CMD_RF_CONFIGURATION;
    Buf[1] = 0x05;   // Max retries
    Buf[2] = 0x00;   // ATR: once
    Buf[3] = 0x00;   // PSL: once
    Buf[4] = 0x05;   // Activation: 5
    Length = 5;
    WriteAndWaitData();
    Delay.ms(40);

    // ======= Card =======
    Card.State = csCardOut;

//    Buf[0] = PN_CMD_SET_PARAMETERS;
//    //Buf[1] = 0x14; // fAutoATR_RES=1, AutoRATS=1
//    Buf[1] = 0x24;
//    Length = 2;
//    WriteAndWaitData();
//    _delay_ms(40);
//    PN.Buf[0] = PN_CMD_WRITE_REGISTER;
//    PN.Buf[1] = 0x63;   // }
//    PN.Buf[2] = 0x0D;   // } Address CIU_ManualRcv
//    PN.Buf[3] = 0x07;   // Value
//    PN.Length = 4;
//    PN.WriteAndWaitData();
//    PN.Buf[0] = PN_CMD_WRITE_REGISTER;
//    PN.Buf[1] = 0x63;   // }
//    PN.Buf[2] = 0x16;   // } Address CIU_RFCfg
//    PN.Buf[3] = 0x68;   // Value
//    PN.Length = 4;
//    PN.WriteAndWaitData();
}

void PN_t::Task() {
    if (Delay.Elapsed(&Timer, 450)) {
        if (Card.State != csCardOk) {
            if (CardIsAppeared()) {
                klPrintf("CardOk\r");
                Card.State = csCardOk;
                // Mifare read
                MifareRead();
            }
        }
        else {
            if (!CheckIfCardStillNear()) {
                klPrintf("Card Out\r");
                Card.State = csCardOut;
            }
        } // if Card is ok
    } // if delay
}



// ============================ Initiator =====================================
uint8_t PN_t::InSelect(uint8_t ATagID) {
    Buf[0] = PN_CMD_IN_SELECT;
    Buf[1] = ATagID;
    Length = 2;
    PN_PktRslt_t rslt = WriteAndWaitData();
    if (rslt != pnPktOK) return 0x01;   // timeout
    else return (Buf[1]);
}
uint8_t PN_t::InATR(uint8_t ATagID) {
    Buf[0] = PN_CMD_IN_ATR;
    Buf[1] = ATagID;
    Buf[2] = 0;
    Length = 3;
    PN_PktRslt_t rslt = WriteAndWaitData();
    if (rslt != pnPktOK) return 0x01;   // timeout
    else return (Buf[1]);
}

// ============================== Card ========================================
/*
 * Check if new cards appeared
 */
bool PN_t::CardIsAppeared() {
    uint16_t NFCIDLength, ATSLength=0, NumberOfHistoricalBytes=0;
    uint8_t *ATS = 0;
    // Check if something is near
    Buf[0] = PN_CMD_IN_LIST_PASSIVE_TARGET;
    Buf[1] = 0x01;   // Max number of targets
    Buf[2] = 0x00;   // Baudrate: 106kbps type A
    Length = 3;
    PN_PktRslt_t rslt = WriteAndWaitData();
    if (rslt != pnPktOK) return false;
    // Pn tried to find something, lets look what happened
    if (Buf[0] != PN_RPL_IN_LIST_PASSIVE_TARGET) return false;
    if (Buf[1] == 0) return false;    // Nothing found
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
    // Diagnose
    Buf[0] = PN_CMD_DIAGNOSE;
    Buf[1] = 0x06;  // Attention request test
    Length = 2;
    if (WriteAndWaitData() == pnPktOK) {
        klPrintf("PN_CMD_DIAGNOSE: ");
        for (uint16_t i=0; i<Length; i++) klPrintf("%X ", Buf[i]);
        klPrintf("\r");
        if ((Buf[0] == 0x01) && (Buf[1] == 0x00)) return true;
    }
    return false;
}

uint8_t PN_t::MifareRead(void) {
    Card.DataLength = 0;
    Buf[0] = PN_CMD_IN_DATA_EXCHANGE;
    Buf[1] = 0x01;   // Tag ID
    Buf[2] = 0x30;
    Buf[3] = 0x00;
    Length = 4;
    PN_PktRslt_t rslt = WriteAndWaitData();
    if (rslt == pnPktOK) {
        if (Buf[1] == 0x00) {   // if status ok
            Card.DataLength = Length-2;
            klPrintf("<");
            for (uint16_t i=0; i<Card.DataLength; i++) klPrintf(" %X", Card.Data[i]);
            klPrintf("\r");

//            Buf[0] = PN_CMD_IN_DATA_EXCHANGE;
//            Buf[1] = 0x01;   // Tag ID
//            Buf[2] = 0xA2;
//            Buf[3] = 0x02;
//
//            Buf[4] = 0x00;
//            Buf[5] = 0x00;
//            Buf[6] = 0xF1;
//            Buf[7] = 0x00;
//
//            Length = 8;
//            rslt = WriteAndWaitData();
//
//            Card.DataLength = Length-2;
//            klPrintf("<");
//            for (uint16_t i=0; i<Card.DataLength; i++) klPrintf(" %X", Card.Data[i]);
//            klPrintf("\r");



//            if( (Card.Data[1] == 0xD9) &&
//                (Card.Data[2] == 0x10) &&
//                (Card.Data[3] == 0x45) &&
//                (Card.Data[4] == 0x0A) &&
//                (Card.Data[5] == 0xB4) &&
//                (Card.Data[6] == 0x24) ) {
//                Led.Off();
//            }
//            else Led.On();
//
//            _delay_ms(1107);

            return 0;
        } // if status ok
        klPrintf("Buf !=0\r");
        return 1;
    } // if pkt ok
    klPrintf("Rslt != Ok\r");
    return 2;
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

