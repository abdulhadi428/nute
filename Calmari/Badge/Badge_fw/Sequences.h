/*
 * Sequences.h
 *
 *  Created on: 09 џэт. 2015 у.
 *      Author: Kreyl
 */

#ifndef SEQUENCES_H_
#define SEQUENCES_H_

#include "ChunkTypes.h"

#if 1 // ============================ LED RGB ==================================
#define LSQ_CHANGING_DELAY  999
#define LSQ_WAIT_DELAY      999
const LedChunk_t lsqChange[] = {
        {csSetup, LSQ_CHANGING_DELAY, clRed},
        {csWait,  LSQ_WAIT_DELAY},
        {csSetup, LSQ_CHANGING_DELAY, clYellow},
        {csWait,  LSQ_WAIT_DELAY},
        {csSetup, LSQ_CHANGING_DELAY, clGreen},
        {csWait,  LSQ_WAIT_DELAY},
        {csSetup, LSQ_CHANGING_DELAY, clCyan},
        {csWait,  LSQ_WAIT_DELAY},
        {csSetup, LSQ_CHANGING_DELAY, clBlue},
        {csWait,  LSQ_WAIT_DELAY},
        {csSetup, LSQ_CHANGING_DELAY, clMagenta},
        {csWait,  LSQ_WAIT_DELAY},
        {csSetup, LSQ_CHANGING_DELAY, clWhite},
        {csWait,  LSQ_WAIT_DELAY},
        {csGoto, 0}
};


const LedChunk_t lsqSmootnGreen[] = {
        {csSetup, 1800, clGreen},
        {csWait, 999},
        {csSetup, 1800, clBlack},
        {csWait, 999},
        {csGoto, 0}
};


#endif

#endif /* SEQUENCES_H_ */
