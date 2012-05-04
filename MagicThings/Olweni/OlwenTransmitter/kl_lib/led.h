/*
 * led.h
 *
 *  Created on: 03.11.2011
 *      Author: g.kruglov
 */

#ifndef LED_H_
#define LED_H_

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"

#include "kl_lib.h"

class Led_t {
private:
    uint16_t IPinNumber;
    GPIO_TypeDef* IGPIO;
public:
    Led_t(GPIO_TypeDef *PGpioPort, uint16_t APinNumber) { Init(PGpioPort, APinNumber); }
    Led_t(void) { }
    void Init(GPIO_TypeDef *PGpioPort, uint16_t APinNumber) { IGPIO = PGpioPort; IPinNumber = APinNumber; klGpioSetupByN(PGpioPort, APinNumber, GPIO_Mode_Out_PP); }
    void On(void)  { klGpioSetByN  (IGPIO, IPinNumber); }
    void Off(void) { klGpioClearByN(IGPIO, IPinNumber); }
};

class LedBlink_t : public Led_t {
private:
    uint32_t ITimer, IBlinkDelay;
    bool IsInsideBlink;
public:
    LedBlink_t(void) { }
    LedBlink_t(GPIO_TypeDef *PGpioPort, uint16_t APinNumber) { Init(PGpioPort, APinNumber); }
    void Disable(void) { Off(); IsInsideBlink = false; }
    void Blink(uint32_t ABlinkDelay);
    void Task(void);
};

/*
 * Smooth on & off.
 * Class is stand-alone as ordinal On & Off methods will not work with Alternate Function pin.
 */
enum LedState_t {lsOff, lsOn, lsUp, lsDown};
class LedSmooth_t {
private:
    uint32_t IDelayTmr, IDelay;
    TIM_TypeDef* IPwmTimer;
    uint16_t *ICCR, IValue;
    void ISetupDelay(void) {
        if (IValue < 11) IDelay = 81;
        else if (IValue < 27) IDelay = 36;
        else IDelay = 9;
    }
public:
    LedState_t State;
    void Init(GPIO_TypeDef *AGpio, uint16_t APinNumber, TIM_TypeDef* ATimer, uint16_t ATopValue, uint16_t APrescaler, uint8_t AChannelNumber, bool InvertedPolarity);
    void Task(void);
    void Set(uint16_t AValue) { *ICCR = AValue; IValue = AValue; }
    uint16_t Top(void)        { return (IPwmTimer->ARR); }
    void On(void)       { Set(Top()); State = lsOn; }
    void Off(void)      { Set(0); State = lsOff; }
    void RampUp(void);
    void RampDown(void);
};


#endif /* LED_H_ */
