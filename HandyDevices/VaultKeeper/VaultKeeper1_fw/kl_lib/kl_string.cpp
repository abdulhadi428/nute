/*
 * kl_string.cpp
 *
 *  Created on: 11.06.2012
 *      Author: kreyl
 */

#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include "kl_string.h"

// Copies Src to Dst, null-terminating Dst. Returns pointer to NULL at Dst
char* klStrCpy(char *Dst, const char *Src) {
    char c;
    while ((c = *Src++) != 0) *Dst++ = c;
    *Dst = 0;   // Null-terminate result
    return Dst;
}

// Copies ACount bytes of Src to Dst, null-terminating Dst. Returns pointer to NULL at Dst
char* klStrNCpy(char *Dst, const char *Src, uint32_t ACount) {
    char c;
    while (((c = *Src++) != 0) and (ACount-- != 0)) *Dst++ = c;
    *Dst = 0;   // Null-terminate result
    return Dst;
}



// ============================== Sprint works =================================
void SprintUint(uint32_t ANumber, uint8_t ACharCount, char **PPSrc) {
    char *S;
    S = *PPSrc;
    uint8_t digit = '0';
    bool ShouldPrint = false;
    const uint32_t m[9] = {1000000000, 100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10};

    for(uint8_t i=0; i<9; i++) {
        while (ANumber >= m[i]) {
            digit++;
            ANumber -= m[i];
        }
        if ((digit != '0') or ShouldPrint or (9-i < ACharCount)) {
            *S++ = digit;
            ShouldPrint = true;
        }
        digit = '0';
    } // for
    *S++ = (char)('0'+ANumber);
    *PPSrc = S;
}

void SPrintInt (int32_t ANumber, uint8_t ACharCount, char **PPSrc) {
    if (ANumber < 0) {
        char *S = *PPSrc;
        *S++ = '-';
        ANumber = -ANumber;
        *PPSrc = S;
    }
    SprintUint(ANumber, ACharCount, PPSrc);
}

void SPrintString (const char *Str, char **PPSrc) {
    char *S, c;
    S = *PPSrc;
    while ((c = *Str++) != 0) *S++ = c;
    *PPSrc = S;
}


char SUintToHexChar (uint8_t b) {
    return ((b<=0x09) ? (b+'0') : (b+'A'-10));
}
void SPrintAsHex (uint32_t ANumber, uint8_t ACharCount, char **PPSrc) {
    char *S = *PPSrc;
    bool MustPrint = false;
    if ((ANumber & 0xFF000000) or (ACharCount > 6)) {
        *S++ = SUintToHexChar(0x0F & (ANumber >> 28));
        *S++ = SUintToHexChar(0x0F & (ANumber >> 24));
        MustPrint = true;
    }
    if ((ANumber & 0x00FF0000) or MustPrint or (ACharCount > 4)) {
        *S++ = SUintToHexChar (0x0F & (ANumber >> 20));
        *S++ = SUintToHexChar (0x0F & (ANumber >> 16));
        MustPrint = true;
    }
    if ((ANumber & 0x0000FF00) or MustPrint or (ACharCount > 2)) {
        *S++ = SUintToHexChar (0x0F & (ANumber >> 12));
        *S++ = SUintToHexChar (0x0F & (ANumber >> 8));
    }
    *S++ = SUintToHexChar (0x0F & (ANumber >> 4));
    *S++ = SUintToHexChar (0x0F & (ANumber));
    *PPSrc = S;
}


/*
 * c: character
 * i: signed int
 * u: unsigned int
 * s, S: string
 * X: hexadecimal, capital letters
 * (1...9) width - Minimum number of characters to be printed
 * A - array as 8-bit hexadecimal, length is to be next after uint8_t*
 */
char* klSPrintf(char *Dst, const char *Src, ...) {
    char c;
    bool WasPercent = false;
    va_list Arg;
    va_start(Arg, Src);    // Set pointer to last argument
    while ((c = *Src) != 0) {
        if (c == '%') {
            if (WasPercent) {
                *Dst++ = c;  // %%
                WasPercent = false;
            }
            else WasPercent = true;
        }
        else { // not %
            if (WasPercent) {
                // Get number of characters
                char c1 = *(Src+1);
                uint8_t N;
                if ((c1 >= '0') and (c1 <= '9')) {
                    N = c1 - '0';
                    Src++;
                }
                else N = 0;
                // Parse c
                if (c == 'c') *Dst++ = ((uint8_t)va_arg(Arg, int32_t));
                else if (c == 'u') SprintUint(va_arg(Arg, uint32_t), N, &Dst);
                else if (c == 'i') SPrintInt(va_arg(Arg, int32_t), N, &Dst);
                else if (c == 'X') SPrintAsHex(va_arg(Arg, uint32_t), N, &Dst);
                else if ((c == 's') || (c == 'S')) SPrintString(va_arg(Arg, char*), &Dst);
                //else if (c == 'A') IPrint8HexArray(va_arg(Arg, uint8_t*), va_arg(Arg, uint32_t));
                WasPercent = false;
            } // if was percent
            else *Dst++ = c;
        }
        Src++;
    } // while
    va_end(Arg);
    *Dst = 0;
    return Dst;
}

