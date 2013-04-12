#ifndef CHTHREADS__H
#define CHTHREADS__H

#include <stdint.h>
#include "chtypes.h"

typedef void Thread; 

typedef msg_t (*tfunc_t)(void *);
Thread *chThdCreateStatic(void *wsp, size_t size,
                            tprio_t prio, tfunc_t pf, void *arg);

void chThdSleepSeconds(uint32_t s);
void chThdSleepMilliseconds(uint32_t msec);


#endif
