#include "tiny_sprintf.h"

// Uncomment to test as standalone executable
//#define TESTING

#include <stdint.h>

#ifdef TESTING

#include <stdio.h>
#include <string.h>
#include <assert.h>
#endif


// Print number n to buffer p in base base. If number is shorter
// than width, it's prepended with spaces or zeros (if zero_padded
// is set) from the left.
// Return pointer to the next position in the buffer.
char *put_uint(char *p,
               unsigned int n,
               unsigned int base, int width, bool zero_padded) {
    char digits[10];
    int len = 0;
    do {
        unsigned int digit = n%base;
        n /= base;
        digits[len++] = digit < 10 ? '0'+digit : 'A'+digit-10;
    } while (n > 0);

    for (int i = len; i < width; i++)
        if (zero_padded) *p++ = '0';
        else             *p++ = ' ';

    while (len > 0) *p++ = digits[--len];
    return p;
}

int tiny_vsprintf(char *buf, int32_t BufSz, const char *format, va_list args) {
    BufSz--;    // leave last byte for '\0'
    const char *f = format;
    char *p = buf;
    char c;
    while ((c = *f++) != 0) {
        if (c != '%') {
            *p++ = c;
            if(--BufSz <= 0) break;
            continue;
        }

        // Here goes optional width specification.
        // If it starts with zero (zero_padded is true), it means we use '0'
        // instead of ' ' as a filler.
        int width = 0;
        bool zero_padded = false;
        while(true) {
            c = *f++;
            if (c < '0' || c > '9') break;
            if (width == 0 && c == '0') zero_padded = true;
            width *= 10;
            width += c-'0';
        }

        if((c == 's') or (c == 'S')) {
            char *s = va_arg(args, char*);
            while (*s != 0) {
                *p++ = *s++;
                if(--BufSz <= 0) break;
            }
        }
        else if (c == 'c') {
            *p++ = va_arg(args, int);
            if(--BufSz <= 0) break;
        }
        else if (c == 'X') {
            unsigned int n = va_arg(args, unsigned int);
            p = put_uint(p, n, 16, width, zero_padded);
        }
        else if (c == 'u') {
            unsigned int n = va_arg(args, unsigned int);
            p = put_uint(p, n, 10, width, zero_padded);
        }
        else if (c == 'd') {
            int n = va_arg(args, int);
            if (n < 0) {
                *p++ = '-';
                n = -n;
            }
            p = put_uint(p, n, 10, 0, false);
        }
        else if (c == 'A') {
            uint8_t *arr = va_arg(args, uint8_t*);
            int n = va_arg(args, int);
            unsigned int Delimiter = va_arg(args, unsigned int);
            for(int i = 0; i < n; i++) {
                if((i > 0) and (Delimiter != 0)) *p++ = (char)Delimiter;
                p = put_uint(p, arr[i], 16, 2, true);
            }
        }
    }
    *p = 0;
    return p-buf;
}

void PutUint2Q(OutputQueue *PQ, unsigned int n, unsigned int base, int width, bool zero_padded) {
    char digits[10];
    int len = 0;
    do {
        unsigned int digit = n%base;
        n /= base;
        digits[len++] = digit < 10 ? '0'+digit : 'A'+digit-10;
    } while (n > 0);

    for(int i = len; i < width; i++)
        if (zero_padded) chOQPutTimeout(PQ, '0', TIME_IMMEDIATE);
        else             chOQPutTimeout(PQ, ' ', TIME_IMMEDIATE);
    while(len > 0) chOQPutTimeout(PQ, digits[--len], TIME_IMMEDIATE);
}

void PrintToQueue(OutputQueue *PQ, const char *format, va_list args) {
    const char *f = format;
    char c;
    while((c = *f++) != 0) {
        if(chOQIsFullI(PQ)) return;
        if(c != '%') {
            chOQPut(PQ, c);
            continue;
        }
        // Here goes optional width specification.
        // If it starts with zero (zero_padded is true), it means we use '0' instead of ' ' as a filler.
        int width = 0;
        bool zero_padded = false;
        while(true) {
            c = *f++;
            if (c < '0' || c > '9') break;
            if (width == 0 && c == '0') zero_padded = true;
            width *= 10;
            width += c-'0';
        }

        if((c == 's') or (c == 'S')) {
            char *s = va_arg(args, char*);
            while(*s != 0) {
                chOQPut(PQ, *s++);
                if(chOQIsFullI(PQ)) return;
            }
        }
        else if (c == 'c') chOQPut(PQ, (uint8_t)va_arg(args, int));
        else if (c == 'X') {
            unsigned int n = va_arg(args, unsigned int);
            PutUint2Q(PQ, n, 16, width, zero_padded);
        }
        else if (c == 'u') {
            unsigned int n = va_arg(args, unsigned int);
            PutUint2Q(PQ, n, 10, width, zero_padded);
        }
        else if (c == 'd') {
            int n = va_arg(args, int);
            if (n < 0) {
                chOQPutTimeout(PQ, '-', TIME_IMMEDIATE);
                n = -n;
            }
            PutUint2Q(PQ, n, 10, 0, false);
        }
        else if (c == 'A') {
            uint8_t *arr = va_arg(args, uint8_t*);
            int n = va_arg(args, int);
            unsigned int Delimiter = va_arg(args, unsigned int);
            for(int i = 0; i < n; i++) {
                if((i > 0) and (Delimiter != 0)) chOQPutTimeout(PQ, Delimiter, TIME_IMMEDIATE);
                PutUint2Q(PQ, arr[i], 16, 2, true);
            }
        }
    } // while
}

//int tiny_sprintf(char *buf, const char *format, ...) {
//    va_list args;
//    va_start(args, format);
//    int result = tiny_vsprintf(buf, format, args);
//    va_end(args);
//    return result;
//}

