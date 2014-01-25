/*
 * buzzer.cpp
 *
 *  Created on: 25 џэт. 2014 у.
 *      Author: Kreyl
 */

#include "buzzer.h"

Buzz_t Buzzer;

void BuzzTmrCallback(void *p) { Buzzer.ITmrCallback(); }

void Buzz_t::ITmrCallback() {
    if     (IPeriod > 2700) IPeriod -= 18;
    else if(IPeriod > 1800) IPeriod -= 7;
    else if(IPeriod > 1350) IPeriod -= 4;
    else if(IPeriod > 900 ) IPeriod -= 3;
    else if(IPeriod > 603 ) IPeriod -= 2;
    else                    IPeriod -= 1;
    IPin.SetTop(IPeriod);
    if(IPeriod >= PERIOD_MIN) {
        chVTSetI(&ITmr, MS2ST(BUZZ_DELAY), (vtfunc_t)&BuzzTmrCallback, nullptr);
    }
}

void Buzz_t::BuzzUp() {
    IPeriod = PERIOD_MAX;
    IPin.SetTop(IPeriod);
    IPin.Set(VOLUME);
    chVTSetI(&ITmr, MS2ST(BUZZ_DELAY), (vtfunc_t)&BuzzTmrCallback, nullptr);
}

