/*
 * vibro.cpp
 *
 *  Created on: 18.09.2012
 *      Author: kreyl
 */

#include "vibro.h"

Vibro_t Vibro;

void Vibro_t::Task() {
    if(ICounter != 0) {
        if(IsOn) {
            if(Delay.Elapsed(&ITimer, VIBRO_ON_TIME)) {
                Off();
                ICounter--;
            }
        } // if on
        else {
            if(Delay.Elapsed(&ITimer, VIBRO_OFF_TIME)) {
                On();
            }
        } // if off
    } //if != 0
}
