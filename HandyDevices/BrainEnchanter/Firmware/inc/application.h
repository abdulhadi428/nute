/*
 * application.h
 *
 *  Created on: Nov 9, 2013
 *      Author: kreyl
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "kl_lib_L15x.h"

# if 1 // Uart Command Codes. See https://docs.google.com/document/d/1pGQf9CrQ016ObS0w7PhPLAy92MRPhdBriICflt1YGXA/edit
#define CMD_PING            0x01
#endif

#if 1 // ==== Timings ====

#endif

enum AppState_t {asIdle, asCurrent, asStandBy};

class App_t {
public:
    Thread *PThd;
    AppState_t State;
    void Init();
};

extern App_t App;

#endif /* APPLICATION_H_ */
