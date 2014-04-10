/*
 * rlvl1_defins.h
 *
 *  Created on: Nov 21, 2013
 *      Author: kreyl
 */

#ifndef RLVL1_DEFINS_H_
#define RLVL1_DEFINS_H_

#include "color.h"

#if 1 // =========================== Pkt_t =====================================
struct rPkt_t {
    Color_t Clr;
    uint8_t Brightness;
} __attribute__ ((__packed__));
#define RPKT_LEN    sizeof(rPkt_t)
#endif

#if 1 // =========================== Timings ===================================
#define RX_T_MS        6

#endif



#endif /* RLVL1_DEFINS_H_ */
