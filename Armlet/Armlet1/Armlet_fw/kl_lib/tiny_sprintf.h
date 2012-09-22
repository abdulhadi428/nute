#ifndef TINY_SPRINTF_H
#define TINY_SPRINTF_H

#include <stdarg.h>

// Supported format specifiers:
//   %s - string
//   %d - int
//   %[0][<width>]u - uint
//   %[0][<width>]X - uint as hex
//   %A - pair (uint8_t *arr, int len) as hex array

int tiny_vsprintf(char *buf, const char *format, va_list args);
int tiny_sprintf(char *buf, const char *format, ...);

#endif