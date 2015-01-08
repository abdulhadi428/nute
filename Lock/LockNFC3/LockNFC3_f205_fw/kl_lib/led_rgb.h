/*
 * led_rgb.h
 *
 *  Created on: 31 рту. 2014 у.
 *      Author: Kreyl
 */

#ifndef LED_RGB_H_
#define LED_RGB_H_

#include "hal.h"
#include "color.h"
#include "ChunkTypes.h"

#ifdef STM32F2XX
#include "kl_lib_f2xx.h"
#endif

//#include "kl_lib_f100.h"



// ======================= Lite version: blinker only ==========================
struct LedChnl_t {
    GPIO_TypeDef *PGpio;
    uint16_t Pin;
    void Init() const { PinSetupOut(PGpio, Pin, omPushPull); }
    void On()   const { PinSet(PGpio, Pin); }
    void Off()  const { PinClear(PGpio, Pin); }
};

class LedRgbBlinker_t {
public:
    LedChnl_t R, G, B;

    void Init() {
        R.Init();
        G.Init();
        B.Init();
        SetColor(clBlack);
    }
    void SetColor(Color_t AColor) {
        if(AColor.R != 0) R.On(); else R.Off();
        if(AColor.G != 0) G.On(); else G.Off();
        if(AColor.B != 0) B.On(); else B.Off();
    }
    // Inner use
    const LedChunk_t *IPStartChunk;
};

// ================================= LedRGB ====================================
struct LedChnlTmr_t {
    GPIO_TypeDef *PGpio;
    uint16_t Pin;
    TIM_TypeDef *PTimer;
    uint32_t TmrChnl;
    void Set(const uint8_t AValue) const { *(uint16_t*)(&PTimer->CCR1 + (TmrChnl-1)*2) = AValue; }    // CCR[N] = AValue
    void Init() const;
};

#define LED_TOP_VALUE       255
#define LED_INVERTED_PWM    FALSE
// TimeToWaitBeforeNextAdjustment = SmoothVar / (N+4) + 1, where N - current LED brightness.

class LedRGB_t {
public:
    LedChnlTmr_t R, G, B;
/*    void Init();
    void SetColor(Color_t AColor) {
        R.Set(AColor.R);
        G.Set(AColor.G);
        B.Set(AColor.B);
        ICurrColor = AColor;
    }
    void StartSequence(const LedChunk_t *PLedChunk) {
        chSysLock();
        IPStartChunk = PLedChunk;   // Save first chunk
        IStartSequenceI(PLedChunk);
        chSysUnlock();
    }
    void Stop() {
        chSysLock();
        if(chVTIsArmedI(&ITmr)) chVTResetI(&ITmr);
        SetColor(clBlack);
        chSysUnlock();
    }
    const LedChunk_t* GetCurrentSequence() { return IPStartChunk; }
    // Inner use
    void IStartSequenceI(const LedChunk_t *PLedChunk);
private:
    const LedChunk_t *IPStartChunk;
    uint32_t ICalcDelay(uint32_t CurrentBrightness, uint32_t SmoothVar) { return (uint32_t)((SmoothVar / (CurrentBrightness+4)) + 1); }
    VirtualTimer ITmr;
    Color_t ICurrColor;
    void ISetCurrent() {
        R.Set(ICurrColor.R);
        G.Set(ICurrColor.G);
        B.Set(ICurrColor.B);
    }
    */
};

#endif /* LED_RGB_H_ */
