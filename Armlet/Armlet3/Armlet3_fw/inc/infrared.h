/*
 * infrared.h
 *
 *  Created on: 12.03.2013
 *      Author: kreyl
 */

#ifndef INFRARED_H_
#define INFRARED_H_

#include "kl_lib_f2xx.h"

/*
 *  ModTmr outputs a TRGO at every Update event, i.e., every 600us.
 *  CarTmr as slave receives signal from ModTmr and uses it to Reset.
 *  CarTmr outputs a TRGO at every Reset event - i.e., every 600us (since it is
 *  Reset by ModTmr).
 *  CarTmr uses TRGO as DMA request; i.e. DMA request generated at every 600us.
 *  Buffer contains power level for every 600us chunk of packet;
 *  i.e. "PWM on for 2400us, off for 600us" translates to {Pwr, Pwr, Pwr, Pwr, 0}
 *  in Buf. (Pwr is power level).
 */

#define IR_CARRIER_HZ       56000
#define IR_BIT_CNT          14

#define IR_CARRIER_TMR      TIM3
#define IR_CARRIER_CHNL     2
#define IR_CARRIER_GPIO     GPIOC
#define IR_CARRIER_PIN      7

#define IR_CARRIER_TRG_IN   tiITR2  // Slave using TIM5 as input (ITR2)

#define IR_MODULATION_TMR   TIM5

#define IR_TX_DMA_STREAM    STM32_DMA1_STREAM4  // }
#define IR_TX_DMA_CHNL      5                   // } TIM3 trig


// Delays, uS
#define IR_TICK_US      600 // Protocol smallest time unit, us
/* Header = 4 * IR_TICK_US
 * Space  = 1 * IR_TICK_US
 * Zero   = 1 * IR_TICK_US
 * One    = 2 * IR_TICK_US
 */

class Infrared_t {
private:
    uint16_t TxPwrBuf[4+1 + IR_BIT_CNT*(2+1) + 1];    // Buffer of power values: header + all one's + 1 delay after
    uint32_t MaxPower;
    Timer_t Carrier, Modulator;
public:
    Infrared_t():
        MaxPower(0),
        Carrier(IR_CARRIER_TMR), Modulator(IR_MODULATION_TMR),
        IsBusy(false) { }
    bool IsBusy;
    void Init();
    uint8_t TransmitWord(uint16_t wData, uint8_t PwrPercent);
    // Inner use
    void IStopModulator() { Modulator.Disable(); }
};

extern Infrared_t IR;

#endif /* INFRARED_H_ */
