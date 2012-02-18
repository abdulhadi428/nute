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

#define ID_MAX_COUNT    11 // Fit in 1024 bytes (1 pages)

typedef struct {
    uint32_t Count, FlashEraseCounter;
    uint32_t ID[ID_MAX_COUNT];
} ID_Array_t;

class IDStore_t {
private:
    bool IsPresentIndx(uint32_t AID, uint32_t *AIndx);
public:
    ID_Array_t IDArr;
    bool IsChanged;
    // ID operations
    void Add(uint32_t AID);
    // Load/save
    void Load(void);
    void Save(void);
    void EraseAll(void);
};

extern IDStore_t IDStore;

#endif /* IDSTORE_H_ */
