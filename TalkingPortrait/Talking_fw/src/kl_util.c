/*
 * kl_util.c
 *
 *  Created on: 21.11.2011
 *      Author: Kreyl
 */

#include "kl_util.h"

/*
 * Converts AValue to NULL-terminated string S.
 * Returns pointer to NULL-terminating symbol.
 */
char *UintToStr(uint32_t AValue, char* S) {
    char digit = '0';
    bool ShouldPrint = false;
    const uint32_t m[9] = {1000000000, 100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10};

    for(uint32_t i=0; i<9; i++) {
        while (AValue >= m[i]) {
            digit++;
            AValue -= m[i];
        }
        if (digit != '0' || ShouldPrint) {
            *S++ = digit;
            ShouldPrint = true;
        }
        digit = '0';
    } // for
    *S++ = ((uint8_t)('0'+AValue));
    *S = 0;
    return S;
}
