/*
 * power.cpp
 *
 *  Created on: 09.04.2013
 *      Author: kreyl
 */

#include "power.h"

Pwr_t Power;

static WORKING_AREA(waPwrThread, 128);
static msg_t PwrThread(void *arg) {
    (void)arg;
    chRegSetThreadName("Pwr");

    bool WasExternal = false;
    while(1) {
        chThdSleepMilliseconds(PWR_MEASURE_EVERY_MS);
        // Check if power src changed
        if(WasExternal and !Power.ExternalPwrOn()) {
            WasExternal = false;
            chEvtBroadcast(&Power.EvtSrcPwrChange);
        }
        else if(!WasExternal and Power.ExternalPwrOn()) {
            WasExternal = true;
            chEvtBroadcast(&Power.EvtSrcPwrChange);
        }

        // ADC
    }
    return 0;
}

void Pwr_t::Init() {
    // GPIO
    PinSetupIn(PWR_EXTERNAL_GPIO, PWR_EXTERNAL_PIN, pudPullDown);
    PinSetupIn(PWR_CHARGING_GPIO, PWR_CHARGING_PIN, pudPullUp);
    PinSetupAnalog(PWR_BATTERY_GPIO,  PWR_BATTERY_PIN);

    // Event
    chEvtInit(&EvtSrcPwrChange);

    // ADC
    // Create and start thread
    chThdCreateStatic(waPwrThread, sizeof(waPwrThread), NORMALPRIO, PwrThread, NULL);
}
