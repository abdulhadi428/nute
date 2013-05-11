/*
 * kl_buf.h
 *
 *  Created on: 07.04.2013
 *      Author: kreyl
 */

#ifndef KL_BUF_H_
#define KL_BUF_H_

#include "ch.h"
#include "string.h" // for memcpy
#include "kl_lib_f2xx.h"

// =========================== Circular buffer =================================
template <typename T, uint32_t Sz>
class CircBuf_t {
protected:
    uint32_t IFullSlotsCount;
    T IBuf[Sz], *PRead, *PWrite;
public:
    uint8_t Get(T *p) {
        if(IFullSlotsCount == 0) return FAILURE;
        memcpy(p, PRead, sizeof(T));
        if(++PRead > (IBuf + Sz - 1)) PRead = IBuf;     // Circulate buffer
        IFullSlotsCount--;
        return OK;
    }

    uint8_t Put(T *p) {
        if(IFullSlotsCount >= Sz) return FAILURE;
        memcpy(PWrite, p, sizeof(T));
        if(++PWrite > (IBuf + Sz - 1)) PWrite = IBuf;   // Circulate buffer
        IFullSlotsCount++;
        return OK;
    }
    inline uint32_t GetEmptyCount() { return Sz-IFullSlotsCount; }
    inline uint32_t GetFullCount()  { return IFullSlotsCount; }
    void Flush(uint32_t ALength) {
        TRIM_VALUE(ALength, IFullSlotsCount);
        IFullSlotsCount -= ALength;
        uint32_t PartSz = (IBuf + Sz) - PRead;
        if(ALength >= PartSz) {
            ALength -= PartSz;
            PRead = IBuf + ALength;
        }
        else PRead += ALength;
    }
    void Init() { PRead = IBuf; PWrite = IBuf; IFullSlotsCount = 0; }
    // Friendship
    //friend class BufChunkPut_t;
};

/*
template <typename T>
class CircBufSemaphored_t : public CircBuf_t<T> {
private:
    BinarySemaphore SemAddItem;
public:
    uint8_t PutWithTimeout(T *p, uint32_t Timeout_ms) {
        if(Timeout_ms != TIME_INFINITE) Timeout_ms = MS2ST(Timeout_ms);
        if(chBSemWaitTimeout(&SemAddItem, Timeout_ms) != RDY_OK) return FAILURE;
        this->Put(p);
        // Check if buf is not full
        if(this->IFullSlotsCount != this->IBufSize) chBSemSignal(&SemAddItem);
        return OK;
    }

    uint8_t Get(T *p) {
        if(this->IFullSlotsCount == 0) return FAILURE;
        memcpy(p, this->PRead, sizeof(T));
        if(++this->PRead > (this->IPBuf + this->IBufSize - 1)) this->PRead = this->IPBuf;     // Circulate buffer
        this->IFullSlotsCount--;
        chBSemSignal(&SemAddItem);
        return OK;
    }

    void Reset() {
        this->PRead = this->IPBuf;
        this->PWrite = this->IPBuf;
        this->IFullSlotsCount = 0;
        chBSemReset(&SemAddItem, NOT_TAKEN);
    }

    void Init(T *PBuf, uint32_t Sz) {
        chBSemInit(&SemAddItem, NOT_TAKEN);
        this->IPBuf = PBuf;
        this->IBufSize = Sz;
        Reset();
    }
};
*/

// Buffer for simple types, like uint8_t etc.
template <typename T, uint32_t Sz>
class CircBufNumber_t : public CircBuf_t<T, Sz> {
public:
    uint8_t Get(T *p, uint32_t ALength) {
        uint8_t Rslt = FAILURE;
        if(this->IFullSlotsCount >= ALength) {  // if enough data
            this->IFullSlotsCount -= ALength;   // 'Length' slots will be freed
            uint32_t PartSz = (this->IBuf + Sz) - this->PRead;  // Data from PRead to right bound
            if(ALength > PartSz) {
                memcpy(p, this->PRead, PartSz);
                this->PRead = this->IBuf;     // Start from beginning
                p += PartSz;
                ALength -= PartSz;
            }
            memcpy(p, this->PRead, ALength);
            this->PRead += ALength;
            if(this->PRead >= (this->IBuf + Sz)) this->PRead = this->IBuf;   // Circulate pointer
            Rslt = OK;
        }
        return Rslt;
    }

    uint8_t Put(T *p, uint32_t Length) {
        uint8_t Rslt = FAILURE;
        if(this->GetEmptyCount() >= Length) {    // check if Buffer overflow
            this->IFullSlotsCount += Length;                      // 'Length' slots will be occupied
            uint32_t PartSz = (this->IBuf + Sz) - this->PWrite;  // Data from PWrite to right bound
            if(Length > PartSz) {
                memcpy(this->PWrite, p, PartSz);
                this->PWrite = this->IBuf;     // Start from beginning
                p += PartSz;
                Length -= PartSz;
            }
            memcpy(this->PWrite, p, Length);
            this->PWrite += Length;
            if(this->PWrite >= (this->IBuf + Sz)) this->PWrite = this->IBuf; // Circulate pointer
            Rslt = OK;
        }
        return Rslt;
    }
};


#endif /* KL_BUF_H_ */
