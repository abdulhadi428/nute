#include "chthreads.h"
#include <Windows.h>

typedef struct 
{
    tfunc_t f;
    void* p;
} sim_param;

DWORD WINAPI sim_thread(
  _In_  LPVOID lpParameter
)
{
    sim_param* p = (sim_param*)lpParameter;
    return p->f(p->p);
}

Thread *chThdCreateStatic(void *wsp, size_t size,
                            tprio_t prio, tfunc_t pf, void *arg)
{
    sim_param p;
    p.f = pf;
    p.p = arg;
    HANDLE hndl = CreateThread(NULL, 0, sim_thread, &p, 0, NULL);
    return hndl;
}

void chThdSleepSeconds(uint32_t s)
{
    Sleep(s*1000);
}

void chThdSleepMilliseconds(uint32_t msec)
{
    Sleep(msec);
}
