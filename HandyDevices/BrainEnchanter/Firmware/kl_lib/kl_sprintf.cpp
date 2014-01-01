#include "kl_sprintf.h"
#include <stdint.h>

// Print number n to buffer p in base base. If number is shorter
// than width, it's prepended with spaces or zeros (if zero_padded
// is set) from the left.
void vsPrintf_t::IPutUint(uint32_t MaxLength, uint32_t n, uint32_t base, uint32_t width, bool zero_padded) {
    char digits[10];
    uint32_t len = 0;
    // Place digits to buffer
    do {
        uint32_t digit = n % base;
        n /= base;
        digits[len++] = (digit < 10)? '0'+digit : 'A'+digit-10;
    } while(n > 0);

    // Add padding
    for(uint32_t i = len; i < width; i++) {
        if(zero_padded) IPutChar('0');
        else            IPutChar(' ');
        if(++CharCnt >= MaxLength) return;
    }
    // Print digits
    len = (len < (MaxLength - CharCnt))? len : (MaxLength - CharCnt);
    while(len > 0) IPutChar(digits[--len]);
}

void vsPrintf_t::vsprintf(uint32_t MaxLength, const char *format, va_list args) {
    CharCnt = 0;
    const char *f = format;
    char c;
    while((c = *f++) != 0) {
        if(c != '%') {
            IPutChar(c);
            if(CharCnt >= MaxLength) break;
            else continue;
        }

        // Here goes optional width specification.
        // If it starts with zero (zero_padded is true), it means we use '0'
        // instead of ' ' as a filler.
        int width = 0;
        bool zero_padded = false;
        while(true) {
            c = *f++;
            if(c < '0' || c > '9') break;
            if((width == 0) and (c == '0')) zero_padded = true;
            width *= 10;
            width += c-'0';
        }

        if((c == 's') or (c == 'S')) {
            char *s = va_arg(args, char*);
            while((*s != 0) and (CharCnt < MaxLength)) IPutChar(*s++);
        }
        else if(c == 'c') IPutChar(va_arg(args, int));
        else if(c == 'X') IPutUint(MaxLength, va_arg(args, uint32_t), 16, width, zero_padded);
        else if(c == 'u') IPutUint(MaxLength, va_arg(args, uint32_t), 10, width, zero_padded);
        else if(c == 'd') {
            int32_t n = va_arg(args, int32_t);
            if(n < 0) {
                IPutChar('-');
                if(CharCnt >= MaxLength) break;
                n = -n;
            }
            IPutUint(MaxLength, n, 10, width, zero_padded);
        }
        else if(c == 'A') {
            uint8_t *arr = va_arg(args, uint8_t*);
            int32_t n = va_arg(args, int32_t);
            int32_t Delimiter = va_arg(args, int32_t);
            for(int32_t i = 0; i < n; i++) {
                if((i > 0) and (Delimiter != 0)) {
                    IPutChar((char)Delimiter); // do not place delimiter before or after array
                    if(CharCnt >= MaxLength) break;
                }
                IPutUint(MaxLength, arr[i], 16, 2, true);
                if(CharCnt >= MaxLength) break;
            }
        }
        if(CharCnt >= MaxLength) break;
    }
}
