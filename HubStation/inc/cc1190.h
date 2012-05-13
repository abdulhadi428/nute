/*
 * cc1190.h
 *
 *  Created on: 05.05.2012
 *      Author: kreyl
 */

#ifndef CC1190_H_
#define CC1190_H_

#include "kl_lib.h"

#define CC1190_GPIO GPIOA
#define CC_LNA_EN   11
#define CC_PA_EN    12
#define CC_HGM      15

class cc1190_t {
private:
    klPin_t LnaEn, PaEn, Hgm;
public:
    void LnaEnable(void)  { PaEn = 0; LnaEn = 1; }
    void LnaDisable(void) { LnaEn = 0; }
    void PaEnable(void)   { LnaEn = 0; PaEn = 1; }
    void PaDisable(void)  { PaEn = 0; }
    void SetHighGainMode(void) { Hgm = 1; }
    void SetLowGainMode(void)  { Hgm = 0; }
    void Powerdown(void) { Hgm = 0; LnaEn = 0; PaEn = 0; }
    void Init(void) {
        LnaEn.Init(CC1190_GPIO, CC_LNA_EN, GPIO_Mode_Out_PP);
        PaEn.Init (CC1190_GPIO, CC_PA_EN,  GPIO_Mode_Out_PP);
        Hgm.Init  (CC1190_GPIO, CC_HGM,    GPIO_Mode_Out_PP);
        Powerdown();
    }
};


#endif /* CC1190_H_ */
