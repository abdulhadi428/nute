/*
 * sound.h
 *
 *  Created on: 25.01.2012
 *      Author: kreyl
 */

#ifndef SOUND_H_
#define SOUND_H_

#include "delay_util.h"

#define BEEP_MAX_CHUNKS 4

typedef struct {
    uint16_t Length;
    uint8_t Volume;
} BeepChunk_t;

struct BeepSnd_t {
    uint8_t ChunkCount;
    BeepChunk_t Chunks[BEEP_MAX_CHUNKS];
};

class Beep_t {
private:
    uint32_t ITimer;
    uint32_t ICounter;
    void On(void);
    void Off(void);
    BeepSnd_t *ISnd;
public:
    void SetSound(BeepSnd_t *ASnd);
    void Init(void);
    void Task(void);
};

extern Beep_t Beep;
// Sounds
extern BeepSnd_t NothingBeep, AlienBeep;


#endif /* SOUND_H_ */
