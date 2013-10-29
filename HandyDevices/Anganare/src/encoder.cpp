/*
 * encoder.cpp
 *
 *  Created on: 25 окт. 2013 г.
 *      Author: r.leonov
 */

#include "encoder.h"
#include "load.h"
#include "lcd.h"

encoder_t Encoder;
// =============================== Thread ======================================
static WORKING_AREA(waEncThread, 256);
__attribute__ ((__noreturn__))
static void EncThread(void *arg) {
    chRegSetThreadName("EncoderTask");
    while(1) Encoder.Task();
}

void encoder_t::Init() {
    // Init Gpios
    InitGpios();
    // Init Encoder part of TIM
    rccEnableTIM3(false); // enable Clock to TIM3
    ENCODER_TIM->CCER   = TIM_CCER_CC1P | TIM_CCER_CC2P;
    ENCODER_TIM->CCMR1  = TIM_CCMR1_CC2S_0 | TIM_CCMR1_CC1S_0;
    ENCODER_TIM->SMCR   = TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;
    // Sampling filter & Prescaler
    ENCODER_TIM->CCMR1  = TIM_CCMR1_IC1PSC | TIM_CCMR1_IC1F | TIM_CCMR1_IC2PSC | TIM_CCMR1_IC2F;
    ENCODER_TIM->ARR    = 100;
    ENCODER_TIM->CNT    = 5;
    ENCODER_TIM->CR1    = TIM_CR1_CEN;

    ENCODER_TIM->DIER   = TIM_DIER_CC1IE | TIM_DIER_CC2IE;
    PCnt = (uint32_t*)&ENCODER_TIM->CNT;
    Value = 49;
    // Create thread
    chThdCreateStatic(waEncThread, sizeof(waEncThread), NORMALPRIO, (tfunc_t)EncThread, NULL);
}

void encoder_t::Task() {
    if(CurrentValue != *PCnt) {
    	if(CurrentValue < *PCnt) {
    		if(Value++ >= MAX_VALUE) Value = MIN_VALUE;
    	}
		else if(Value-- <= MIN_VALUE) Value = MAX_VALUE;
        CurrentValue = *PCnt;
        Load.TimeToWork = Value * 100;   // To ms
        Uart.Printf("Time=%ums\r", Load.TimeToWork);
        Lcd.Printf(0,0, "%u.%us", (Load.TimeToWork/1000), (Load.TimeToWork%1000)/100);
    }
    chThdSleepMilliseconds(45);
}
