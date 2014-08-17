/*
 * led.h
 *
 *  Created on: 03.11.2011
 *      Author: g.kruglov
 */

#ifndef LED_H_
#define LED_H_

#include "stm32f10x.h"
#include "ch.h"
#include "hal.h"
#include "kl_lib_f100.h"

#if 1 // ========================== LED_SMOOTH =================================
#define LED_SMOOTH          TRUE

#define LED_GPIO            GPIOB
#define LED_GPIO_N          9
#define LED_TIM_N           17
#define LED_CCR_N           1
#define LED_TOP_VALUE       1998
#define LED_BOTTOM_VALUE    0
#define LED_INITIAL_VALUE   LED_TOP_VALUE
#define LED_PWM_FREQ_HZ     999

#define SMOOTH_CONST        270

enum SmLedState_t {slsNone, slsGlimmer};
class LedSmooth_t {
private:
    VirtualTimer ITmr;
    PwmPin_t IPin;
    SmLedState_t IState;
    uint16_t IMax, IMin;
    uint16_t ICurrentValue, INeededValue;
    uint32_t ISetupDelay(uint16_t AValue) { return (uint32_t)((SMOOTH_CONST / (AValue+4)) + 1); }
public:
    void Init();
    void Set(uint16_t AValue) { IPin.Set(AValue); ICurrentValue = AValue; }
    void SetSmoothly(uint16_t AValue);
    bool IsEqOrAbove(uint16_t AValue) { return (ICurrentValue >= AValue); }
    bool IsEqOrBelow(uint16_t AValue) { return (ICurrentValue <= AValue); }
    void Glimmer(uint16_t AMax, uint16_t AMin);
    // Inner use
    void IrqHandlerI();
};

extern LedSmooth_t LedSmooth;
#endif


#if LED_RGB_ENABLE
#include "color.h"
/*
 * RGB LED
 * Provides smooth color change.
 */
#define LED_SETUP_DELAY_MS  45

#define LED_TIM         TIM2
#define LED_RED_CCR     CCR4
#define LED_GREEN_CCR   CCR3
#define LED_BLUE_CCR    CCR2
#define LED_RCC_EN()    rccEnableTIM2()

class LedRGB_t {
private:
	Color_t ICurrentColor, INeededColor;
	Thread *PThread;
	bool IsSleeping;
	void ISetRed  (uint8_t AValue) {LED_TIM->LED_RED_CCR   = AValue;}
	void ISetGreen(uint8_t AValue) {LED_TIM->LED_GREEN_CCR = AValue;}
	void ISetBlue (uint8_t AValue) {LED_TIM->LED_BLUE_CCR  = AValue;}
public:
	void Init();
	void Task();
	void SetColor(Color_t AColor) {
		ISetRed(AColor.Red);
		ISetGreen(AColor.Green);
		ISetBlue(AColor.Blue);
		ICurrentColor = AColor;
	}
	void SetColorSmoothly(Color_t AColor);
	void SetColorNow(Color_t AColor) {
	    SetColor(AColor);
	    INeededColor = AColor;
	}
	bool IsOff() { return (ICurrentColor == INeededColor) and (ICurrentColor == clBlack); }
//	void Blink(uint32_t ABlinkDelay, Color_t AColor) {
//	        IsInsideBlink = true;
//	        IBlinkDelay = ABlinkDelay;
//	        INeededColor = AColor;
//	        SetColor(AColor);
//	        Delay.Reset(&ITimer);
//	    }
};

extern LedRGB_t Led;
#endif

#endif /* LED_H_ */
