/*
 * sim900.h
 *
 *  Created on: 01.06.2012
 *      Author: kreyl
 */

#ifndef SIM900_H_
#define SIM900_H_

#include <stdint.h>
#include "kl_lib.h"

class sim900_t {
private:
    // Init
    void GPIOInit(void);
    void USARTInit(void);
    // Pins operations
    void PwrKeyHi(void) { klGpioSetByMsk(GPIOA, GPIO_Pin_15); }
    void PwrKeyLo(void) { klGpioClearByMsk(GPIOA, GPIO_Pin_15);}
public:
    void Init(void);

};

extern sim900_t Mdm;

#endif /* SIM900_H_ */
