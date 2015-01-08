/*
 * ChunkTypes.h
 *
 *  Created on: 08 џэт. 2015 у.
 *      Author: Kreyl
 */

#ifndef KL_LIB_CHUNKTYPES_H_
#define KL_LIB_CHUNKTYPES_H_

#if 1 // ============================= RGB LED chenks ==========================
enum LedChunkSort_t {csSetColor, csWait, csGoto, csEnd};

struct LedChunk_t {
    LedChunkSort_t ChunkSort;
    union {
        uint32_t MorphMS;       // 0 means immediately. Up to several thounds
        uint32_t Time_ms;
        uint32_t ChunkToJumpTo;
    };
    Color_t Color;
} __attribute__((packed));
#define LED_CHUNK_SZ    sizeof(LedChunk_t)  // 8 bytes

#endif


#endif /* KL_LIB_CHUNKTYPES_H_ */
