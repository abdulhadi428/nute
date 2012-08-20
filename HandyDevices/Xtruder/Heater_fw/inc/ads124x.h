/*
 * ads124x.h
 *
 *  Created on: 21.08.2012
 *      Author: kreyl
 */

#ifndef ADS124X_H_
#define ADS124X_H_

#include "kl_lib.h"

class Ads124x_t {
private:
    // GPIOs
    void PwdnHi(void) { klGpioSetByMsk  (GPIOA, GPIO_Pin_2); }
    void PwdnLo(void) { klGpioClearByMsk(GPIOA, GPIO_Pin_2); }
    void CsHi(void)   { klGpioSetByMsk  (GPIOA, GPIO_Pin_4); }
    void CsLo(void)   { klGpioClearByMsk(GPIOA, GPIO_Pin_4); }
    bool DataIsReady(void) { return klGpioIsClearByMsk(GPIOA, GPIO_Pin_3); }
    // SPI
    uint8_t ReadWriteByte(uint8_t AByte);
public:
    void Init(void);

};

extern Ads124x_t Ads;

#endif /* ADS124X_H_ */
