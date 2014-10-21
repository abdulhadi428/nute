/*
 * LedSequence.h
 *
 *  Created on: 21 окт. 2014 г.
 *      Author: g.kruglov
 */

#ifndef LEDSEQUENCE_H_
#define LEDSEQUENCE_H_

#include "color.h"

enum LedChunkSort_t {csSetColor, csWait, csGoto, csEnd};

struct LedChunk_t {
    LedChunkSort_t ChunkSort;
    union {
        uint32_t SmoothVar;       // 0 means immediately. Up to several thounds
        uint32_t Time_ms;
        uint32_t ChunkToJumpTo;
    };
    Color_t Color;
} __attribute__((packed));
#define LED_CHUNK_SZ    sizeof(LedChunk_t)

#define LED_CHUNK_CNT   18



#endif /* LEDSEQUENCE_H_ */
