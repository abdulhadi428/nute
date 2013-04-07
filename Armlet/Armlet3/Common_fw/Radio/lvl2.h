/*
 * lvl2.h
 *
 *  Created on: Apr 3, 2013
 *      Author: g.kruglov
 */

#ifndef LVL2_H_
#define LVL2_H_

#include "lvl1_assym.h"

class rLevel2_t {
private:
    Semaphore AddPktSem;
public:
    void Init();
    void Transmit(uint8_t DevID, uint8_t *Ptr, int32_t Length, uint8_t PResult);
};

extern rLevel2_t rLevel2;

#endif /* LVL2_H_ */
