/*
 * File:   sd.h
 * Author: Kreyl
 *
 * Created on June 5, 2011, 6:40 PM
 */

#ifndef SD_H
#define	SD_H

#include "stm32f10x.h"
#include "stm32f10x_sdio.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#include "stm32_eval_sdio_sd.h"

// ============================ Defines ========================================


// ============================= Types =========================================
class sd_t {
private:
    SD_Error Status;
public:
    void Init(void);
};

extern sd_t SD;

// ==== Declare Timer IRQ. Use externC to make it visible from asm file. =======
extern "C" {
    void SDIO_IRQHandler(void);
}



#endif	/* SD_H */

