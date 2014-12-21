/*
 * led_rgb.cpp
 *
 *  Created on: 31 рту. 2014 у.
 *      Author: Kreyl
 */

#include <led_smooth.h>

LedSmooth_t Led;

// Timer callback
static void LedTmrCallback(void *p) {
    chSysLockFromIsr();
    Led.IAdjustBrightness();
    chSysUnlockFromIsr();
}

void LedSmooth_t::IAdjustBrightness() {
    if(CurrentBrt == DesiredBrt) return;
    else if(CurrentBrt < DesiredBrt) CurrentBrt++;
    else CurrentBrt--;
    LedCh.Set(CurrentBrt);
    chVTSetI(&ITmr, ICalcDelay(CurrentBrt, ISmoothVar), LedTmrCallback, nullptr);
}

void LedSmooth_t::FadeIn(const uint32_t ASmoothVar) {
    chSysLock();
    if(chVTIsArmedI(&ITmr)) chVTResetI(&ITmr);
    ISmoothVar = ASmoothVar;
    DesiredBrt = LED_TOP_VALUE;
    chVTSetI(&ITmr, ICalcDelay(CurrentBrt, ISmoothVar), LedTmrCallback, nullptr);
    chSysUnlock();
}

void LedSmooth_t::FadeOut(const uint32_t ASmoothVar) {
    chSysLock();
    if(chVTIsArmedI(&ITmr)) chVTResetI(&ITmr);
    ISmoothVar = ASmoothVar;
    DesiredBrt = 0;
    chVTSetI(&ITmr, ICalcDelay(CurrentBrt, ISmoothVar), LedTmrCallback, nullptr);
    chSysUnlock();
}


#if 1 // ============================= LED Channel =============================
void LedChnl_t::Init() const {
    // ==== GPIO setup ====
    PinSetupAlterFunc(PGpio, Pin, omPushPull, pudNone, AF);

    // ==== Timer setup ====
    if     (PTimer == TIM1)  { rccEnableTIM1(FALSE); }
    else if(PTimer == TIM2)  { rccEnableTIM2(FALSE); }
    else if(PTimer == TIM3)  { rccEnableTIM3(FALSE); }
    else if(PTimer == TIM14) { rccEnableAPB1(RCC_APB1ENR_TIM14EN, FALSE); }
    else if(PTimer == TIM15) { rccEnableAPB2(RCC_APB2ENR_TIM15EN, FALSE); }
    else if(PTimer == TIM16) { rccEnableAPB2(RCC_APB2ENR_TIM16EN, FALSE); }
    else if(PTimer == TIM17) { rccEnableAPB2(RCC_APB2ENR_TIM17EN, FALSE); }

    PTimer->ARR = LED_TOP_VALUE;
    PTimer->CR1 = TIM_CR1_CEN | TIM_CR1_ARPE; // Enable timer, set clk division to 0, AutoReload buffered
    PTimer->CR2 = 0;

    // ==== Timer's channel ====
#if LED_INVERTED_PWM
#define PwmMode 0b111
#else
#define PwmMode 0b110
#endif
    switch(TmrChnl) {
        case 1:
            PTimer->CCMR1 |= (PwmMode << 4);
            PTimer->CCER  |= TIM_CCER_CC1E;
            break;
        case 2:
            PTimer->CCMR1 |= (PwmMode << 12);
            PTimer->CCER  |= TIM_CCER_CC2E;
            break;
        case 3:
            PTimer->CCMR2 |= (PwmMode << 4);
            PTimer->CCER  |= TIM_CCER_CC3E;
            break;
        case 4:
            PTimer->CCMR2 |= (PwmMode << 12);
            PTimer->CCER  |= TIM_CCER_CC4E;
            break;
        default: break;
    }
}
#endif
