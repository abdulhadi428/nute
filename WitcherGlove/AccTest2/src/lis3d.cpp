/*
 * lis3d.cpp
 *
 *  Created on: 09.10.2012
 *      Author: kreyl
 */

#include "lis3d.h"

Acc_t Acc;

void Acc_t::Init() {
    // GPIOs
    klGpioSetupByMsk(GPIOA, ACC_CLK | ACC_MOSI | ACC_CS, GPIO_Mode_Out_PP);
    klGpioSetupByMsk(GPIOA, ACC_MISO, GPIO_Mode_IPD);

    CsHi();
    ClkHi();

    Delay.ms(10);

    // Send initial commands
    WriteReg(CTRL_REG1, 0b00100111);    // 10 Hz, normal mode, all axis enabled
    WriteReg(CTRL_REG2, 0b00000000);    // Filter bypassed
//    while(1);
    WriteReg(CTRL_REG3, 0b00000000);    // IRQs disabled
    WriteReg(CTRL_REG4, 0b00100000);    // cont update, LSB at L reg, +-8g, no HiRes, no SelfTest, 4-wire
    //WriteReg(CTRL_REG5, 0b00000000);    // NormMode, FIFO disabled, Irq not latched, no 4D
    WriteReg(CTRL_REG5, 0b01000000);    // NormMode, FIFO enabled, Irq not latched, no 4D
    //WriteReg(CTRL_REG6, 0b00000000);    // ?? no doc
    WriteReg(FIFO_CTRL_REG, 0b10000000);// FIFO mode = Stream, trigger linked to Int1, FIFO Threshold = 0
    //WriteReg(FIFO_SRC_REG, 0b10000000); // ??
    WriteReg(INT1_CFG, 0b00000000);     // Irqs disabled
    WriteReg(INT1_SOURCE, 0b00000000);  // Irqs disabled
    uint8_t b = ReadReg(WHO_AM_I);
    Uart.Printf("Wai: %X\r", b);
}

void Acc_t::Read() {
    // Read 3 Axis data
    //Acceleration_t acc;
    uint16_t w[3];
    CsLo();   // Select chip
//    WriteByte(OUT_X_H | 0xC0);    // Set R/W bit to 1, set M/S to 1
//    ReadNBytes((uint8_t*)&acc, 1);
    WriteByte(OUT_X_L | 0xC0);
    ReadNBytes((uint8_t*)&w, 6);
    CsHi();
//    bool IsNegative = (w > 0x7FFF);
//    w >>= 6;
    int16_t a[3];
    for(uint8_t i=0; i<3; i++) {
        a[i] = w[i];
        a[i] >>= 6;
    }
    Uart.Printf("w: %04X; a: %d\r", w[2], a[2]);
    //Uart.Printf("X: %d; Y: %d; Z: %d\r", acc.x, acc.y, acc.z);
    //Uart.Printf("X: %04X; Y: %04X; Z: %04X\r", acc.x, acc.y, acc.z);
//    for(uint8_t i=0; i<ACC_FIFO_SZ; i++) {
//
//    }
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
    while(N) {
        uint8_t b=0;
        for(uint8_t i=0; i<8; i++) {
            ClkLo();
            b <<= 1;
            if(MisoIsHi()) b |= 0x01;
            ClkHi();
        } // for
        *PDst = b;
        PDst++;
        N--;
    } // while
}

inline uint16_t Acc_t::ReadWord() {
    uint16_t r=0;
    for(uint8_t i=0; i<8; i++) {
        ClkLo();
        r <<= 1;
        if(MisoIsHi()) r |= 0x01;
        ClkHi();
    }
    return r;
}

inline void Acc_t::WriteByte(uint8_t AByte) {
    for(uint8_t i=0; i<8; i++) {
        ClkLo();
        if(AByte & 0x80) MosiHi();
        else MosiLo();
        AByte <<= 1;
        ClkHi();
    }
}
