/*
 * File:   main.cpp
 * Author: Kreyl
 * Project: Armlet2South
 *
 * Created on Feb 05, 2013, 20:27
 */

#include "kl_lib_f0.h"

int main(void) {
    PinSetupOut(GPIOC, 8, omPushPull, pudNone, ps10MHz);



    while(TRUE) {
        PinSet(GPIOC, 9);
        DelayLoop(11);
        PinClear(GPIOC, 9);
        DelayLoop(11);
    }
}
