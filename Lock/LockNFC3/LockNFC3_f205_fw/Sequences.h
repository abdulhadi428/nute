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
const LedChunk_t lsqIdle[] = {
        {csSetup, 0, clBlack},
        {csEnd}
};

const LedChunk_t lsqError[] = {
        {csSetup, 0, clRed},
        {csWait, 4005},
        {csSetup, 0, clBlack},
        {csEnd}
};

// Adding / removing IDs
const LedChunk_t lsqAddingAccIdle[] = {
        {csSetup, 0, clGreen},
        {csEnd}
};
const LedChunk_t lsqAddingAccNew[] = {
        {csSetup, 0, clBlack},
        {csWait, 180},
        {csSetup, 0, clGreen},
        {csEnd}
};

const LedChunk_t lsqRemovingAccIdle[] = {
        {csSetup, 0, clRed},
        {csEnd}
};
const LedChunk_t lsqRemovingAccNew[] = {
        {csSetup, 0, clBlack},
        {csWait, 180},
        {csSetup, 0, clRed},
        {csEnd}
};

const LedChunk_t lsqEraseAll[] = {
        {csSetup, 0, clRed},
        {csWait, 180},
        {csSetup, 0, clBlack},
        {csWait, 180},
        {csSetup, 0, clRed},
        {csWait, 180},
        {csSetup, 0, clBlack},
        {csWait, 180},
        {csSetup, 0, clRed},
        {csWait, 180},
        {csSetup, 0, clBlack},
        {csWait, 180},
        {csSetup, 0, clRed},
        {csWait, 180},
        {csSetup, 0, clBlack},
        {csEnd}
};

// General
const LedChunk_t lsqBlinkGreen[] = {
        {csSetup, 0, clGreen},
        {csWait, 180},
        {csSetup, 0, clBlack},
        {csEnd}
};

const LedChunk_t lsqBlinkGreenX2[] = {
        {csSetup, 0, clGreen},
        {csWait, 180},
        {csSetup, 0, clBlack},
        {csWait, 180},
        {csSetup, 0, clGreen},
        {csWait, 180},
        {csSetup, 0, clBlack},
        {csWait, 999},
        {csGoto, 0}
//        {csEnd}
};
#endif

#if 1 // ============================ LED RGB ==================================
const LedChunk_t lsqSmootnGreen[] = {
        {csSetup, 1800, clGreen},
        {csWait, 999},
        {csSetup, 1800, clBlack},
        {csWait, 999},
        {csGoto, 0}
};


#endif

#endif /* SEQUENCES_H_ */
