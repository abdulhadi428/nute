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

const LedChunk_t lsqIdle[] = {
        {csSetColor, 0, clBlack},
        {csEnd}
};

// Adding / removing IDs
const LedChunk_t lsqAddingAccIdle[] = {
        {csSetColor, 0, clGreen},
        {csEnd}
};
const LedChunk_t lsqAddingAccNew[] = {
        {csSetColor, 0, clBlack},
        {csWait, 180},
        {csSetColor, 0, clGreen},
        {csEnd}
};

const LedChunk_t lsqRemovingAccIdle[] = {
        {csSetColor, 0, clRed},
        {csEnd}
};
const LedChunk_t lsqRemovingAccNew[] = {
        {csSetColor, 0, clBlack},
        {csWait, 180},
        {csSetColor, 0, clRed},
        {csEnd}
};

const LedChunk_t lsqEraseAll[] = {
        {csSetColor, 0, clRed},
        {csWait, 180},
        {csSetColor, 0, clBlack},
        {csWait, 180},
        {csSetColor, 0, clRed},
        {csWait, 180},
        {csSetColor, 0, clBlack},
        {csWait, 180},
        {csSetColor, 0, clRed},
        {csWait, 180},
        {csSetColor, 0, clBlack},
        {csEnd}
};

// General
const LedChunk_t lsqBlinkGreen[] = {
        {csSetColor, 0, clGreen},
        {csWait, 180},
        {csSetColor, 0, clBlack},
        {csEnd}
};

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
