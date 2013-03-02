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
    uint8_t CtrlRegs[6];
    while(1) {
        // Setup High-Pass filter and acceleration scale
        CtrlRegs[0] = ACC_REG_XYZ_DATA_CFG;
        CtrlRegs[1] = 0x01; // No filter, scale = 4g
        i2c.WriteBuf(ACC_ADDR, CtrlRegs, 2);
        chThdSleepMilliseconds(99);
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
}

bool i2c_t::IsAcked() {
    SclLo();
    SdaHi();
    IDelay();
    SclHi();
    IDelay();
    return !SdaIsHi();
}

uint8_t i2c_t::WriteBuf(uint8_t Addr, uint8_t *P, uint16_t Sz) {
    uint8_t Rslt=0;
    if((Rslt = Start()) != 0) return Rslt;
    // Write Addr with Write bit (0)
    WriteByte(Addr<<1);
    // Write data
    if(IsAcked()) {
        while(Sz--) {
            WriteByte(*P++);
            if(!IsAcked()) {
                Uart.Printf("NACK\r");
                Rslt = 2;
                break;
            }
        }
    }
    else {
        Rslt = 1;
        Uart.Printf("Addr NACK\r");
    }

    Stop();
    return Rslt;
}
