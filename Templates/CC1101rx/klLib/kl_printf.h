/*
 * klstring.h
 *
 *  Created on: 21.11.2011
 *      Author: Kreyl
 */

#ifndef KLSTRING_H_
#define KLSTRING_H_

#include <stdbool.h>
#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

// Prototypes
char *UintToStr(uint32_t AValue, char* S);
void klPrintfInit(void);
void klPrintf(const char *S, ...);


#ifdef __cplusplus
}
#endif

#endif /* KLSTRING_H_ */
