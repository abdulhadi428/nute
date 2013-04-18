/*
 * kl_lib_f0.cpp
 *
 *  Created on: 10.12.2012
 *      Author: kreyl
 */

#include "kl_lib_f2xx.h"
#include <stdarg.h>
#include <string.h>
#include "tiny_sprintf.h"

// ================================ PWM pin ====================================
void PwmPin_t::Init(GPIO_TypeDef *GPIO, uint16_t N, uint8_t TimN, uint8_t Chnl, uint16_t TopValue, bool Inverted) {
    switch(TimN) {
        case 1:
            Tim = TIM1;
            PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF1);
            rccEnableTIM1(FALSE);
            break;
        case 2:
            Tim = TIM2;
            PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF1);
            rccEnableTIM2(FALSE);
            break;

        case 3:
            Tim = TIM3;
            PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF2);
            rccEnableTIM3(FALSE);
            break;
        case 4:
            Tim = TIM4;
            PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF2);
            rccEnableTIM4(FALSE);
            break;
        case 5:
            Tim = TIM5;
            PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF2);
            rccEnableTIM5(FALSE);
            break;

        case 8:
            Tim = TIM8;
            PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF3);
            rccEnableTIM8(FALSE);
            break;
        case 9:
            Tim = TIM9;
            PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF3);
            rccEnableAPB2(RCC_APB2ENR_TIM9EN, FALSE);
            break;
        case 10:
            Tim = TIM10;
            PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF3);
            rccEnableAPB2(RCC_APB2ENR_TIM10EN, FALSE);
            break;
        case 11:
            Tim = TIM11;
            PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF3);
            rccEnableAPB2(RCC_APB2ENR_TIM11EN, FALSE);
            break;

        case 12:
            Tim = TIM12;
            PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF9);
            rccEnableAPB1(RCC_APB1ENR_TIM12EN, FALSE);
            break;
        case 13:
            Tim = TIM13;
            PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF9);
            rccEnableAPB1(RCC_APB1ENR_TIM13EN, FALSE);
            break;
        case 14:
            Tim = TIM14;
            PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF9);
            rccEnableAPB1(RCC_APB1ENR_TIM14EN, FALSE);
            break;

        default: return; break;
    }

    // Clock src
    if((TimN == 1) or (TimN == 8) or (TimN == 9) or (TimN == 10) or (TimN == 11))
        PClk = &Clk.APB2FreqHz;
    else PClk = &Clk.APB1FreqHz;

    // Common
    Tim->CR1 = TIM_CR1_CEN; // Enable timer, set clk division to 0, AutoReload not buffered
    Tim->CR2 = 0;
    Tim->ARR = TopValue;
    Tim->BDTR = TIM_BDTR_MOE | TIM_BDTR_AOE;

    // Output
    uint16_t tmp = Inverted? 0b111 : 0b110; // PWM mode 1 or 2
    switch(Chnl) {
        case 1:
            PCCR = &Tim->CCR1;
            Tim->CCMR1 |= (tmp << 4);
            Tim->CCER  |= TIM_CCER_CC1E;
            break;

        case 2:
            PCCR = &Tim->CCR2;
            Tim->CCMR1 |= (tmp << 12);
            Tim->CCER  |= TIM_CCER_CC2E;
            break;

        case 3:
            PCCR = &Tim->CCR3;
            Tim->CCMR2 |= (tmp << 4);
            Tim->CCER  |= TIM_CCER_CC3E;
            break;

        case 4:
            PCCR = &Tim->CCR4;
            Tim->CCMR2 |= (tmp << 12);
            Tim->CCER  |= TIM_CCER_CC4E;
            break;

        default: break;
    }
    *PCCR = 0;
}

void PwmPin_t::SetFreqHz(uint32_t FreqHz) {
    uint32_t divider = Tim->ARR * FreqHz;
    if(divider == 0) return;
    uint32_t FPrescaler = *PClk / divider;
    if(FPrescaler != 0) FPrescaler--;   // do not decrease in case of high freq
    Tim->PSC = (uint16_t)FPrescaler;
}

