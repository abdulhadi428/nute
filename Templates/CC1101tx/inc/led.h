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

#define LED_BLINK_DELAY 144

class Led_t : public klPin_t {
private:
    uint32_t Timer;
    bool IsInsideBlink;
public:
    Led_t(GPIO_TypeDef* AGPIO, uint16_t APinNumber) : klPin_t(AGPIO, APinNumber, GPIO_Mode_Out_PP) { }
    void On(void)      { klPin_t::operator =(true); }
    void Off(void)     { klPin_t::operator =(false); }
    void Disable(void) { Off(); IsInsideBlink = false; }
    void Blink(void);
    void Task(void);
};

// Colors
struct Color_t {
    uint8_t Red, Green, Blue;
    bool operator == (const Color_t AColor) { return ((this->Red == AColor.Red) && (this->Green == AColor.Green) && (this->Blue == AColor.Blue)); }
    //bool operator != (const Color_t AColor) { return ((this->Red != AColor.Red) || (this->Green != AColor.Green) || (this->Blue != AColor.Blue)); }
    bool IsOn(void) const { return (Red || Green || Blue); }
};
#define clBlack     {0, 0, 0}

class RGBLed_t {
private:
    uint32_t Timer;
    Color_t CurrentColor, NeededColor;
public:
    void Init(void);
    void Task(void);
    void On(void);
    void Off(void);
    void SetColor(Color_t AColor);
    void SetColorSmoothly(Color_t AColor) { NeededColor = AColor; }
};


#endif /* LED_H_ */
