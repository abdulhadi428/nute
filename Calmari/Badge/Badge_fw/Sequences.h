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
const LedChunk_t lsqChange[] = {
        {csSetup, 900, clRed},
        {csSetup, 900, clYellow},
        {csSetup, 900, clGreen},
        {csSetup, 900, clCyan},
        {csSetup, 900, clBlue},
        {csSetup, 900, clMagenta},
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
