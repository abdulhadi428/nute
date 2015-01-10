/*
 * Sequences.h
 *
 *  Created on: 09 џэт. 2015 у.
 *      Author: Kreyl
 */

#ifndef SEQUENCES_H_
#define SEQUENCES_H_

#include "ChunkTypes.h"

#if 1 // ============================ LED blink ================================
// Timings
#define T_SHORT_BLINK_MS    45

const LedChunk_t lsqBlinkGreenX2[] = {
        {csSetColor, 0, clGreen},
        {csWait, 180},
        {csSetColor, 0, clBlack},
        {csWait, 180},
        {csSetColor, 0, clGreen},
        {csWait, 180},
        {csSetColor, 0, clBlack},
        {csEnd}
};

#endif

#endif /* SEQUENCES_H_ */
