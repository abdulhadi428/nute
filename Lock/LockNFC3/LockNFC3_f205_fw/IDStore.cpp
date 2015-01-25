/*
 * IDStore.cpp
 *
 *  Created on: 25.11.2011
 *      Author: Kreyl
 */

#include "IDStore.h"

//#define DO_NOT_SAVE   // Use this to save Flash when debugging

IdKind_t IDStore_t::Check(ID_t &sID, uint32_t *PIndx) {
    if     (IDAccess.ContainsID(sID, PIndx))  return ikAccess;
    else if(IDAdder.ContainsID(sID, PIndx))   return ikAdder;
    else if(IDRemover.ContainsID(sID, PIndx)) return ikRemover;
    else if(IDSpecial.ContainsID(sID, PIndx)) return ikSpecial;
    else return ikNone;
}

// =============================== Load/save ===================================
void IDStore_t::Load() {
//    IsChanged = false;
//    IDArr.FlashEraseCounter = SavedIDs->FlashEraseCounter;
//    IDArr.Count = SavedIDs->Count;
//    //klPrintf("Loading IDs...\r");
//    klPrintf("Flash Erase counter = %u\r", SavedIDs->FlashEraseCounter);
//    if (IDArr.Count == 0) {   // nothing interesting there
//        klPrintf("Flash is empty\r");
//    }
//    else {
//        for(uint32_t i=0; i<IDArr.Count; i++) IDArr.ID[i] = SavedIDs->ID[i];
//        PrintIDs();
//        klPrintf("Loading completed\r\r");
//    }
}

void IDStore_t::Save(void) {
    HasChanged = false;
    FRESULT Rslt = f_open(&IFile, IDSTORE_FILENAME, FA_WRITE+FA_CREATE_ALWAYS);
    if(Rslt != FR_OK) { Uart.Printf("\rIdStore file create error: %u", Rslt); return; }
    // Write Access group
    IDAccess.Save(&IFile, ID_GROUP_ACCESS);

    f_close(&IFile);
}

#if 1 // ========================== ID array ===================================

#endif
