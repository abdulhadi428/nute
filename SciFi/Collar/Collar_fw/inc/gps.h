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

#define GPS_BUF_SIZE    90

enum GpsState_t {gsDisconnected, gsNoPosition, gsFixed};

const char MsgHdr[] = "GPGGA";
#define MsgHdrSz    (sizeof(MsgHdr))

class GpsR_t {
private:
    char IBuf[GPS_BUF_SIZE];
    char IMsg[81];
    void IParseGPGGAMsg(void);
public:
    GpsState_t State;
    // Coordinates
    bool LatNorth, LongEast;
    Time_t Time;
    void Init(void);
    void Task(void);
};

extern GpsR_t Gps;

#endif /* GPS_H_ */
