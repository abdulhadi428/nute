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

#define SNS_COUNT   4

// Sensor states
#define LOWER_OK     0
#define LOWER_

enum SnsState_t {ssOk=0x00,
    ssLwrBreak=0x01, ssLwrShort=0x02, ssLwrWater=0x03, ssLwrFail=0x04,
    ssTopBreak=0x10, ssTopShort=0x20, ssTopWater=0x30, ssTopFail=0x40
};

struct RowData_t {
    DateTime_t DateTime;
    uint32_t WaterValue;
    SnsState_t SnsArr[SNS_COUNT];
    uint16_t Battery;   // == 0xFFFF in case of external power
} PACKED;
#define ROWDATA_SZ  sizeof(RowData_t)

#define SNS_BUF_SZ  48  // Two days without connection
class SnsDataBuf_t {
private:
    RowData_t IRow[SNS_BUF_SZ];
    uint8_t IReadIndx, IWriteIndx;
public:
    bool IsEmpty(void) {return (IReadIndx == IWriteIndx); }
    void Init(void) {IWriteIndx = 0; IReadIndx = 0; }
    RowData_t* Read(void) {
        RowData_t *PRow = &IRow[IReadIndx];
        IReadIndx++;
        if (IReadIndx >= SNS_BUF_SZ) IReadIndx = 0;
        return PRow;
    }
    void Write(RowData_t *PRow) {
        memcpy(&IRow[IWriteIndx], PRow, ROWDATA_SZ);
        IWriteIndx++;
        if (IWriteIndx >= SNS_BUF_SZ) IWriteIndx = 0;
    }
};

extern SnsDataBuf_t SnsBuf;

#endif /* SENSORS_H_ */
