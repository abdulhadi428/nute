/*
 * gps.h
 *
 *  Created on: 27.05.2012
 *      Author: kreyl
 */

#ifndef GPS_H_
#define GPS_H_

#include "kl_lib.h"
#include "stdint.h"
#include "nute.h"

#define GPS_BUF_SIZE    90

const char MsgHdr[] = "GPGGA";
#define MsgHdrSz    (sizeof(MsgHdr))

// Lattitude, Longtitude == (Deg * 10^7) + (Min * 10^7 / 60)

class GpsR_t {
private:
    char IBuf[GPS_BUF_SIZE];
    char IMsg[81];
    void IParseGPGGAMsg(void);
public:
    void Init(void);
    void Task(void);
};

extern GpsR_t Gps;
extern Situation_t *Situation;


#endif /* GPS_H_ */
