#include "kl_lib_f2xx.h"
#include <stdio.h>
#include <stdarg.h>

DbgUart_t Uart;

void DbgUart_t::Printf(const char *S, ...)
{
    va_list lst;
    va_start(lst, S);
    vprintf(S, lst);
    va_end(lst);
}
