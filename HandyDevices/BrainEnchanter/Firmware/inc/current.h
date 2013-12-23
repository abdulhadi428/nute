/*
 * current.h
 *
 *  Created on: Dec 23, 2013
 *      Author: kreyl
 */

#ifndef CURRENT_H_
#define CURRENT_H_

#include "kl_lib_L15x.h"
#include "application.h"

class Current_t {
private:
    void EnableHighV()  { PinSet(GPIOA, 15); }
    void DisableHighV() { PinClear(GPIOA, 15); }
public:
    int32_t uA;
    uint8_t M_Set, M_Now, S_Now;
    uint32_t Get_mA_Whole() { return uA / 1000; }
    uint32_t Get_mA_Fract() { return (uA % 1000) / 100; }
    void Init();
    void Reset() {
        uA = CURRENT_INIT_uA;
        M_Set = 18;
        M_Now = 0;
        S_Now = 0;
    }
    void On();
    void Off() { DAC->DHR12R1 = 0; }
};

extern Current_t Current;

#endif /* CURRENT_H_ */
