/*
 * File:   vs.h
 * Author: Kreyl
 *
 * Created on June 9, 2011, 11:46 AM
 */

#ifndef VS_H
#define	VS_H

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

// Defines
#define VS_XCS          GPIO_Pin_0
#define VS_XDCS         GPIO_Pin_4
#define VS_RST          GPIO_Pin_15
#define VS_DREQ         GPIO_Pin_1
#define VS_XCLK         GPIO_Pin_5
#define VS_SO           GPIO_Pin_6
#define VS_SI           GPIO_Pin_7

class VS_t {
private:
    void Rst_Lo(void) { GPIOA->BRR  = VS_RST; }
    void Rst_Hi(void) { GPIOA->BSRR = VS_RST; }
    void XCS_Lo(void) { GPIOA->BRR  = VS_XCS; }
    void XCS_Hi(void) { GPIOA->BSRR = VS_XCS; }
    void XDCS_Lo(void) { GPIOA->BRR  = VS_XDCS; }
    void XDCS_Hi(void) { GPIOA->BSRR = VS_XDCS; }
public:
    void Init(void);
};

extern VS_t Vs;

#endif	/* VS_H */

