/*
 * IDStore.h
 *
 *  Created on: 25.11.2011
 *      Author: Kreyl
 */

#ifndef IDSTORE_H_
#define IDSTORE_H_

#include <inttypes.h>

#define ID_COUNT    100

class IDStore_t {
private:
    uint64_t ID[ID_COUNT];
    bool IsPresentIndx(uint64_t AID, uint32_t *AIndx);
public:
    // ID operations
    bool IsPresent(uint64_t AID) { return IsPresentIndx(AID, 0); }
    void Add(uint64_t AID);
    void Remove(uint64_t AID);
    // Load/save
    void Load(void);
    void Save(void);
};

extern IDStore_t IDStore;

#endif /* IDSTORE_H_ */
