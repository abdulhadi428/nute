/*
 * beep.h
 *
 *  Created on: 07.02.2013
 *      Author: kreyl
 */

#ifndef BEEP_H_
#define BEEP_H_

#include "SouthbridgeTxRx.h"
#include "kl_lib_f0.h"

struct BeepChunk_t {
    int8_t VolumePercent;   // 0 means silence, 1...100 means volume, -1 means end
    uint16_t Time_ms;
    uint16_t Freq_Hz;
} PACKED;
#define BEEP_CHUNK_SZ   sizeof(BeepChunk_t)

#define BEEP_MAX_CHUNK_COUNT    9

class Beep_t : public Feeder_t {
private:
    Thread *PThread;
    bool ResetOccured;
    int32_t ChunkCnt;
    BeepChunk_t Buf[BEEP_MAX_CHUNK_COUNT], *PChunk;
    void On(uint8_t Volume)  { TIM1->CCR4 = Volume; TIM1->CCER = TIM_CCER_CC4E; }
    void Off() { TIM1->CCER = 0; }
    void SetFreqHz(uint32_t AFreq);
    void Reset() {
        ResetOccured = true;
        ChunkCnt = 0;
        PChunk = 0;
        FdrByteCnt = 0;
        PFeedData = (uint8_t*)Buf;
        Off();
    }
public:
    // Feeder
    FeederRetVal_t FeedStart(uint8_t Byte);
    FeederRetVal_t FeedData(uint8_t Byte);
    void FeederEndPkt();
    // General
    void Init();
    void Squeak();
};

extern Beep_t Beep;

#endif /* BEEP_H_ */
