/*
 * battery.cpp
 *
 *  Created on: 27.07.2011
 *      Author: Kreyl
 */

#include "battery.h"
#include "delay_util.h"

Battery_t Battery;

void Battery_t::Init() {
    // Setup charging IC output
    BAT_CHRG_DDR  &= ~(1<<BAT_CHRG_P);
    BAT_CHRG_PORT |=  (1<<BAT_CHRG_P);
    IsCharging = false;
    ADCState = ADCNoMeasure;
}

void Battery_t::Task() {
    // ====== Check if charging ======
    if (Delay.Elapsed(&ChargeTimer, BAT_CHARGE_T)) {
        if (IsChargingSignalled() && !IsCharging) {
            IsCharging = true;
            EVENT_ChargeStarted();
        }
        else if (!IsChargingSignalled() && IsCharging) {
            IsCharging = false;
            EVENT_ChargeEnded();
        }
    }

    // ====== ADC ======
    switch (ADCState) {
        case ADCNoMeasure:
            if (Delay.Elapsed(&ADCTimer, ADC_MEASURE_T)) { // Check if timeout has passed
                ADC_REF_ENABLE();           // Start ADC
                ADC_START_MEASUREMENT();    // Start new measure - dummy one for the first time
                ADCState = ADCInit;
            }
            break;

        case ADCInit:
            // Check if dummy measurement completed & timeout passed
            if (ADC_COMPLETED() && Delay.Elapsed (&ADCTimer, ADC_PREPARE_T)) {
                ADC_START_MEASUREMENT();    // Start new measure
                ADCValue = 0;
                MeasuresCounter = 0;
                ADCState = ADCMeasuring;
            }
            break;

        case ADCMeasuring:
            if (ADC_COMPLETED()) {  // Check if current measurement completed
                MeasuresCounter++;
                ATOMIC_BLOCK(ATOMIC_FORCEON) {
                    ADCValue += ADC;    // Add ADC result
                } // atomic
                // Check if series of measurements is completed
                if (MeasuresCounter >= ADC_NUMBER_OF_MEASURES) {
                    // Disable ADC to save energy
                    ADC_DISABLE();
                    ADC_REF_DISABLE();
                    // Prepare result
                    ATOMIC_BLOCK(ATOMIC_FORCEON) {
                        ADCValue >>= ADC_POWER_OF_MEASURES;
                    }
                    ADCState = ADCNoMeasure;
                    Delay.Reset(&ADCTimer);
                    EVENT_ADCMeasureCompleted();
                }
                // if not, start measurement again
                else ADC_START_MEASUREMENT();
            } // if current measurement completed
            break;
    } // switch
}
