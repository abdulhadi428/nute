/*
 * File:   images.h
 * Author: Kreyl Laurelindo
 *
 * Created on 14 Декабрь 2010 г., 15:30
 */

#ifndef IMAGES_H
#define	IMAGES_H

#include <inttypes.h>

#if 1 // ========================== Battery ====================================
const uint8_t iconBattery80_100[] = {
        6, // width
        1,  // height
        0x7F,0x7F,0xFF,0xFF,0x7F,0x7F,
};
const uint8_t iconBattery60_80[] = { 6, 1, 0x7F,0x5F,0xDF,0xDF,0x5F,0x7F  };
const uint8_t iconBattery40_60[] = { 6, 1, 0x7F,0x4F,0xCF,0xCF,0x4F,0x7F  };
const uint8_t iconBattery20_40[] = { 6, 1, 0x7F,0x43,0xC3,0xC3,0x43,0x7F  };
const uint8_t iconBattery0_20[]  = { 6, 1, 0x7F,0x41,0xC1,0xC1,0x41,0x7F  };

#endif

#endif	/* IMAGES_H */

