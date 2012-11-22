/*
 * led.h
 *
 *  Created on: 03.11.2011
 *      Author: g.kruglov
 */

#ifndef LED_H_
#define LED_H_

#include "stm32f10x.h"
#include "stm32f10x_tim.h"

#include "kl_lib.h"

class Led_t {
protected:
    uint16_t IPinNumber;
    GPIO_TypeDef* IGPIO;
public:
    void Init(GPIO_TypeDef *PGpioPort, uint16_t APinNumber) {
        IGPIO = PGpioPort;
        IPinNumber = APinNumber;
        klPinSetup(PGpioPort, APinNumber, pmOutPushPull);
    }
    void On(void)  { klPinSet  (IGPIO, IPinNumber); }
    void Off(void) { klPinClear(IGPIO, IPinNumber); }
};

class LedBlink_t : public Led_t {
private:
    uint32_t ITimer, IBlinkDelay;
    bool IsInsideBlink;
public:
    void Disable(void) { Off(); IsInsideBlink = false; }
    void Blink(uint32_t ABlinkDelay);
    void Task(void);
};

class LedBlinkInverted_t {
private:
    uint32_t ITimer, IBlinkDelay;
    bool IsInsideBlink;
    uint16_t IPinNumber;
    GPIO_TypeDef* IGPIO;
public:
    void Init(GPIO_TypeDef *PGpioPort, uint16_t APinNumber) {
        IGPIO = PGpioPort;
        IPinNumber = APinNumber;
        klPinSetup(PGpioPort, APinNumber, pmOutPushPull);
    }
    void On(void)  { klPinClear(IGPIO, IPinNumber); }
    void Off(void) { klPinSet  (IGPIO, IPinNumber); }
    void Disable(void) { Off(); IsInsideBlink = false; }
    void Blink(uint32_t ABlinkDelay) {
        IsInsideBlink = true;
        IBlinkDelay = ABlinkDelay;
        On();
        Delay.Reset(&ITimer);
    }
    void Task(void) {
        if (!IsInsideBlink) return;
        if (Delay.Elapsed(&ITimer, IBlinkDelay)) {
            IsInsideBlink = false;
            Off();
        }
    }
};

// Adjust delay depending on value
static inline uint32_t SetupSmoothDelay(uint16_t AValue) {
    return (uint32_t)(700 / (AValue+10));
}

// Smooth on & off.
class LedSmooth_t {
private:
    uint32_t IDelayTmr, IDelay;
    TIM_TypeDef* IPwmTimer;
    uint16_t *ICCR, IValue, INeededValue;
public:
    void Init(GPIO_TypeDef *AGpio, uint16_t APinNumber, TIM_TypeDef* ATimer, uint16_t ATopValue, uint16_t APrescaler, uint8_t AChannelNumber, bool InvertedPolarity);
    void Task(void);
    void Set(uint16_t AValue) { *ICCR = AValue; IValue = AValue; }
    void SetSmoothly(uint16_t AValue) { INeededValue = AValue; }
};

// Colors
struct Color_t {
    uint8_t Red, Green, Blue;
    bool operator == (const Color_t AColor) { return ((Red == AColor.Red) and (Green == AColor.Green) and (Blue == AColor.Blue)); }
    //bool operator != (const Color_t AColor) { return ((this->Red != AColor.Red) || (this->Green != AColor.Green) || (this->Blue != AColor.Blue)); }
};
#define clBlack     ((Color_t){0,   0,   0})
#define clRed       ((Color_t){255, 0,   0})
#define clGreen     ((Color_t){0,   255, 0})
#define clBlue      ((Color_t){0,   0,   255})
#define clYellow    ((Color_t){255, 255, 0})

/*
 * RGB LED
 * Provides smooth color change.
 */

class LedRGB_t {
private:
    uint32_t ITimer, IBlinkDelay;
    bool IsInsideBlink;
	Color_t ICurrentColor, INeededColor;
	void ISetRed  (uint8_t AValue) {TIM1->CCR3 = AValue;}
	void ISetGreen(uint8_t AValue) {TIM1->CCR1 = AValue;}
	void ISetBlue (uint8_t AValue) {TIM1->CCR4 = AValue;}
public:
	void Init(void);
	void Task(void);
	void SetColor(Color_t AColor) {
		ISetRed(AColor.Red);
		ISetGreen(AColor.Green);
		ISetBlue(AColor.Blue);
		ICurrentColor = AColor;
	}
	void SetColorSmoothly(Color_t AColor) { INeededColor = AColor; }
	void Blink(uint32_t ABlinkDelay, Color_t AColor) {
	        IsInsideBlink = true;
	        IBlinkDelay = ABlinkDelay;
	        INeededColor = AColor;
	        SetColor(AColor);
	        Delay.Reset(&ITimer);
	    }
};


#endif /* LED_H_ */
