/*
 * keys.cpp
 *
 *  Created on: 26.11.2011
 *      Author: Kreyl
 */

#include "keys.h"

Keys_t Keys;

Key_t Keys_t::Up(GPIOB, GPIO_Pin_3);
Key_t Keys_t::Down(GPIOB, GPIO_Pin_4);
Key_t Keys_t::Enter(GPIOB, GPIO_Pin_5);

Key_t *Keys_t::keys[] = {
	&Keys_t::Up,
	&Keys_t::Down,
	&Keys_t::Enter
};

void Keys_t::Init() {
	Delay.Reset(&timer);

    for (unsigned int i = 0; i < countof(keys); i++)
        klGpioSetupByMsk(keys[i]->PGpioPort, keys[i]->PinMask, GPIO_Mode_IPU);
}

void Keys_t::Task() {
    if (!Delay.Elapsed(&timer, KEY_DELAY)) return;
    for (unsigned int i = 0; i < countof(keys); i++) {
    	Key_t &key = *keys[i];
    	key.prev_pressed = key.is_pressed;
    	key.is_pressed = klGpioIsClearByMsk(key.PGpioPort, key.PinMask);
    	if (key.is_pressed && !key.prev_pressed)
    		key.unhandled_presses++;
    }
}

bool Keys_t::AnyKeyWasJustPressed() {
    for(unsigned int i = 0; i < countof(keys); i++) {
        if(keys[i]->unhandled_presses != 0) return true;
    }
    return false;
}
