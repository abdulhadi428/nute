#ifndef SIM_KL_LIB_F2XX__H
#define SIM_KL_LIB_F2XX__H

class DbgUart_t
{
public:
    void Printf(const char *S, ...);
};

extern DbgUart_t Uart;

#endif
