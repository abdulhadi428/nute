/*
 * lvl1_assym.h
 *
 *  Created on: 28.03.2013
 *      Author: kreyl
 */

/*
 * Level 1 of assymetric protocol.
 * Level 1 is responsible to sync rpkt exchange and therefore has
 * high priority.
 */

#ifndef LVL1_ASSYM_H_
#define LVL1_ASSYM_H_

#include "ch.h"
#include "rCommon.h"





// ==== Surround ====
struct Neighbour_t {

};

class Surround_t {
private:

public:

};

extern Surround_t Surround;
extern uint16_t rID;        // Radio ID
extern uint8_t rChannel;    // Radio channel

void rLvl1_Init();

#endif /* LVL1_ASSYM_H_ */
