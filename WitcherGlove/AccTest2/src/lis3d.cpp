/*
 * lis3d.cpp
 *
 *  Created on: 09.10.2012
 *      Author: kreyl
 */

#include "lis3d.h"

Acc_t Acc;




// Returns 0 if OK and 1 if error
void Acc_t::Init() {
    for(uint8_t n=0; n<ACC_COUNT; n++) {
        Uart.Printf("%u: ", n);
        Accs[n].Init();
    }
}

void Acc_t::Task() {
    // Read data ACC_FIFO_SZ times
    for(uint8_t n=0; n<ACC_COUNT; n++) {
        if(Accs[n].IrqIsHi()) {
            //Uart.Printf("%u ", n);
            Accs[n].Read(&a[n*3]);
        }
    }

    // Print data
    static uint32_t Tmr;
    if(Delay.Elapsed(&Tmr, 4)) {
        for(uint8_t n=0; n<6; n++) {
            for(uint8_t i=0; i<3; i++) Uart.Printf(" %d", a[3*n+i]);
            Uart.Printf(";");
        }
        Uart.Printf("\r\n");
    }
}

// ================================= Single acc ================================
void SingleAcc_t::Read(int16_t *a) {
    // Read 3 Axis data
    CsLo();   // Select chip
    WriteByte(OUT_X_L | 0xC0);  // Read many
    ReadNBytes((uint8_t*)a, 6);
    CsHi();
    // Shift data right
    for(uint8_t i=0; i<3; i++) a[i] >>= 6;
    //Uart.Printf("X: %d; Y: %d; Z: %d\r", a[0], a[1], a[2]);
}

void SingleAcc_t::Init() {
    // GPIOs
    klPinSetup(PGpio, Clk, pmOutPushPull);
    klPinSetup(PGpio, Io,  pmOutPushPull);
    klPinSetup(PGpio, Cs,  pmOutPushPull);
    klPinSetup(PGpio, Irq, pmInPullDown);
    CsHi();
    ClkHi();
    // Send initial commands
    WriteReg(CTRL_REG1, 0b00000111 | ACC_DATARATE);    // ACC_DATARATE Hz, normal mode, all axis enabled
    WriteReg(CTRL_REG2, 0b00000000);    // Filter bypassed
    //WriteReg(CTRL_REG3, 0b00000100);    // FIFO watermark irq
    WriteReg(CTRL_REG3, 0b00010000);    // Data ready irq
//    WriteReg(CTRL_REG4, 0b00100000);    // cont update, LSB at L reg, +-8g, no HiRes, no SelfTest, 4-wire
    WriteReg(CTRL_REG4, 0b00100001);    // cont update, LSB at L reg, +-8g, no HiRes, no SelfTest, 3-wire
    //WriteReg(CTRL_REG5, 0b01000000);    // NormMode, FIFO enabled, Irq not latched, no 4D
    WriteReg(CTRL_REG5, 0b00000000);    // NormMode, FIFO disabled, Irq not latched, no 4D
    WriteReg(CTRL_REG6, 0b00000000);    // No IRQ2
    //WriteReg(FIFO_CTRL_REG, 0b10000000 | ACC_FIFO_SZ);// FIFO mode = Stream, trigger linked to Int1, FIFO Threshold = ACC_FIFO_SZ
    WriteReg(FIFO_CTRL_REG, 0b00000000);// FIFO mode = Bypass
    WriteReg(INT1_CFG, 0b00000000);     // Irqs disabled
    WriteReg(INT1_SOURCE, 0b00000000);  // Irqs disabled
    // Read WhoAmI register to check connection
    uint8_t b = ReadReg(WHO_AM_I);
    Uart.Printf("Wai: %X\r", b);
//    return (b == 0x33)? 0 : 1;
}

uint8_t SingleAcc_t::ReadReg(uint8_t AAddr) {
    CsLo();   // Select chip
    WriteByte(AAddr | 0x80);    // Set R/W bit to 1, left M/S 0
    uint8_t r;
    ReadNBytes(&r, 1);
    CsHi();
    return r;
}

void SingleAcc_t::WriteReg(uint8_t AAddr, uint8_t AValue) {
    CsLo();   // Select chip
    WriteByte(AAddr);    // Left R/W bit 0, left M/S 0
    WriteByte(AValue);
    CsHi();
}

inline void SingleAcc_t::ReadNBytes(uint8_t *PDst, uint8_t N) {
    // Switch MOSI to input
    IoMakeIn();
    while(N) {
        uint8_t b=0;
        for(uint8_t i=0; i<8; i++) {
            ClkLo();
            b <<= 1;
            if(IoIsHi()) b |= 0x01;
            ClkHi();
        } // for
        *PDst = b;
        PDst++;
        N--;
    } // while
    // Switch MOSI back to output
    IoMakeOut();
}

inline void SingleAcc_t::WriteByte(uint8_t AByte) {
    for(uint8_t i=0; i<8; i++) {
        ClkLo();
        if(AByte & 0x80) IoHi();
        else IoLo();
        AByte <<= 1;
        ClkHi();
    }
}
