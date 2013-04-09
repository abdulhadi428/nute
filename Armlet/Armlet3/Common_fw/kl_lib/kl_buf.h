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
template <typename T>
class CircBuf_t {
protected:
    uint32_t IBufSize, IFullSlotsCount;
    T *IPBuf, *PRead, *PWrite;
public:
    uint8_t Get(T *p) {
        if(IFullSlotsCount == 0) return FAILURE;
        memcpy(p, PRead, sizeof(T));
        if(++PRead > (IPBuf + IBufSize - 1)) PRead = IPBuf;     // Circulate buffer
        IFullSlotsCount--;
        return OK;
    }

    uint8_t Put(T *p) {
        if(IFullSlotsCount == IBufSize) return FAILURE;
        chSysLock();
        memcpy(PWrite, p, sizeof(T));
        if(++PWrite > (IPBuf + IBufSize - 1)) PWrite = IPBuf;   // Circulate buffer
        IFullSlotsCount++;
        chSysUnlock();
        return OK;
    }

    uint32_t GetFullSlotsCount() { return IFullSlotsCount; }
    void Reset() { PRead = IPBuf; PWrite = IPBuf; IFullSlotsCount = 0; }
    void Init(T *PBuf, uint32_t Sz) { IPBuf = PBuf; IBufSize = Sz; Reset(); }
};

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


#endif /* KL_BUF_H_ */
