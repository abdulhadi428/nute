#include "tiny_sprintf.h"

// Uncomment to test as standalone executable
//#define TESTING  

#include <stdint.h>

#ifdef TESTING

#include <stdio.h>
#include <string.h>
#include <assert.h>

#else

#define assert(x) ((void)0)

#endif


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
        if (zero_padded)
            *p++ = '0';
        else
            *p++ = ' ';

    while (len > 0)
        *p++ = digits[--len];
    return p;
}

int tiny_vsprintf(char *buf, const char *format, va_list args) {
    const char *f = format;
    char *p = buf;
    char c;
    while ((c = *f++) != 0) {
        if (c != '%') {
            *p++ = c;
            continue;
        }
        int width = 0;
        bool zero_padded = false;
        while (true) {
            c = *f++;
            if (c < '0' || c > '9')
                break;
            if (width ==0 && c == '0')
                zero_padded = true;
            width *= 10;
            width += c-'0';
        }
        if (c == 's') {
            assert(width == 0 && !zero_padded); // not implemented
            char *s = va_arg(args, char*);
            while (*s != 0)
                *p++ = *s++;
        }
        else if (c == 'c') {
            assert(width == 0 && !zero_padded); // not implemented
            *p++ = va_arg(args, int);
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
            assert(width == 0 && !zero_padded); // not implemented
            int n = va_arg(args, int);
            if (n < 0) {
                *p++ = '-';
                n = -n;
            }
            p = put_uint(p, n, 10, 0, false);
        }
        else if (c == 'A') {
            assert(width == 0 && !zero_padded); // not implemented
            uint8_t *arr = va_arg(args, uint8_t*);
            int n = va_arg(args, int);
            for (int i = 0; i < n; i++) {
                if (i > 0)
                    *p++ = ' ';
                p = put_uint(p, arr[i], 16, 2, true);
            }
        }
        else 
            assert(false);
    }
    *p = 0;
    return p-buf;
}

int tiny_sprintf(char *buf, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int result = tiny_vsprintf(buf, format, args);
    va_end(args);
    return result;
}


#ifdef TESTING
void test_sprintf(char *expected, const char *format, ...) {
    va_list args;
    va_start(args, format);
    char buf[1000];
    int len = tiny_vsprintf(buf, format, args);
    va_end(args);
    printf("expected: %s\n     got: %s\n", expected, buf);
    assert(strcmp(buf, expected) == 0);
    assert(strlen(buf) == len);
}

int main(int argc, char* argv[])
{
    test_sprintf("hello world!", "hello %s!", "world");

    test_sprintf("x = 2A", "x = %X", 42);
    test_sprintf("x =  0", "x = %2X", 0);
    test_sprintf("x = 0FFFFFFFF", "x = %09X", -1);

    test_sprintf("x = 123", "x = %u", 123);
    test_sprintf("x = 0", "x = %u", 0);
    test_sprintf("x = 4294967295", "x = %u", -1);

    test_sprintf("x = 123", "x = %d", 123);
    test_sprintf("x = 0", "x = %d", 0);
    test_sprintf("x = -123", "x = %d", -123);

    char dead_beef[] = {0xDE, 0xAD, 0xBE, 0xEF, 0};
    test_sprintf("arr = DE AD BE EF 00", "arr = %A", dead_beef, 5);

    printf("All ok\n");
    return 0;
}
#endif
