/*
 * radio.cpp
 *
 *  Created on: Feb 15, 2013
 *      Author: g.kruglov
 */

#include "radio.h"

Radio_t Radio;
static RadioState_t IState;

static WORKING_AREA(waRadioThread, 128);
static msg_t RadioThread(void *arg) {
    (void)arg;
    chRegSetThreadName("Radio");
    while(1) {
        switch(IState) {
            case rIdle:
                break;

            case rService:
                break;
        }
    } // while 1
    return 0;
}



void Radio_t::Init() {
    // Init variables
    IState = rIdle;
    ClearContacts();

    // ==== Create and start thread ====
    chThdCreateStatic(waRadioThread, sizeof(waRadioThread), NORMALPRIO, RadioThread, NULL);
}


