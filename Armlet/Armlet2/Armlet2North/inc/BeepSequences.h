/*
 * BeepSequences.h
 *
 *  Created on: 03.02.2013
 *      Author: kreyl
 */

#ifndef BEEPSEQUENCES_H_
#define BEEPSEQUENCES_H_

/*
 * This file contains sequences of beeps.
 * Every sequence is an array of BeepCmd_t:
 struct BeepCmd_t {
    uint8_t VolumePercent;
    uint32_t Time_ms;
    uint16_t Freq_Hz;
};

 * Do not exceed BEEP_MAX_CHUNK_COUNT! (9)
 */

#include "peripheral.h"

const BeepSequence_t ShortBeep = {
        1,
        {
                {50, 180, 2000},
        }
};

const BeepSequence_t BeepBeep = {
        3,
        {
                {50, 180, 2000},
                {BEEP_SILENCE, 180},
                {0xEE, 180, 2000},
        }
};


#endif /* BEEPSEQUENCES_H_ */
