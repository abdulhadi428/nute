/*
 * areas.cpp
 *
 *  Created on: 15.06.2012
 *      Author: kreyl
 */

#include "areas.h"
#include "kl_lib.h"

AreaList_t AreaList;

void AreaList_t::Init() {
    // Initial restrictions
    Restriction[0]  = 0b00001000;
    Restriction[1]  = 0x00;
    Restriction[16] = 0x00;
    Restriction[17] = 0x00;
}

#ifdef NUTE_MODE_TIXE
bool AreaList_t::CoordToAreaIndx(int32_t ALatitude, int32_t ALongitude, uint8_t *PIndx) {
    // Find latitude
    for (uint8_t lat=0; lat<AREA_LAT_COUNT; lat++) {
        if((Area[lat][0].BLLatitude <= ALatitude) and (ALatitude < Area[lat][0].TRLatitude)) {
            // Find longitude
            for (uint8_t lng=0; lng<AREA_LNG_COUNT; lng++) {
                if((Area[lat][lng].BLLongitude <= ALongitude) and (ALongitude < Area[lat][lng].TRLongitude)) {
                    // Area found
                    *PIndx = Area[lat][lng].Indx;
                    CmdUnit.Printf("Lat: %u; Lng: %u; Indx: %u\r", lat, lng, Area[lat][lng].Indx);
                    return true;
                } // if lng
            } // for lng
        } // if latitude
    } // for latitude
    return false;
}
bool AreaList_t::IsAreaRestricted(uint32_t AIndx) {
    uint8_t ByteIndx = AIndx/8;
    uint8_t BitIndx = AIndx - (ByteIndx * 8);
    uint8_t Mask = 1<<BitIndx;
    //CmdUnit.Printf("Byte: %u, %X; Bit: %u; r: %u\r", ByteIndx, Restriction[ByteIndx], BitIndx, (Mask & Restriction[ByteIndx]));
    return (Mask & Restriction[ByteIndx]);
}

#endif
