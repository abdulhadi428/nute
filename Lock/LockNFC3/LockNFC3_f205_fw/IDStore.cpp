/*
 * IDStore.cpp
 *
 *  Created on: 25.11.2011
 *      Author: Kreyl
 */

#include "IDStore.h"
#include "main.h"
//#include "kl_util.h"
//#include "stm32f10x.h"
//#include "stm32f10x_flash.h"

//#define DO_NOT_SAVE   // Use this to save Flash when debugging

//void IDStore_t::PrintIDs(void) {
//    klPrintf(" Count = %u\r", IDArr.Count);
//    for (uint32_t i=0; i<IDArr.Count; i++)
//        klPrintf(" %u %X%X\r", i, (uint32_t)((IDArr.ID[i] >> 32)& 0xFFFFFFFF), (uint32_t)(IDArr.ID[i] & 0xFFFFFFFF));
//}

//bool IDStore_t::IsPresentIndx(uint64_t AID, uint32_t *AIndx) {
//    for (uint32_t i=0; i<IDArr.Count; i++) {
//        if (IDArr.ID[i] == AID) {
//            if (AIndx != 0) *AIndx = i;
//            klPrintf("Key indx: %u\r", i);
//            return true;
//        }
//    } // for
//    klPrintf("No such key\r");
//    return false;
//}

IdKind_t IDStore_t::Check(ID_t &sID, uint32_t *PIndx) {
    // Iterate access ids
    for(uint32_t i=0; i<IDArr.CntAccess; i++) {
        if(IDArr.IDAcc[i] == sID) {
            if(PIndx != nullptr) *PIndx = i;
            Uart.Printf("\rID Acc indx: %u", i);
            return ikAccess;
        }
    }
    // Iterate master ids
    for(uint32_t i=0; i<IDArr.CntMaster; i++) {
        if(IDArr.IDMaster[i] == sID) {
            if(PIndx != nullptr) *PIndx = i;
            Uart.Printf("\rID Master indx: %u", i);
            return ikMaster;
        }
    }
    return ikNone;
}

void IDStore_t::AddAcc(ID_t &sID) {
    IdKind_t IdKind = Check(sID);
    if(IdKind == ikAccess) {
        Uart.Printf("\rKeyAcc is already in base");
        return;         // Already in base
    }
    // TODO: remove from masters if there
    if(IDArr.CntAccess == ID_ACCESS_CNT) {  // Base overflow, display error
        LedService.StartSequence(lsqError);
        Uart.Printf("\rBase overflow");
        return;
    }
    IDArr.IDAcc[IDArr.CntAccess] = sID;
    IDArr.CntAccess++;
    HasChanged = true;
    Uart.Printf("\rKeyAcc added, count = %u", IDArr.CntAccess);
}

void IDStore_t::RemoveAcc(ID_t &sID) {
    uint32_t indx=0;
    IdKind_t IdKind = Check(sID, &indx);
    if(IdKind == ikAccess) {
        if(indx != (IDArr.CntAccess-1)) {  // Check if not last
            for(uint32_t i=indx; i<IDArr.CntAccess-1; i++) IDArr.IDAcc[i] = IDArr.IDAcc[i+1];
        }
        IDArr.CntAccess--;
        HasChanged = true;
        Uart.Printf("\rKeyAcc removed, count = %u\r", IDArr.CntAccess);
        //dbgPrintIDs();
    }
    else Uart.Printf("\rNo such KeyAcc in base");
}

 /*
// =============================== Load/save ===================================
void IDStore_t::Load(void) {
    IsChanged = false;
    IDArr.FlashEraseCounter = SavedIDs->FlashEraseCounter;
    IDArr.Count = SavedIDs->Count;
    //klPrintf("Loading IDs...\r");
    klPrintf("Flash Erase counter = %u\r", SavedIDs->FlashEraseCounter);
    if (IDArr.Count == 0) {   // nothing interesting there
        klPrintf("Flash is empty\r");
    }
    else {
        for(uint32_t i=0; i<IDArr.Count; i++) IDArr.ID[i] = SavedIDs->ID[i];
        PrintIDs();
        klPrintf("Loading completed\r\r");
    }
}
void IDStore_t::Save(void) {
#ifndef FLASH_DO_NOT_SAVE
    FLASH_Status FLASHStatus;
    uint32_t FAddr = (uint32_t)&SaveAddr;
    klPrintf("Saving IDs...\r");
    FLASH_Unlock();

    // Erase flash
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);   // Clear All pending flags
    for (uint32_t i=0; i<PAGE_COUNT; i++) {
        FLASHStatus = FLASH_ErasePage(FAddr + (FLASH_PAGE_SIZE * i));
        //klPrintf("  Flash erase %u: %u\r", i, FLASHStatus);
        if (FLASHStatus != FLASH_COMPLETE) {
            klPrintf("  Flash erase error");
            return;
        }
    }

    IDArr.FlashEraseCounter++;  // increase flash erase counter
    // Write flash
    uint32_t *PRAM = (uint32_t*)&IDArr;    // What to write
    uint32_t DataWordCount = (IDArr.Count * 2) + 2; // 64-bit ID fills two 32-bit words; and two 32-bit values are added for Count and EraseCounter
    //klPrintf("  Flash write %u words: ", DataWordCount);
    for(uint32_t i=0; i<DataWordCount; i++) {
        FLASHStatus = FLASH_ProgramWord(FAddr, *PRAM);
        //klPrintf("%X", *PRAM);
        if (FLASHStatus != FLASH_COMPLETE) {
            klPrintf("  Flash write error");
            return;
        }
        //else klPrintf("#");
        FAddr += 4;
        PRAM++;
    }
    FLASH_Lock();

    // Check the correctness of written data
    uint32_t *PFLASH = (uint32_t*)&SaveAddr;
    PRAM = (uint32_t*)&IDArr;
    //klPrintf("\r PFLASH=%X\r", (uint32_t)PFLASH);
    for(uint32_t i=0; i<DataWordCount; i++) {
        //klPrintf("PRAM: %X; PFLASH: %X\r", *PRAM, *PFLASH);
        if ((*PFLASH) != (*PRAM)) {
            klPrintf("  Data check failed");
            //return;
        }
        PFLASH++;
        PRAM++;
    } // for

    klPrintf("\r  Write completed\r\r");
    IsChanged = false;
#endif
}
*/
