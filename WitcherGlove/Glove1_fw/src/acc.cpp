/*
 * acc.cpp
 *
 *  Created on: 02.03.2013
 *      Author: kreyl
 */

#include "acc.h"

Acc_t Acc;

void Acc_t::Init() {
    // Init pwr
    PinSetupOut(GPIOA, 15, omPushPull);
    PinSet(GPIOA, 15);
    // Init Accs
    //IAcc[0].Init(GPIOA, 1, 3, 4);
    IAcc[6].Init(GPIOB, 13, 14, 15);
}


// =============================== SingleAcc_t =================================
void SingleAcc_t::Init(GPIO_TypeDef *PGPIO, uint16_t AScl, uint16_t ASda, uint16_t AIrq) {
    i2c.Init(PGPIO, AScl, ASda);
    Irq = AIrq;
    PinSetupIn(PGPIO, Irq, pudNone);

    // ==== Setup initial registers ====
    uint8_t BufW[6];
    // Setup High-Pass filter and acceleration scale
    BufW[0] = ACC_REG_XYZ_DATA_CFG;
    BufW[1] = 0x01; // No filter, scale = 4g
    i2c.WriteBuf(ACC_ADDR, BufW, 2);
    // Control registers
    BufW[0] = ACC_REG_CONTROL1; // CtrReg[0] is register address
    BufW[1] = 0x21;     // DR=100 => 50Hz data rate; Mode = Active
    BufW[2] = 0x00;     // Normal mode
    BufW[3] = 0x02;     // No IRQ; IRQ output active high
    BufW[4] = 0x00;     // All interrupts disabled
    BufW[5] = 0x04;     // FreeFall IRQ is routed to INT1 pin
    i2c.WriteBuf(ACC_ADDR, BufW, 6);

    uint8_t BufR[7];
    //BufW[0] = ACC_REG_STATUS;
    BufW[0] = 0x01;

    while(1) {
        chThdSleepMilliseconds(153);
        i2c.WriteReadBuf(ACC_ADDR, BufW, 1, BufR, 6);
        Uart.Printf("X: %u; Y: %u; Z: %u\r", BufR[0], BufR[2], BufR[4]);
    }


}

// ================================== i2c_t ====================================
void i2c_t::Init(GPIO_TypeDef *PGPIO, uint16_t AScl, uint16_t ASda) {
    // Copy init data
    GPIO = PGPIO;
    Scl = AScl;
    Sda = ASda;
    // Setup pins
    PinSetupOut(GPIO, Scl, omOpenDrain, pudNone);
    PinSetupOut(GPIO, Sda, omOpenDrain, pudNone);
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
    // Send repeated start
    Start();
    // Write Addr with Read bit (1)
    WriteByte((Addr<<1) | 0x01);
    if(!IsAcked()) {
        Stop();
        Uart.Printf("Addr NACK\r");
        return 4;
    }
    // Read data
    while(SzR) {
        *PR++ = ReadByte();
        if(SzR != 1) Ack();
        else Nack();
        SzR--;
    }
    Stop();
    return 0;
}
