/*
 * sensors.cpp
 *
 *  Created on: 22.06.2012
 *      Author: kreyl
 */

#include "sensors.h"
#include "kl_time.h"

SnsDataBuf_t SnsBuf;
Sensors_t Sensors;

void Sensors_t::Init() {
    NewLeakageOccured = false;
    ILeakagesOccured = false;
}

void Sensors_t::Task() {
    static uint32_t FTmr;
    static uint8_t FLastHour = 27; // dummy for first time
    if (Delay.Elapsed(&FTmr, SNS_CHECK_TIMEOUT)) {
        if (ILeakagesOccured) { // Update
            ILeakagesOccured = false;
            WriteMeasurements();
            NewLeakageOccured = true;
        }
        else if (Time.GetHour() != FLastHour) {
            FLastHour = Time.GetHour();
            WriteMeasurements();
        } // if hour passed
    } // if check timeout
}

void Sensors_t::WriteMeasurements() {
    // Update time, other values are updated online
    Time.GetDateTime(&CurrentData.DateTime);
    SnsBuf.Write(&CurrentData);
}

bool Sensors_t::LeakagesOccured(void) {

}

// ================================= Water =====================================
void Sensors_t::UpdateWaterValue(void) {
    uint32_t V = ReadWaterValue();
    V++;
    WriteWaterValue(V);
    CurrentData.WaterValue = V;
}

uint32_t Sensors_t::ReadWaterValue(void) {
    uint32_t Lo = BKP_ReadBackupRegister(BKPREG_WATER_LO);
    uint32_t Hi = BKP_ReadBackupRegister(BKPREG_WATER_HI);
    Hi <<= 16;
    return (uint32_t)(Hi + Lo);
}
void Sensors_t::WriteWaterValue(uint32_t AValue) {
    uint32_t Hi = AValue >> 16;
    uint32_t Lo = AValue & 0x0000FFFF;
    BKP_WriteBackupRegister(BKPREG_WATER_LO, (uint16_t)Lo);
    BKP_WriteBackupRegister(BKPREG_WATER_HI, (uint16_t)Hi);
}
