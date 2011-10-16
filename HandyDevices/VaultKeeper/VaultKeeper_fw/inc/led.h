/*
 * led.h
 *
 *  Created on: 15.10.2011
 *      Author: Kreyl
 */

#ifndef LED_H_
#define LED_H_

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "delay_util.h"


#define LED_ON_TIME     108

class Led_t {
private:
    uint32_t Timer;
    void On(void)  { GPIOA->BSRR = GPIO_Pin_0; }
    void Off(void) { GPIOA->BRR  = GPIO_Pin_0; }
public:
    void Init(void);
    void Task(void);
    void Blink(void) { On(); Delay.Reset(&Timer); }
};

extern Led_t Led;

#endif /* LED_H_ */
