/*
 * mpr121.cpp
 *
 *  Created on: 16 сент. 2014 г.
 *      Author: Kreyl
 */

#include "mpr121.h"

#if 1 // ==== Constants ====
// ==== Filtering settings ====
struct FilteringSettingsRisingFalling_t {
    uint8_t MHD;
    uint8_t NHD_Amount;
    uint8_t NCL;
    uint8_t FDL;
} __attribute__((__packed__));

struct FilteringSettingsTouched_t {
    uint8_t NHD_Amount;
    uint8_t NCL;
    uint8_t FDL;
} __attribute__((__packed__));

struct FilteringControl_t {
    FilteringSettingsRisingFalling_t Rising;
    FilteringSettingsRisingFalling_t Falling;
    FilteringSettingsTouched_t Touched;
} __attribute__((__packed__));
#define MPR_FILTERING_CTRL_SZ   sizeof(FilteringControl_t)

const FilteringControl_t FilteringControlCommon = {
        {0x01, 0x01, 0x00, 0x00},  // Rising
        {0x01, 0x01, 0xFF, 0x02},  // Falling
        {1,2,3,4},  // Touched
};

const FilteringControl_t FilteringControlEleprox = {
        {1,2,3,4},  // Rising
        {1,2,3,4},  // Falling
        {1,2,3,4},  // Touched
};

// ==== Touch / Release threshold ====
struct Threshold_t {
    uint8_t Touch;
    uint8_t Release;
} __attribute__((__packed__));

/* For very small electrodes, smaller values can be used and for very large electrodes
 * the reverse is true. One easy method is to view the deltas actually seen in a system
 * and set the touch at 80% and release at 70% of delta for good performance */
#define TOUCH_THRESHOLD     15
#define RELEASE_THRESHOLD   10
const Threshold_t Tresholds[MPR_TOTAL_SNS_CNT] = {
        {TOUCH_THRESHOLD, RELEASE_THRESHOLD},   // 0
        {TOUCH_THRESHOLD, RELEASE_THRESHOLD},   // 1
        {TOUCH_THRESHOLD, RELEASE_THRESHOLD},   // 2
        {TOUCH_THRESHOLD, RELEASE_THRESHOLD},   // 3
        {TOUCH_THRESHOLD, RELEASE_THRESHOLD},   // 4
        {TOUCH_THRESHOLD, RELEASE_THRESHOLD},   // 5
        {TOUCH_THRESHOLD, RELEASE_THRESHOLD},   // 6
        {TOUCH_THRESHOLD, RELEASE_THRESHOLD},   // 7
        {TOUCH_THRESHOLD, RELEASE_THRESHOLD},   // 8
        {TOUCH_THRESHOLD, RELEASE_THRESHOLD},   // 9
        {TOUCH_THRESHOLD, RELEASE_THRESHOLD},   // 10
        {TOUCH_THRESHOLD, RELEASE_THRESHOLD},   // 11
        {TOUCH_THRESHOLD, RELEASE_THRESHOLD},   // 12 ELEPROX
};

/* The status bits in Status Register 0x00 and 0x01 will only take place after
 * the number of consecutive touch or release detection meets the debounce number setting. */
#define DEBOUNCE_REG_VALUE  0x11

// Filter and global CDC CDT cfg
#define CDC_CFG_VALUE       0x00    // 0x5C
#define CDT_CFG_VALUE       0x04

// Electrode current
#define ELECTRODE_CURRENT   0   // 0 means that global charge current settings is used
const uint8_t ChargeCurrent[MPR_TOTAL_SNS_CNT] = {
        ELECTRODE_CURRENT,
        ELECTRODE_CURRENT,
        ELECTRODE_CURRENT,
        ELECTRODE_CURRENT,
        ELECTRODE_CURRENT,
        ELECTRODE_CURRENT,
        ELECTRODE_CURRENT,
        ELECTRODE_CURRENT,
        ELECTRODE_CURRENT,
        ELECTRODE_CURRENT,
        ELECTRODE_CURRENT,
        ELECTRODE_CURRENT,
        ELECTRODE_CURRENT,
};

// Charge time
#define CHARGE_TIME         0 // 0 means that global charge time is used
const uint8_t ChargeTime[7] = {
        CHARGE_TIME,
        CHARGE_TIME,
        CHARGE_TIME,
        CHARGE_TIME,
        CHARGE_TIME,
        CHARGE_TIME,
        CHARGE_TIME,
};

// Auto-configuration
struct AutoCfgCtrl_t {
    uint8_t Reg0;
    uint8_t Reg1;
    uint8_t UpSideLimit;
    uint8_t LowSideLimit;
    uint8_t TargetLevel;
} __attribute__((__packed__));

const AutoCfgCtrl_t AutoCfg = {
        0x0B, // Reg0; AutoCfg en, AutoRecfg en, Baseline auto en. Do as AN3889 p.9 says and do not ask.
        0x00, // Reg1; Both CDTx and CDCx will be searched and set by autoconfiguration and/or autoreconfiguration; no failure IRQs.
        ((256 * (MPR_VDD_MV - 700))        / MPR_VDD_MV), // USL; AN3889 p.7
        ((256 * (MPR_VDD_MV - 700) * 0.65) / MPR_VDD_MV), // LowSideLimit
        ((256 * (MPR_VDD_MV - 700) * 0.90) / MPR_VDD_MV)  // TargetLevel; AN3889 p.8
};
#endif

void mpr121_t::Init() {
    for(uint8_t i=MPR_REG_BASELINE_FILTERING_CTRL; i<(MPR_REG_BASELINE_FILTERING_CTRL + MPR_FILTERING_CTRL_SZ); i++) {
        Write(i, *((uint8_t*)&FilteringControlCommon));
    }
}
