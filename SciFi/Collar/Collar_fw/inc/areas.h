/*
 * areas.h
 *
 *  Created on: 15.06.2012
 *      Author: kreyl
 */

#ifndef AREAS_H_
#define AREAS_H_

#include <stdint.h>

#define AREA_COUNT  10

struct AreaList_t {
    int32_t TopLeftLattitude[AREA_COUNT];
    int32_t TopLeftLongitude[AREA_COUNT];
    uint8_t Restriction[18];    // Bit-coded restrictions
    void Init(void);
#ifdef NUTE_MODE_TIXE
    bool CoordToAreaIndx(int32_t Lattitude, int32_t Longitude, uint8_t *PIndx);
    bool IsAreaRestricted(uint32_t AIndx);
#endif
} PACKED;

extern AreaList_t Areas;



#endif /* AREAS_H_ */
