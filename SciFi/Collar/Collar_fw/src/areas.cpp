/*
 * areas.cpp
 *
 *  Created on: 15.06.2012
 *      Author: kreyl
 */

#include "areas.h"

AreaList_t Areas;

void AreaList_t::Init() {
    // Initial restrictions
    Restriction[0]  = 0xDE;
    Restriction[1]  = 0xAD;
    Restriction[16] = 0xBE;
    Restriction[17] = 0xEF;
}

#ifdef NUTE_MODE_TIXE
bool AreaList_t::CoordToAreaIndx(int32_t Lattitude, int32_t Longitude, uint8_t *PIndx) {

}
#ifdef NUTE_MODE_TIXE
