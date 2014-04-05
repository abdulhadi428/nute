/*
 * ws2812b.h
 *
 *  Created on: 05 апр. 2014 г.
 *      Author: Kreyl
 */

#ifndef WS2812B_H_
#define WS2812B_H_

#include "stm32f10x.h"
#include "ch.h"
#include "hal.h"
#include "kl_lib_f100.h"
#include "color.h"
#include "main.h"

#if LED_WS_ENABLE

#define LED_CNT         3
#define RST_BIT_CNT     45 // 45 zero bits to produce reset
#define DATA_BIT_CNT    (LED_CNT * 3 * 8)   // 3 channels 8 bit each
#define TOTAL_BIT_CNT   (DATA_BIT_CNT + RST_BIT_CNT)

class LedWs_t {
private:
    Timer_t TxTmr;
    uint8_t BitBuf[TOTAL_BIT_CNT], *PBit;
    void AppendBitsMadeOfByte(uint8_t Byte);
public:
    void Init();
    void SetCommonColor(Color_t Clr);
    // Inner use
    void IStopTx() { TxTmr.SetPwm(0); TxTmr.Disable(); }
};

extern LedWs_t LedWs;
#endif

#endif /* WS2812B_H_ */
