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

#define ADC_AVERAGE_COUNT   64      // Number of times to measure
#define SNS_CHECK_TIMEOUT   1008    // ms; check sensors every second.
#define BKPREG_WATER_HI     BKP_DR5
#define BKPREG_WATER_LO     BKP_DR6

// Sensor states
enum SnsState_t {ssOk=0x00,
    ssOpen=0x01, ssShort=0x02, ssWater=0x03, ssFail=0x04,
};
enum SnsCh_t {
    Sns1A, Sns1B, Sns2A, Sns2B, Sns3A, Sns3B,
    Sns4A, Sns4B, Sns5A, Sns5B, Sns6A, Sns6B,
    SnsBattery
};

// =================== Set here channels to measure ============================
const SnsCh_t ChToMeasure[] = {
        Sns3A, Sns3B, Sns4A, Sns4B, Sns5A, Sns5B, Sns6A, Sns6B, SnsBattery
};
#define ADC_CH_COUNT    CountOf(ChToMeasure)
#define SNS_COUNT       4
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
};


class Sensors_t {
private:
    RowData_t CurrentData;
    void WriteMeasurements(void);
    bool IProblemsChanged;
    bool IsExtPwrOk(void) { return klGpioIsSetByN(GPIOB, 12); }
    // Adc
    uint8_t ChIndx;
    uint16_t ADCValues[ADC_AVERAGE_COUNT];
    void AdcInit(void);
    void StartNextMeasure(void);
    bool MeasureIsCompleted(void) { return DMA_GetFlagStatus(DMA1_FLAG_TC1); }
    // Water
    uint32_t ReadWaterValue(void);
    void WriteWaterValue(uint32_t AValue);
    void UpdateWaterValue(void);
public:
    bool NewProblemOccured;
    void Init(void);
    void Task(void);
};

extern SnsDataBuf_t SnsBuf;
extern Sensors_t Sensors;

#endif /* SENSORS_H_ */
