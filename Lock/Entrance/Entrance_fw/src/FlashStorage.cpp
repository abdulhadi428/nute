/*
 * FlashStorage.cpp
 *
 *  Created on: 12.09.2012
 *      Author: kreyl
 */

#include "FlashStorage.h"
#include "stm32f10x_flash.h"
#include "kl_lib.h"

// Address of saved data
const uint32_t SaveArr[SAVED_DATA_SIZE] __attribute__ ((aligned(SAVED_DATA_SIZE))) = {0xFFFFFFFF};


void FlashStorage_t::Load(uint32_t *PData, uint32_t ALength) {
    for(uint32_t i=0; i<ALength; i++) PData[i] = SaveArr[i];
}

//#define VERBOSE
void FlashStorage_t::Save(uint32_t *PData, uint32_t ALength) {
    FLASH_Status FLASHStatus;
    uint32_t FAddr = (uint32_t)&SaveArr[0];
    FLASH_Unlock();

    // Erase flash
#ifdef VERBOSE
    Uart.Printf("Erasing flash...\r");
    Uart.FlushTx();
#endif
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);   // Clear All pending flags
    for (uint32_t i=0; i<PAGE_COUNT; i++) {
        FLASHStatus = FLASH_ErasePage(FAddr + (FLASH_PAGE_SIZE * i));
        //klPrintf("  Flash erase %u: %u\r", i, FLASHStatus);
        if (FLASHStatus != FLASH_COMPLETE) {
            Uart.Printf("  Flash erase error\r");
            return;
        }
    }

    // Write flash
    uint32_t *P = PData;
#ifdef VERBOSE
    Uart.Printf("Writing flash...\r");
    Uart.FlushTx();
#endif
    //klPrintf("  Flash write %u words: ", DataWordCount);
    for(uint32_t i=0; i<ALength; i++) {
        FLASHStatus = FLASH_ProgramWord(FAddr, *P);
        //klPrintf("%X", *PRAM);
        if (FLASHStatus != FLASH_COMPLETE) {
            Uart.Printf("  Flash write error\r");
            return;
        }
        //else klPrintf("#");
        FAddr += 4;
        P++;
    }
    FLASH_Lock();

    // Check the correctness of written data
    P = PData;
#ifdef VERBOSE
    Uart.Printf("Checking flash...\r");
    Uart.FlushTx();
#endif
    //klPrintf("\r PFLASH=%X\r", (uint32_t)PFLASH);
    for(uint32_t i=0; i<ALength; i++) {
        //klPrintf("PRAM: %X; PFLASH: %X\r", *PRAM, *PFLASH);
        if (SaveArr[i] != *P) {
            Uart.Printf("  Data check failed\r");
            //return;
        }
        P++;
    } // for

#ifdef VERBOSE
    Uart.Printf("  Write completed\r");
#endif
}

#undef VERBOSE
