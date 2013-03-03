/*
 * acc.cpp
 *
 *  Created on: 02.03.2013
 *      Author: kreyl
 */

#include "acc.h"

SingleAcc_t Acc[ACC_CNT];

void AccInit() {
    // Init pwr
    PinSetupOut(GPIOA, 15, omPushPull);
    PinSet(GPIOA, 15);
    chThdSleepMilliseconds(450);
    // Set ports & pins
    Acc[0].SetPortAndPins(GPIOA, 1, 3, 4);
    Acc[1].SetPortAndPins(GPIOA, 5, 6, 7);
    Acc[2].SetPortAndPins(GPIOC, 0, 1, 2);
    Acc[3].SetPortAndPins(GPIOC, 3, 4, 5);
    Acc[4].SetPortAndPins(GPIOC, 6, 7, 8);
    Acc[5].SetPortAndPins(GPIOC, 10, 11, 12);
    Acc[6].SetPortAndPins(GPIOB, 13, 14, 15);
    // Init
    for(uint8_t i=0; i<ACC_CNT; i++) {
        Acc[i].Init();
        if(!Acc[i].IsOperational) Uart.Printf("Acc %u is not operational\r", i);
    }
}


// =============================== SingleAcc_t =================================
void SingleAcc_t::Init() {
    i2c.Init(GPIO, Scl, Sda);
    IsOperational = false;
    PinSetupIn(GPIO, Irq, pudNone);

    // ==== Setup initial registers ====
    uint8_t BufW[6];
    uint8_t Rslt;
    // Setup High-Pass filter and acceleration scale
    BufW[0] = ACC_REG_XYZ_DATA_CFG;
    BufW[1] = 0x01; // No filter, scale = 4g
    Rslt = i2c.WriteBuf(ACC_ADDR, BufW, 2);
    if(Rslt) {
        IsOperational = false;
        return;
    }
    // Control registers
    BufW[0] = ACC_REG_CONTROL1; // CtrReg[0] is register address
    BufW[1] = 0x21;     // DR=100 => 50Hz data rate; Mode = Active
    BufW[2] = 0x00;     // Normal mode
    BufW[3] = 0x02;     // No IRQ; IRQ output active high
    BufW[4] = 0x00;     // All interrupts disabled
    BufW[5] = 0x04;     // FreeFall IRQ is routed to INT1 pin
    Rslt = i2c.WriteBuf(ACC_ADDR, BufW, 6);
    if(Rslt) {
        IsOperational = false;
        return;
    }
    IsOperational = true;
    ReadAccelerations();
}

static int16_t Convert(uint8_t HiByte, uint8_t LoByte) {
    int16_t w;
    w = HiByte;
    w <<= 8;
    w |= LoByte;
    w /= 16;
    return w;
}

void SingleAcc_t::ReadAccelerations() {
    if(!IsOperational) return;
    uint8_t BufW[1], BufR[6];
    BufW[0] = ACC_REG_OUT_X_MSB;
    if(i2c.WriteReadBuf(ACC_ADDR, BufW, 1, BufR, 6) != 0) {
        IsOperational = false;
        Uart.Printf("Fail\r");
    }
    // Convert received values to signed integers
    a[0] = Convert(BufR[0], BufR[1]);
    a[1] = Convert(BufR[2], BufR[3]);
    a[2] = Convert(BufR[4], BufR[5]);
}

// ================================== i2c_t ====================================
void i2c_t::Init(GPIO_TypeDef *PGPIO, uint16_t AScl, uint16_t ASda) {
    // Copy init data
    GPIO = PGPIO;
    Scl = AScl;
    Sda = ASda;
    // Setup pins
    PinSetupOut(GPIO, Scl, omOpenDrain, pudNone, ps100MHz);
    PinSetupOut(GPIO, Sda, omOpenDrain, pudNone, ps100MHz);
    SclHi();
    SdaHi();
}

uint8_t i2c_t::Start() {
    SclHi();
    SdaHi();
    IDelay();
    if(!SdaIsHi()) return 1;
    if(!SclIsHi()) return 2;
    SdaLo();
    IDelay();
    return 0;
}

void i2c_t::Stop() {
    SclLo();
    IDelay();
    SclHi();

    SdaLo();
    IDelay();
    SdaHi();
    IDelay();
}

void i2c_t::Ack() {
    SclLo();
    IDelay();
    SdaLo();
    IDelay();
    SclHi();
    IDelay();
    SclLo();
    IDelay();
}

void i2c_t::Nack() {
    SclLo();
    IDelay();
    SdaHi();
    IDelay();
    SclHi();
    IDelay();
    SclLo();
    IDelay();
}

void i2c_t::WriteByte(uint8_t b) {
    for(uint8_t i=0; i<8; i++) {
        SclLo();
        if(b & 0x80) SdaHi();
        else SdaLo();
        b <<= 1;
        IDelay();
        SclHi();
        IDelay();
    }
    SclLo();
}

uint8_t i2c_t::ReadByte() {
    uint8_t b = 0;
    SdaHi();
    for(uint8_t i=0; i<8; i++) {
        b <<= 1;
        SclLo();
        IDelay();
        SclHi();
        IDelay();
        if(SdaIsHi()) b |= 0x01;
    }
    SclLo();
    return b;
}

bool i2c_t::IsAcked() {
    SclLo();
    SdaHi();
    IDelay();
    SclHi();
    IDelay();
    bool r = !SdaIsHi();
    SclLo();
    return r;
}

uint8_t i2c_t::WriteBuf(uint8_t Addr, uint8_t *PW, uint16_t SzW) {
    if(Start() != 0) return 1;
    // Write Addr with Write bit (0)
    WriteByte(Addr<<1);
    if(!IsAcked()) {
        Stop();
        Uart.Printf("Addr NACK\r");
        return 2;
    }
    // Write data
    while(SzW--) {
        WriteByte(*PW++);
        if(!IsAcked()) {
            Stop();
            Uart.Printf("NACK\r");
            return 3;
        }
    }
    Stop();
    return 0;
}

uint8_t i2c_t::WriteReadBuf(uint8_t Addr, uint8_t *PW, uint16_t SzW, uint8_t *PR, uint16_t SzR) {
    uint8_t Rslt = 0;
    chSysLock();
    if(Start() != 0) {
        Rslt = 1;
        goto End;
    }
    // Write Addr with Write bit (0)
    WriteByte(Addr<<1);
    if(!IsAcked()) {
        Rslt = 2;
        Uart.Printf("Addr NACK\r");
        goto End;
    }
    chSysUnlock();

    // Write data
    chSysLock();
    while(SzW--) {
        WriteByte(*PW++);
        if(!IsAcked()) {
            Rslt = 3;
            Uart.Printf("NACK\r");
            goto End;
        }
    }
    chSysUnlock();

    // Send repeated start
    chSysLock();
    Start();
    // Write Addr with Read bit (1)
    WriteByte((Addr<<1) | 0x01);
    if(!IsAcked()) {
        Rslt = 4;
        Uart.Printf("Addr NACK\r");
        goto End;
    }
    chSysUnlock();

    // Read data
    chSysLock();
    while(SzR) {
        *PR++ = ReadByte();
        if(SzR != 1) Ack();
        else Nack();
        SzR--;
    }
    End:
    Stop();
    chSysUnlock();
    return Rslt;
}
