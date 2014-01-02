/*
 * sequences.h
 *
 *  Created on: Nov 9, 2013
 *      Author: kreyl
 */

#ifndef SEQUENCES_H_
#define SEQUENCES_H_

#include "peripheral.h"

/*
 * ckNormal => after this, goto next chunk
 * ckStop   => after this, stop and off
 * ckRepeat => after this, goto begin
 */

#if 1 // ============================= Beep ====================================
/* Every sequence is an array of BeepCmd_t:
 struct BeepChunk_t {
    uint8_t Volume;   // 0 means silence, 10 means top
    uint16_t Freq_Hz;
    uint16_t Time_ms;
    ChunkKind_t ChunkKind;
  };
*/
#define BEEP_VOLUME     1   // set to 10 in production, and to 1 when someone sleeps near
const BeepChunk_t BeepKeyOk[] = {
        {BEEP_VOLUME, 1975, 54, ckStop},
};
const BeepChunk_t BeepKeyErr[] = {
        {BEEP_VOLUME, 1000, 54, ckStop},
};

const BeepChunk_t BeepStart[] = {   // Re F# La
        {BEEP_VOLUME, 1175, 180, ckNormal},
        {BEEP_VOLUME, 1480, 180, ckNormal},
        {BEEP_VOLUME, 1720, 180, ckNormal},
        {BEEP_VOLUME, 2093, 180, ckStop},
};
const BeepChunk_t BeepStop[] = {
        {BEEP_VOLUME, 2093, 180, ckNormal},
        {BEEP_VOLUME, 1720, 180, ckNormal},
        {BEEP_VOLUME, 1480, 180, ckNormal},
        {BEEP_VOLUME, 1175, 180, ckStop},
};

const BeepChunk_t BeepBeep[] = {
        {BEEP_VOLUME, 1975, 54, ckNormal},
        {0, 0, 54, ckNormal},
        {BEEP_VOLUME, 1975, 54, ckStop},
};

const BeepChunk_t BeepWake[] = {
        {BEEP_VOLUME, 1975, 180, ckNormal},
        {BEEP_VOLUME, 2489, 180, ckNormal},
        {BEEP_VOLUME, 2960, 180, ckStop},
};

const BeepChunk_t BeepStandBy[] = {
        {BEEP_VOLUME, 2960, 180, ckNormal},
        {BEEP_VOLUME, 2489, 180, ckNormal},
        {BEEP_VOLUME, 1975, 180, ckStop},
};

#endif

#endif /* SEQUENCES_H_ */
