/*
 * peripheral.cpp
 *
 *  Created on: 18.01.2013
 *      Author: kreyl
 */

#include "peripheral.h"
#include "ch.h"
#include "hal.h"
#include "clocking.h"

// Variables
bool KeyPressed[KEY_COUNT];
PwrStatus_t PwrStatus;
IR_t IR;
Pill_t Pill[PILL_COUNT_MAX];
SouthBridge_t SouthBridge;

// =============================== Implementation ==============================
// Inner use
static inline void IResetLo() { PinClear(SB_GPIO, SB_RST); }
static inline void IResetHi() { PinSet  (SB_GPIO, SB_RST); }
static inline void SBUartInit();
// ==== Lcd Thread ====
static WORKING_AREA(waSBThread, 128);
static msg_t SBThread(void *arg) {
    (void)arg;
    chRegSetThreadName("Lcd");
    while(1) {
        // Stop thread if off
        if(SouthBridge.Status == sbsOff) chSchGoSleepS(THD_STATE_SUSPENDED);

    }
    return 0;
}

// ==== Southbridge ====
void SouthBridge_t::Init() {
    // Reset South Bridge
    PinSetupOut(GPIOC, 5, omPushPull);
    IResetLo();
    Status = sbsOff;
    // Create and start thread
    chThdCreateStatic(waSBThread, sizeof(waSBThread), NORMALPRIO, SBThread, NULL);
    On();   // Switch it on
}

void SouthBridge_t::On() {
    // Init inner variables
    Status = sbsOff;    // }
    FwVersion = 0;      // } Will be changed by receiving AnswerToReset
    // Init external structures
    for(uint8_t i=0; i<KEY_COUNT; i++) KeyPressed[i] = 0;
    PwrStatus.ExternalPwrOn = false;
    PwrStatus.IsCharging = false;
    PwrStatus.Voltage_mV = 0;
    IR.RxSize = 0;
    for(uint8_t i=0; i<PILL_COUNT_MAX; i++) Pill[i].Status = psNotConnected;
    // Init hardware
    SBUartInit();
    // Remove Reset => switch it on
    IResetHi();
}

void SouthBridge_t::Shutdown() {
    Status = sbsOff;
    IResetLo();
}

void SBUartInit() {
    // Pins
    PinSetupAlterFunc(SB_GPIO, SB_OUT, omPushPull,  pudNone,   AF8);
    PinSetupAlterFunc(SB_GPIO, SB_IN,  omOpenDrain, pudPullUp, AF8);
    // ==== USART init ====
    rccEnableUSART6(FALSE);         // Usart6 CLK, no clock in low-power
    SB_UART->CR1 = USART_CR1_UE;    // Enable UART
    SB_UART->CR2 = 0;
    SB_UART->BRR = SB_UART_CLK / SB_BAUDRATE;    // Baudrate
    SB_UART->CR1 =
            USART_CR1_UE |      // Enable USART
            USART_CR1_TE |      // Transmitter enable
            USART_CR1_RE;       // Receiver enable
    // After enabling transmitter, it will send empty character
    while(!(USART3->SR & USART_SR_TC));    // wait for transmission to complete
}