// ================================= DEBUG =====================================
//void chDbgPanic(const char *msg1) {
//    //Uart.PrintNow(msg1);
//    (void)msg1;
//}

// =============================== I2C =========================================
void i2c_t::Init(I2C_TypeDef *pi2c, GPIO_TypeDef *PGpio, uint16_t SclPin, uint16_t SdaPin) {
    ii2c = pi2c;
    IPGpio = PGpio;
    ISclPin = SclPin;
    ISdaPin = SdaPin;
    Standby();
    Resume();
}

void i2c_t::Standby() {
    if      (ii2c == I2C1) { rccResetI2C1(); rccDisableI2C1(FALSE); }
    else if (ii2c == I2C2) { rccResetI2C2(); rccDisableI2C2(FALSE); }
    else if (ii2c == I2C3) { rccResetI2C3(); rccDisableI2C3(FALSE); }
    // Disable GPIOs
    PinSetupAnalog(IPGpio, ISclPin);
    PinSetupAnalog(IPGpio, ISdaPin);
}

void i2c_t::Resume() {
    Error = false;
    // GPIOs
    PinSetupAlterFunc(IPGpio, ISclPin, omOpenDrain, pudNone, AF4);
    PinSetupAlterFunc(IPGpio, ISdaPin, omOpenDrain, pudNone, AF4);
    // Clock and reset
    if      (ii2c == I2C1) { rccEnableI2C1(FALSE); rccResetI2C1(); }
    else if (ii2c == I2C2) { rccEnableI2C2(FALSE); rccResetI2C2(); }
    else if (ii2c == I2C3) { rccEnableI2C3(FALSE); rccResetI2C3(); }
    // Minimum clock is 2 MHz
    uint32_t ClkMhz = Clk.APB1FreqHz / 1000000;
    uint16_t tmpreg = ii2c->CR2;
    tmpreg &= (uint16_t)~I2C_CR2_FREQ;
    if(ClkMhz < 2)  ClkMhz = 2;
    if(ClkMhz > 30) ClkMhz = 30;
    tmpreg |= ClkMhz;
    ii2c->CR2 = tmpreg;
    ii2c->CR1 &= (uint16_t)~I2C_CR1_PE; // Disable i2c to setup TRise & CCR
    ii2c->TRISE = (uint16_t)(((ClkMhz * 300) / 1000) + 1);
    // 16/9
    tmpreg = (uint16_t)((Clk.APB1FreqHz / 400000) * 25);
    if(tmpreg == 0) tmpreg = 1; // minimum allowed value
    ii2c->CCR = I2C_CCR_FS | I2C_CCR_DUTY | tmpreg;
    ii2c->CR1 |= I2C_CR1_PE;    // Enable i2c back
}

void i2c_t::Reset() {
    Standby();
    Resume();
}

uint8_t i2c_t::CmdWriteRead(uint8_t Addr, uint8_t *WPtr, uint8_t WLength, uint8_t *RPtr, uint8_t RLength) {
    if(IBusyWait() != OK) return FAILURE;
    // Clear flags
    ii2c->SR1 = 0;
    while(RxIsNotEmpty()) (void)ii2c->DR;   // Read DR until it empty
    ClearAddrFlag();
    // Start transmission
    SendStart();
    if(WaitEv5() != OK) return FAILURE;
    SendAddrWithWrite(Addr);
    if(WaitEv6() != OK) return FAILURE;

    return OK;
}

// Flag operations
uint8_t i2c_t::IBusyWait() {
    uint8_t RetryCnt = 4;
    while(RetryCnt--) {
        if(!(ii2c->SR2 & I2C_SR2_BUSY)) return OK;
        chThdSleepMilliseconds(1);
    }
    Error = true;
    return TIMEOUT;
}

uint8_t i2c_t::WaitEv5() {
    return FAILURE;
}











