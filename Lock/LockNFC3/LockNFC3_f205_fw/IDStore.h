/*
 * IDStore.h
 *
 *  Created on: 25.11.2011
 *      Author: Kreyl
 *
 *      Beware: number of stored IDs and Save function are made for data to fit
 *      to (2 pages = 2048 bytes), and for page size of 1024 bytes. Change this if needed.
 *      Payload data size is (252 * 8 bytes = 2016 bytes) + (4 bytes of count) + (4 bytes of EraseCounter) = 2024 bytes
 */

#ifndef IDSTORE_H_
#define IDSTORE_H_

#include <inttypes.h>
#include "cmd_uart.h"
#include "ch.h"

#define ID_SZ_BYTES         8
#define ID_MASTER_CNT       9
// Setup this carefully. If EEProm size == 512 bytes,
// Space Left = 512 - 4 - 4 - ID_MASTER_CNT * ID_SZ_BYTES = 432;
// AccessID cnt = 432 / 8 = 54.
#define ID_ACCESS_CNT       54

struct ID_t {
    union {
        uint32_t ID32[2];
        uint8_t ID8[8];
    } __attribute__ ((__packed__));
    void ConstructOfBuf(uint8_t *p) {
        chSysLock();
        memcpy(ID8, p, 8);
        chSysUnlock();
    }
    void Print() { Uart.Printf("\r%04X %04X", ID32[0], ID32[1]); }
    bool operator == (const ID_t &AID) { return (ID32[0] == AID.ID32[0]) and (ID32[1] == AID.ID32[1]); }
} __attribute__ ((__packed__));

struct ID_Array_t {
    uint32_t CntMaster, CntAccess;
    ID_t IDAcc[ID_ACCESS_CNT], IDMaster[ID_MASTER_CNT];
} __attribute__ ((__packed__));

enum IdKind_t {ikNone, ikMaster, ikAccess};

class IDStore_t {
private:
    ID_Array_t IDArr;
//    bool IsPresentIndx(uint64_t AID, uint32_t *AIndx);
//    void PrintIDs(void);
public:
    void Init() {}
    bool HasChanged;
    // ID operations
    IdKind_t Check(ID_t &sID) { return ikNone; }
//    bool IsPresent(ID_t *PID) { return IsPresentIndx(AID, 0); }
    void AddAcc(ID_t &sID) {}
    void RemoveAcc(ID_t &sID) {}
    // Load/save
//    void Load(void);
    void Save() {}
    void EraseAll() {}
};

#endif /* IDSTORE_H_ */
