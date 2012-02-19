/*
 * IDStore.cpp
 *
 *  Created on: 25.11.2011
 *      Author: Kreyl
 */

#include "IDStore.h"
#include "kl_util.h"
#include "stm32f10x.h"
#include "stm32f10x_flash.h"

//#define FLASH_DO_NOT_SAVE   // Use this to save Flash when debugging


IDStore_t IDStore;

// Address of saved data

ID_Array_t *SavedIDs = (ID_Array_t*)&SaveAddr;


// ============================== Implementation ===============================

void IDStore_t::EraseAll() {
    IDArr.Count = 0;
}

// ========================== ID operations ====================================
void IDStore_t::Add(uint32_t AID) {
    IDArr.ID[IDArr.Count] = AID;
    IDArr.Count++;
    //dbgPrintIDs();
}

// =============================== Load/save ===================================
void IDStore_t::Load(void) {
    //klPrintf("Loading IDs...\r");
    for(uint32_t i=0; i<4; i++) IDArr.ID[i] = SavedIDs->ID[i];
}
void IDStore_t::Save(void) {
#ifndef FLASH_DO_NOT_SAVE
    FLASH_Status FLASHStatus;
    uint32_t FAddr = (uint32_t)&SaveAddr[0];
    klPrintf("Saving IDs...\r");
    FLASH_Unlock();


    klPrintf("\r addr to write: %X \r",&SaveAddr[0]);

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

    // Write flash
    uint32_t *PRAM = (uint32_t*)&IDArr;    // What to write
    uint32_t DataWordCount = 4; //(IDArr.Count * 2) + 2; // 64-bit ID fills two 32-bit words; and two 32-bit values are added for Count and EraseCounter
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
#endif
}
