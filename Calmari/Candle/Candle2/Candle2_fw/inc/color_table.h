#ifndef __COLORTABLE_H_
#define __COLORTABLE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <avr/pgmspace.h>

#define COLOR_COUNT    97

uint8_t GetColor(uint8_t Indx, uint8_t Chnl);

extern prog_uint8_t ColorTable[COLOR_COUNT][3];


#ifdef __cplusplus
}
#endif

#endif
