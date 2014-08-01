/*
 * rlvl1_defins.h
 *
 *  Created on: Nov 21, 2013
 *      Author: kreyl
 */

#ifndef RLVL1_DEFINS_H_
#define RLVL1_DEFINS_H_

#if 1 // =========================== Pkt_t =====================================
struct rPkt_t {
    uint32_t MagicNumber;
} __attribute__ ((__packed__));
#define RPKT_LEN    sizeof(rPkt_t)
#endif



#endif /* RLVL1_DEFINS_H_ */
