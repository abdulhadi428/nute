/*
 * keys.cpp
 *
 *  Created on: 08.02.2012
 *      Author: kreyl
 */

#include "keys.h"
#include "kl_lib.h"

Keys_t Keys;

void Keys_t::Task() {
    if (!Delay.Elapsed(&ITimer, KEYS_RATE)) return;

    uint8_t KeyPressedNow = KEY_NONE;
    // Read current input state
    if      (klGpio::IsClearByN(GPIOB, 4))  KeyPressedNow = 1;
    else if (klGpio::IsClearByN(GPIOB, 5))  KeyPressedNow = 2;
    else if (klGpio::IsClearByN(GPIOB, 11)) KeyPressedNow = 3;

    // Trigger event if key changed
    if (KeyPressedNow != PressedKey) {
        PressedKey = KeyPressedNow;
        if ((KeyPressedNow != KEY_NONE)  && (EvtKbd != 0)) EvtKbd(KeyPressedNow);
    }
}

void Keys_t::Init() {
    // Inputs
    klGpio::SetupByMsk(GPIOB, (GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_11), GPIO_Mode_IPU);
    // Output
    klGpio::SetupByN(GPIOB, 3, GPIO_Mode_Out_PP);
    // Reset pressed key
    PressedKey = KEY_NONE;
}
