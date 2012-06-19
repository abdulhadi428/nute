/*
 * areas.h
 *
 *  Created on: 15.06.2012
 *      Author: kreyl
 */

#ifndef AREAS_H_
#define AREAS_H_

#include <stdint.h>

struct Area_t {
    uint8_t Indx;
    int32_t BLLatitude, BLLongitude, TRLatitude, TRLongitude;
};

#define AREA_LAT_COUNT  3
#define AREA_LNG_COUNT  3
#define AREA_COUNT      (AREA_LAT_COUNT*AREA_LNG_COUNT)

const Area_t Area[AREA_LAT_COUNT][AREA_LNG_COUNT] = {
        { // Lat0
            {0, 556544160,   375936280,   556547940,   375943573},   // lng0
            {1, 556544160,   375943573,   556547940,   375950866},   // lng1
            {2, 556544160,   375950866,   556547940,   375958159},   // lng2
        },

        { // Lat1
            {3, 556547940,   375936280,   556551720,   375943573},
            {4, 556547940,   375943573,   556551720,   375950866},
            {5, 556547940,   375950866,   556551720,   375958159},
        },

        { // Lat2
            {6, 556551720,   375936280,   556555500,   375943573},
            {7, 556551720,   375943573,   556555500,   375950866},
            {8, 556551720,   375950866,   556555500,   375958159},
        }
};

struct AreaList_t {
    uint8_t Restriction[18];    // Bit-coded restrictions
    void Init(void);
#ifdef NUTE_MODE_TIXE
    bool CoordToAreaIndx(int32_t ALatitude, int32_t ALongitude, uint8_t *PIndx);
    bool IsAreaRestricted(uint32_t AIndx);
#endif
} PACKED;

extern AreaList_t AreaList;



#endif /* AREAS_H_ */
