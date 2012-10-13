/*
 * lis3d.cpp
 *
 *  Created on: 09.10.2012
 *      Author: kreyl
 */

#include "lis3d.h"

Acc_t Acc;

// Returns 0 if OK and 1 if error
uint8_t Acc_t::Init() {
    // GPIOs
    klPinSetup(GPIOA, ACC_CLK,  pmOutPushPull);
    klPinSetup(GPIOA, ACC_DODI, pmOutPushPull);
    klPinSetup(GPIOA, ACC_CS,   pmOutPushPull);
    klPinSetup(GPIOA, ACC_IRQ,  pmInPullDown);
    CsHi();
    ClkHi();
    // Send initial commands
    WriteReg(CTRL_REG1, 0b00000111 | ACC_DATARATE);    // ACC_DATARATE Hz, normal mode, all axis enabled
    WriteReg(CTRL_REG2, 0b00000000);    // Filter bypassed
    WriteReg(CTRL_REG3, 0b00000100);    // FIFO watermark irq
//    WriteReg(CTRL_REG4, 0b00100000);    // cont update, LSB at L reg, +-8g, no HiRes, no SelfTest, 4-wire
    WriteReg(CTRL_REG4, 0b00100001);    // cont update, LSB at L reg, +-8g, no HiRes, no SelfTest, 3-wire
    WriteReg(CTRL_REG5, 0b01000000);    // NormMode, FIFO enabled, Irq not latched, no 4D
    WriteReg(CTRL_REG6, 0b00000000);    // No IRQ2
    WriteReg(FIFO_CTRL_REG, 0b10000000 | ACC_FIFO_SZ);// FIFO mode = Stream, trigger linked to Int1, FIFO Threshold = ACC_FIFO_SZ
    WriteReg(INT1_CFG, 0b00000000);     // Irqs disabled
    WriteReg(INT1_SOURCE, 0b00000000);  // Irqs disabled
    // Read WhoAmI register to check connection
    uint8_t b = ReadReg(WHO_AM_I);
    //Uart.Printf("Wai: %X\r", b);
    return (b == 0x33)? 0 : 1;
}

void Acc_t::Read() {
    // Read data ACC_FIFO_SZ times
    for(uint8_t n=0; n<ACC_FIFO_SZ; n++) {
        // Read 3 Axis data
        CsLo();   // Select chip
        WriteByte(OUT_X_L | 0xC0);
        ReadNBytes((uint8_t*)&Axis[n][0], 6);
        CsHi();
        // Shift data right
        for(uint8_t i=0; i<3; i++) Axis[n][i] >>= 6;
        Uart.Printf("X: %d; Y: %d; Z: %d\r", Axis[n][0], Axis[n][1], Axis[n][2]);
    }
    Uart.Printf("\r");
}

// ================================= Low-level =================================
uint8_t Acc_t::ReadReg(uint8_t AAddr) {
    CsLo();   // Select chip
    WriteByte(AAddr | 0x80);    // Set R/W bit to 1, left M/S 0
    uint8_t r;
    ReadNBytes(&r, 1);
    CsHi();
    return r;
}

void Acc_t::WriteReg(uint8_t AAddr, uint8_t AValue) {
    CsLo();   // Select chip
    WriteByte(AAddr);    // Left R/W bit 0, left M/S 0
    WriteByte(AValue);
    CsHi();
}


inline void Acc_t::ReadNBytes(uint8_t *PDst, uint8_t N) {
    // Switch MOSI to input
    klPinInPullDown(GPIOA, ACC_DODI);
    while(N) {
        uint8_t b=0;
        for(uint8_t i=0; i<8; i++) {
            ClkLo();
            b <<= 1;
            if(DodiIsHi()) b |= 0x01;
            ClkHi();
        } // for
        *PDst = b;
        PDst++;
        N--;
    } // while
    // Switch MOSI back to output
    klPinOutPushPull(GPIOA, ACC_DODI);
}

inline void Acc_t::WriteByte(uint8_t AByte) {
    for(uint8_t i=0; i<8; i++) {
        ClkLo();
        if(AByte & 0x80) DodiHi();
        else DodiLo();
        AByte <<= 1;
        ClkHi();
    }
}
