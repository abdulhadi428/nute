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

// Colors
struct Color_t {
    uint8_t Red, Green, Blue, White;
    bool operator == (const Color_t AColor) { return ((Red == AColor.Red) and (Green == AColor.Green) and (Blue == AColor.Blue) and (White == AColor.White)); }
    //bool operator != (const Color_t AColor) { return ((this->Red != AColor.Red) || (this->Green != AColor.Green) || (this->Blue != AColor.Blue)); }
};
#define clBlack     ((Color_t){0,   0,   0,   0})
#define clRed       ((Color_t){255, 0,   0,   0})
#define clGreen     ((Color_t){0,   255, 0,   0})
#define clBlue      ((Color_t){0,   0,   255, 0})
#define clYellow    ((Color_t){255, 255, 0,   0})
#define clWhite     ((Color_t){0,   0,   0, 255})

/*
 * RGB LED
 * Provides smooth color change.
 */
class LedRGBW_t {
private:
    uint32_t ITimer;
	void ISetRed  (uint8_t AValue) {TIM4->CCR4 = AValue;}
	void ISetGreen(uint8_t AValue) {TIM4->CCR3 = AValue;}
	void ISetBlue (uint8_t AValue) {TIM3->CCR4 = AValue;}
	void ISetWhite(uint8_t AValue) {TIM2->CCR2 = AValue;}
public:
    Color_t CurrentColor, NeededColor;
	void Init();
	void Task();
	void SetColor(Color_t AColor) {
		ISetRed(AColor.Red);
		ISetGreen(AColor.Green);
		ISetBlue(AColor.Blue);
		ISetWhite(AColor.White);
		CurrentColor = AColor;
	}
	void SetColorSmoothly(Color_t AColor) { NeededColor = AColor; }
};


#endif /* LED_H_ */
