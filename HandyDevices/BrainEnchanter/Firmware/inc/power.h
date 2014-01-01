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
    uint32_t uA2mA_Whole(uint32_t AuA) { return uA / 1000; }
    uint32_t uA2mA_Fract(uint32_t AuA) { return (uA % 1000) / 100; }
    void InitHardware();
    void ResetValues() {
        uA = CURRENT_INIT_uA;
        M_Set = 18;
        M_Now = 0;
        S_Now = 0;
    }
    void On();
    void Off();
    int32_t Adc2uA(int32_t AdcValue) { return ((3000 * AdcValue) + 26000) / 2873; }
};

extern Current_t Current;


#if 1 // ======================= Measurement ===================================
enum AdcSampleTime_t {
        ast4Cycles = 0b000,
        ast9Cycles = 0b001,
        ast16Cycles = 0b010,
        ast24Cycles = 0b011,
        ast48Cycles = 0b100,
        ast96Cycles = 0b101,
        ast192Cycles = 0b110,
        ast384Cycles = 0b111
};

enum ADCDiv_t {
    adcDiv1 = (uint32_t)(0b00 << 16),
    adcDiv2 = (uint32_t)(0b01 << 16),
    adcDiv4 = (uint32_t)(0b10 << 16),
};

// DMA
#define ADC_DMA             STM32_DMA1_STREAM1
#define ADC_DMA_MODE        DMA_PRIORITY_LOW | \
                            STM32_DMA_CR_MSIZE_HWORD | \
                            STM32_DMA_CR_PSIZE_HWORD | \
                            STM32_DMA_CR_MINC |       /* Memory pointer increase */ \
                            STM32_DMA_CR_DIR_P2M |    /* Direction is peripheral to memory */ \
                            STM32_DMA_CR_TCIE         /* Enable Transmission Complete IRQ */


// ==== Config this ====
// ADC channels
#define BATTERY_CHNL        0
#define CURRENT_CHNL        2
const uint32_t AdcChannels[] = {BATTERY_CHNL, CURRENT_CHNL};
#define ADC_CHANNEL_CNT     2   // Do not use countof(AdcChannels) as preprocessor does not know what is countof => cannot check
#define ADC_SAMPLE_TIME     ast96Cycles
#define ADC_SAMPLE_CNT      8   // How many times to measure every channel

// Defines, do not touch
#define ADC_MAX_SEQ_LEN     27  // 1...27 in low density devices
#define ADC_SEQ_LEN         (ADC_SAMPLE_CNT * ADC_CHANNEL_CNT)
#if (ADC_SEQ_LEN > ADC_MAX_SEQ_LEN) || (ADC_SEQ_LEN == 0)
#error "Wrong ADC channel count and sample count"
#endif

class Measure_t {
private:
    uint16_t IBuf[ADC_SEQ_LEN];
    void SetupClk(ADCDiv_t Div) { ADC->CCR = (uint32_t)Div; }
    void SetSequenceLength(uint32_t ALen);
    void SetChannelSampleTime(uint32_t AChnl, AdcSampleTime_t ASampleTime);
    void SetSequenceItem(uint8_t SeqIndx, uint32_t AChnl);
    void StartConversion() { ADC1->CR2 |= ADC_CR2_SWSTART; }
public:
    Thread *PThreadToSignal;
    void InitHardware();
    void StartMeasurement();
    void StopAdc();
    uint32_t GetResult(uint8_t AChannel);
};

extern Measure_t Measure;
#endif

#endif /* CURRENT_H_ */
