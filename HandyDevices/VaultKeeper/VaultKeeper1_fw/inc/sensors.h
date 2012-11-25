/*
 * sensors.h
 *
 *  Created on: 22.06.2012
 *      Author: kreyl
 */

#ifndef SENSORS_H_
#define SENSORS_H_

#include "kl_lib.h"
#include "string.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_adc.h"
//#include "UARTClass.h"
#include "DBG_Unit.h"

#define ADC_AVERAGE_COUNT   64      // Number of times to measure
#define PULLUP_DEADTIME     18      // ms; time between pullup on and measurement start
#define BKPREG_WATER_HI     BKP_DR5
#define BKPREG_WATER_LO     BKP_DR6
#define MIN_PULSE_LENGTH    252     // Min length of water pulse
#define NEW_PROBLEM_TIMEOUT 4005    // for channels
#define WRITE_MIN_DELAY     18000   // ms

// Sensor states
enum SnsState_t {ssOk=0x00,
    ssOpen=0x01, ssShort=0x02, ssWater=0x03,
    ssFail=0x04,
};
enum SnsChName_t {
    Sns1A, Sns1B, Sns2A, Sns2B, Sns3A, Sns3B,
    Sns4A, Sns4B, Sns5A, Sns5B, Sns6A, Sns6B,
    SnsBattery
};

// =================== Set here channels to measure ============================
struct SnsChnlParams_t {
    SnsChName_t Name;
    GPIO_TypeDef *PInPort;
    uint16_t InMask;
    GPIO_TypeDef *POutPort;
    uint16_t OutMask;
    uint8_t AdcChannel;
};

// Comment or uncomment needed lines
const SnsChnlParams_t SnsChnlParams[] = {
//        {Sns1A, GPIOA, GPIO_Pin_0, GPIOB, GPIO_Pin_2,  ADC_Channel_0},
//        {Sns1B, GPIOA, GPIO_Pin_1, GPIOB, GPIO_Pin_3,  ADC_Channel_1},
        {Sns2A, GPIOA, GPIO_Pin_4, GPIOB, GPIO_Pin_4,  ADC_Channel_4},
        {Sns2B, GPIOA, GPIO_Pin_5, GPIOB, GPIO_Pin_5,  ADC_Channel_5},
        {Sns3A, GPIOA, GPIO_Pin_6, GPIOB, GPIO_Pin_6,  ADC_Channel_6},
        {Sns3B, GPIOA, GPIO_Pin_7, GPIOB, GPIO_Pin_7,  ADC_Channel_7},
        {Sns4A, GPIOC, GPIO_Pin_0, GPIOB, GPIO_Pin_8,  ADC_Channel_10},
        {Sns4B, GPIOC, GPIO_Pin_1, GPIOB, GPIO_Pin_9,  ADC_Channel_11},
        {Sns5A, GPIOC, GPIO_Pin_2, GPIOB, GPIO_Pin_10, ADC_Channel_12},
        {Sns5B, GPIOC, GPIO_Pin_3, GPIOB, GPIO_Pin_11, ADC_Channel_13},
//        {Sns6A, GPIOC, GPIO_Pin_4, GPIOC, GPIO_Pin_6,  ADC_Channel_14},
//        {Sns6B, GPIOC, GPIO_Pin_5, GPIOC, GPIO_Pin_7,  ADC_Channel_15},
        {SnsBattery, GPIOB, GPIO_Pin_0, 0, 0,          ADC_Channel_8},
};
#define ADC_CH_COUNT    CountOf(SnsChnlParams)
#define SNS_COUNT       (ADC_CH_COUNT>>1)

// =============================================================================

struct RowData_t {
    DateTime_t DateTime;
    uint32_t WaterValue;
    uint8_t SnsArr[SNS_COUNT];
    uint16_t Battery;   // == 0 in case of external power
} PACKED;
#define ROWDATA_SZ  sizeof(RowData_t)

#define SNS_BUF_SZ  48  // Two days without connection
class SnsDataBuf_t {
private:
    RowData_t IRow[SNS_BUF_SZ];
    uint8_t IReadIndx, IWriteIndx;
public:
    void Init(void) { IWriteIndx = 0; IReadIndx = 0; }
    bool IsEmpty(void) { return (IReadIndx == IWriteIndx); }
    RowData_t* Read(void) { return &IRow[IReadIndx]; }
    void PrepareToReadNext(void) { if (++IReadIndx >= SNS_BUF_SZ) IReadIndx = 0; }
    void Write(RowData_t *PRow) {
        memcpy(&IRow[IWriteIndx], PRow, ROWDATA_SZ);
        IWriteIndx++;
        if (IWriteIndx >= SNS_BUF_SZ) IWriteIndx = 0;
    }
    uint32_t ReadCount(void) { return (IWriteIndx >=  IReadIndx)? (IWriteIndx - IReadIndx) : ((IWriteIndx + SNS_BUF_SZ) - IReadIndx); }
};

#define SHORT_ADC_VALUE     477
#define WATER_ADC_VALUE     2304
#define OPEN_ADC_VALUE      3600
struct SnsChnl_t {
private:
    SnsState_t StateLast;
public:
    uint32_t Timer;
    bool HasChanged;
    uint32_t Value;
    SnsState_t StateLongPerspective;
    void Init(void) { StateLongPerspective = ssOk; StateLast = ssOk; }
    void ProcessNewValue(uint32_t AValue);
};

class Sensors_t {
private:
    void WriteMeasurements(void);
    bool ISituationChanged;
    bool IsExtPwrOk(void) { return klGpioIsSetByN(GPIOB, 12); }
    // Adc
    uint16_t ADCValues[ADC_AVERAGE_COUNT];
    uint32_t IMeasureTmr;
    bool MeasureStarted;
    SnsChnl_t Chnl[ADC_CH_COUNT];
    void AdcInit(void);
    void AdcGpioInit(void);
    void IPrepareToNextMeasure(void);
    void StartNextMeasure(void);
    bool MeasureIsCompleted(void) { return DMA_GetFlagStatus(DMA1_FLAG_TC1); }
    // Water
    klPinIrq_t WaterPin;
    void WaterInit(void);
    uint32_t ReadWaterValue(void);
    void WriteWaterValue(uint32_t AValue);
    void UpdateWaterValue(void);
public:
    bool NeedToReport;
    void Init(void);
    void Task(void);
    void WaterIrqHandler(void);
};

extern SnsDataBuf_t SnsBuf;
extern Sensors_t Sensors;
//extern UART_Class DbgUART;
extern DBG_Message_Class DbgMessage;
// Water sensor IRQ
extern "C" {
void EXTI4_IRQHandler(void);
}

#endif /* SENSORS_H_ */
