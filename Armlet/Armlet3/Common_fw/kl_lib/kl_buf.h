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
    inline uint32_t GetEmptyCount() { return IBufSize-IFullSlotsCount; }
    inline uint32_t GetFullCount() { return IFullSlotsCount; }
    void Reset() { PRead = IPBuf; PWrite = IPBuf; IFullSlotsCount = 0; }
    void Init(T *PBuf, uint32_t Sz) { IPBuf = PBuf; IBufSize = Sz; Reset(); }
    // Friendship
    friend class RxBuf_t;
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

// Buffer for simple types, like uint8_t etc.
template <typename T>
class CircBufNumber_t : public CircBuf_t<T> {
public:
    void Get(T *p, uint32_t *PLength) {
        if(*PLength > this->IFullSlotsCount) *PLength = this->IFullSlotsCount;
        uint32_t Length = *PLength;                     // How much data to read
        this->IFullSlotsCount -= Length;                // 'Length' slots will be freed
        uint32_t PartSz = (this->IPBuf + this->IBufSize) - this->PRead;  // Data from PRead to right bound
        if(Length > PartSz) {
            memcpy(p, this->PRead, PartSz);
            this->PRead = this->IPBuf;     // Start from beginning
            p += PartSz;
            Length -= PartSz;
        }
        memcpy(p, this->PRead, Length);
        this->PRead += Length;
        if(this->PRead > (this->IPBuf + this->IBufSize - 1)) this->PRead = this->IPBuf;   // Circulate pointer
    }

    uint8_t Put(T *p, uint32_t Length) {
        if(this->GetEmptyCount() < Length) return FAILURE;    // Buffer overflow
        this->IFullSlotsCount += Length;                      // 'Length' slots will be occupied
        uint32_t PartSz = (this->IPBuf + this->IBufSize) - this->PWrite;  // Data from PWrite to right bound
        if(Length > PartSz) {
            memcpy(this->PWrite, p, PartSz);
            this->PWrite = this->IPBuf;     // Start from beginning
            p += PartSz;
            Length -= PartSz;
        }
        memcpy(this->PWrite, p, Length);
        this->PWrite += Length;
        if(this->PWrite > (this->IPBuf + this->IBufSize - 1)) this->PWrite = this->IPBuf; // Circulate pointer
        return OK;
    }
};

// =============================== Rx buf ======================================
struct DataPktPtr_t {
    uint8_t *Ptr;
    uint32_t Length;
};

class RxBuf_t {
private:
    CircBufNumber_t<uint8_t> IData; // Data buffer
    CircBuf_t<DataPktPtr_t> IPtrs;  // Pointers buffer
    DataPktPtr_t IPkt;
public:
    // Put
    uint8_t StartNewPkt() {
        if(PktInProgress) CancelPkt();          // Cancel unfinished pkt
        // Check if empty slots available
        if(!IPtrs.GetEmptyCount()) return FAILURE; // No room for pointer
        if(!IData.GetEmptyCount()) return FAILURE; // No room for data
        PktInProgress = true;
        // Setup current pkt
        IPkt.Length = 0;
        IPkt.Ptr = IData.PWrite;
        return OK;
    }
    uint8_t AddData(uint8_t *p, uint32_t Length) {
        if(!PktInProgress or (IData.GetEmptyCount() < Length)) return FAILURE;
        IData.Put(p, Length);
        IPkt.Length += Length;
        return OK;
    }
    void CompletePkt() {
        PktInProgress = false;
        IPtrs.Put(&IPkt);
    }
    void CancelPkt() {
        PktInProgress = false;
        // Restore data buf
        IData.PWrite = IPkt.Ptr;
        IData.IFullSlotsCount -= IPkt.Length;
    }
    // Get
    uint32_t GetPktCount() { return IPtrs.GetFullCount(); }
    uint8_t GetPkt(DataPktPtr_t *p) {
        DataPktPtr_t TmpPkt;
        uint8_t Rslt = IPtrs.Get(&TmpPkt);
        if(Rslt == OK) {
            p->Length = TmpPkt.Length;
            IData.Get(p->Ptr, &p->Length);
        }
        return Rslt;
    }
    // Common
    bool PktInProgress;
    void Init(DataPktPtr_t *PPtrBuf, uint32_t PtrBufSz, uint8_t *PDataBuf, uint32_t DataBufSz) {
        PktInProgress = false;
        IPtrs.Init(PPtrBuf, PtrBufSz);
        IData.Init(PDataBuf, DataBufSz);
    }
};


#endif /* KL_BUF_H_ */
