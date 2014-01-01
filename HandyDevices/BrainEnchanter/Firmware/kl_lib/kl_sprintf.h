#ifndef KL_SPRINTF_H
#define KL_SPRINTF_H

#include <stdarg.h>
#include <stdint.h>

/*
Supported format specifiers:
%s - string
%c - char
%d - int
%[0][<width>]u - uint
%[0][<width>]X - uint as hex
%A - pair (uint8_t *arr, int len) as hex array
*/

class vsPrintf_t {
private:
    char *PBuf;
    uint32_t IBufSz;
    void IPutChar(char c) {
        *PWrite++ = c;
        if(PWrite >= &PBuf[IBufSz]) PWrite = PBuf;  // Circulate buff
        CharCnt++;
    }
    void IPutUint(uint32_t MaxLength, uint32_t n, uint32_t base, uint32_t width, bool zero_padded);
public:
    char *PWrite;
    void Init(char *Ptr, uint32_t Sz) { PBuf = Ptr; PWrite = Ptr; IBufSz = Sz; }
    uint32_t CharCnt;
    void vsprintf(uint32_t MaxLength, const char *format, va_list args);
};

#endif
