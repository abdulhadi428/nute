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

#define ID_MAX_COUNT    252 // Fit in 2*1024 bytes (2 pages)

typedef struct {
    uint32_t Count, FlashEraseCounter;
    uint64_t ID[ID_MAX_COUNT];
} ID_Array_t;

class IDStore_t {
private:
    ID_Array_t IDArr;
    bool IsPresentIndx(uint64_t AID, uint32_t *AIndx);
    void PrintIDs(void);
public:
    bool IsChanged;
    // ID operations
    bool IsPresent(uint64_t AID) { return IsPresentIndx(AID, 0); }
    void Add(uint64_t AID);
    void Remove(uint64_t AID);
    // Load/save
    void Load(void);
    void Save(void);
    void EraseAll(void);
};

extern IDStore_t IDStore;

#endif /* IDSTORE_H_ */
