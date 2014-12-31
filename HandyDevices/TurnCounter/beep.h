/*
 * sound.h
 *
 *  Created on: 25.01.2012
 *      Author: kreyl
 */

#ifndef SOUND_H_
#define SOUND_H_

#include "kl_lib_f100.h"

enum ChunkKind_t {ckNormal=0, ckStop=1, ckRepeat=2};

struct BeepChunk_t {
    uint8_t Volume;   // 0 means silence, 10 means top
    uint16_t Freq_Hz;
    uint16_t Time_ms;
    ChunkKind_t ChunkKind;
} PACKED;
#define BEEP_CHUNK_SZ   sizeof(BeepChunk_t)

class Beeper_t {
private:
    VirtualTimer ITmr;
    const BeepChunk_t *IPFirstChunk;
public:
    void BeepI(const BeepChunk_t *PSequence);
    void Beep(const BeepChunk_t *PSequence) {   // Beep with this function
        IPFirstChunk = PSequence;
        chSysLock();
        BeepI(PSequence);
        chSysUnlock();
    }
    void Stop() {
        chSysLock();
        if(chVTIsArmedI(&ITmr)) chVTResetI(&ITmr);
        chSysUnlock();
        IPin.Set(0);
    }
    void Init();
    void Shutdown();
    // Inner use
    PwmPin_t IPin;
};
extern Beeper_t Beeper;



#endif /* SOUND_H_ */
