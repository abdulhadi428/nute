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
#include "ff.h"

#define ID_SZ_BYTES         8   // 7 bytes of Mifare Ultralite's ID
#define ID_MASTER_CNT       9
#define ID_SPECIAL_CNT      4
// Setup this carefully. If EEProm size == 512 bytes,
// Space Left = 512 - 4 - 4 - ID_MASTER_CNT * ID_SZ_BYTES = 432;
// AccessID cnt = 432 / 8 = 54.
#define ID_ACCESS_CNT       99

#if 1 // ========== Files ==========
#define IDSTORE_FILENAME    "ID_Store.ini"

#define ID_GROUP_ACCESS     "AccessID"

#endif

#if 1 // ========================== Auxilary classes ===========================
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
    ID_t& operator = (const ID_t &AID) { ID32[0] = AID.ID32[0]; ID32[1] = AID.ID32[1]; return *this; }
} __attribute__ ((__packed__));

template <uint32_t TCnt>
struct ID_Array_t {
    uint32_t Cnt;
    ID_t ID[TCnt];
    void Erase() { Cnt = 0; }
    bool ContainsID(ID_t &sID, uint32_t *PIndx = nullptr) {
        for(uint32_t i=0; i<Cnt; i++) {
            if(ID[i] == sID) {
                if(PIndx != nullptr) *PIndx = i;
                return true;
            }
        }
        return false;
    }
    uint8_t Add(ID_t &sID) {
        if(ContainsID(sID)) {
            Uart.Printf("\rAlready in base");
            return OK;
        }
        else if(Cnt == TCnt) {
            Uart.Printf("\rBase overflow");
            return FAILURE;
        }
        else {
            ID[Cnt] = sID;
            Cnt++;
            return OK;
        }
    }
    uint8_t Remove(ID_t &sID) {
        uint32_t indx=0;
        if(ContainsID(sID, &indx)) {
            if(indx != (Cnt-1)) {  // Check if not last
                for(uint32_t i=indx; i<Cnt-1; i++) ID[i] = ID[i+1];
            }
            Cnt--;
            Uart.Printf("\rID removed, count = %u\r", Cnt);
            return OK;
        }
        else {
            Uart.Printf("\rNo such ID");
            return FAILURE;
        }
    }
    // Save/Load
    void Save(FIL *PFile, const char *GroupName) {
        f_putc('[', PFile);
        f_puts(ID_GROUP_ACCESS, PFile);
        f_puts("]\r\n", PFile);
        f_printf(PFile, "Count=%u\r\n", Cnt);
        for(uint32_t i=0; i<Cnt; i++) {
            f_printf(PFile, "ID=%04X%04X\r\n", ID[i].ID32[0], ID[i].ID32[1]);
        }
    }
} __attribute__ ((__packed__));

#endif

enum IdKind_t {ikNone, ikAdder, ikRemover, ikAccess, ikSpecial};

class IDStore_t {
private:
    ID_Array_t<ID_ACCESS_CNT> IDAccess;
    ID_Array_t<ID_MASTER_CNT> IDAdder;
    ID_Array_t<ID_MASTER_CNT> IDRemover;
    ID_Array_t<ID_SPECIAL_CNT> IDSpecial;
    FIL IFile;
public:
    bool HasChanged;
    // ID operations
    IdKind_t Check(ID_t &sID, uint32_t *PIndx = nullptr);
    uint8_t AddAccessID(ID_t &sID) {
        if(IDAccess.Add(sID) == OK) {
            HasChanged = true;
            return OK;
        }
        else return FAILURE;
    }
    void RemoveAccess(ID_t &sID) {
        if(IDAccess.Remove(sID) == OK) HasChanged = true;   // Failure means absence in base
    }
    // Load/save
    void Load();
    void Save();
    void EraseAll() {
        IDAccess.Erase();
//        IDArr.CntMaster = 0;
        HasChanged = true;
    }
};

#endif /* IDSTORE_H_ */
