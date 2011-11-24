/*
 * led.h
 *
 *  Created on: 03.11.2011
 *      Author: g.kruglov
 */

#ifndef LED_H_
#define LED_H_

//#include <avr/io.h>
#include <delay_util.h>

//#define LED_PORT        PORTB
//#define LED_DDR         DDRB
//#define LED_P           ((1<<PB4)|(1<<PB5)|(1<<PB6)|(1<<PB7))

class Led_t {
private:
    uint16_t Timer;
    bool IsOn;
public:
//    void Init(void)   { LED_DDR |= LED_P; On(); }
//    void Deinit(void) { Off(); }
   // void Task(void)   { if(!IsOn) if(DelayElapsed(&Timer, 126)) On(); }
    void Blink(void);
//    void On(void)     { LED_PORT |=  LED_P; IsOn = true;  }
//    void Off(void)    { LED_PORT &= ~LED_P; IsOn = false; }
//    void Toggle(void) { LED_PORT ^=  LED_P; IsOn = !IsOn; }
};

extern Led_t Led;


#endif /* LED_H_ */
