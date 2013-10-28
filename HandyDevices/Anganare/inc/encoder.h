/*
 * encoder.h
 *
 *  Created on: 25 окт. 2013 г.
 *      Author: r.leonov
 */

#ifndef ENCODER_H_
#define ENCODER_H_



#include "kl_lib_f100.h"

#define ENCODER_TIM     TIM3
#define ENCODER_GPIO    GPIOB
#define ENCODER_PIN1    4
#define ENCODER_PIN2    5

#define MAX_VALUE 		99
#define MIN_VALUE       0

class encoder_t {
private:
    uint32_t CurrentValue;
    uint32_t *PCnt;
    inline void InitGpios() {
        PinSetupIn(ENCODER_GPIO, ENCODER_PIN1, pudPullUp);
        PinSetupIn(ENCODER_GPIO, ENCODER_PIN2, pudPullUp);
        // TIM3 remap GPIOS
        AFIO->MAPR |= AFIO_MAPR_TIM3_REMAP_PARTIALREMAP;
    }
public:
    uint32_t Value;
    uint32_t GetValue() { return CurrentValue; }
    void Init();
    void Task();

    void IrqHandler();
};

extern encoder_t Encoder;

#endif /* ENCODER_H_ */
