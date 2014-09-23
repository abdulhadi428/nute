/*
 * mpr121.cpp
 *
 *  Created on: 16 сент. 2014 г.
 *      Author: Kreyl
 */

#include "mpr121.h"
#include "cmd_uart.h"

#if 1 // ==== Constants ====
/* The status bits in Status Register 0x00 and 0x01 will only take place after
 * the number of consecutive touch or release detection meets the debounce number setting. */
//#define DEBOUNCE_REG_VALUE  0x11

template <uint8_t Sz>
struct Settings_t {
    uint8_t Addr;
    uint8_t Arr[Sz];
    uint8_t Write() const {
        for(uint8_t i=Addr; i<Addr+Sz; i++) Uart.Printf("\r%u", Arr[i]);
        return OK;
    }
} __attribute__((__packed__));

// ==== Filtering settings ====
const Settings_t<8> Filtering = {
        0x2B,
        0x01, 0x01, 0x00, 0x00, // Rising:  MHD, NHD, NCL, FDL
        0x01, 0x01, 0xFF, 0x02  // Falling: MHD, NHD, NCL, FDL
};

// ==== Touch / Release threshold ====
/* For very small electrodes, smaller values can be used and for very large electrodes
 * the reverse is true. One easy method is to view the deltas actually seen in a system
 * and set the touch at 80% and release at 70% of delta for good performance */
#define TOUCH_THRESHOLD     15
#define RELEASE_THRESHOLD   10
const Settings_t<(MPR_USED_SNS_CNT * 2)> Thresholds = {
        0x41,
        TOUCH_THRESHOLD, RELEASE_THRESHOLD,
        TOUCH_THRESHOLD, RELEASE_THRESHOLD,
        TOUCH_THRESHOLD, RELEASE_THRESHOLD,
        TOUCH_THRESHOLD, RELEASE_THRESHOLD,
        TOUCH_THRESHOLD, RELEASE_THRESHOLD,
};

// ==== Filter cfg ====
const Settings_t<1> FilterCfg = {
        0x5D,
        0x04
};

// ==== Electrode cfg ==== Write last!
const Settings_t<1> ElectrodeCfg = {
        0x5E,
        ELECTRODE_CFG
};

// ==== Auto-configuration ====
const Settings_t<5> AutoCfg = {
        0x7B,
        0x0B, // Reg0; AutoCfg en, AutoRecfg en, Baseline auto en. Do as AN3889 p.9 says and do not ask.
        0x00, // Reg1; Both CDTx and CDCx will be searched and set by autoconfiguration and/or autoreconfiguration; no failure IRQs.
        ((256 * (MPR_VDD_MV - 700))      / MPR_VDD_MV),         // USL; AN3889 p.7
        ((256 * (MPR_VDD_MV - 700) * 65) / (100 * MPR_VDD_MV)), // LowSideLimit
        ((256 * (MPR_VDD_MV - 700) * 90) / (100 * MPR_VDD_MV))  // TargetLevel; AN3889 p.8
};

// ==== Reset ====
const Settings_t<1> ResetReg = {
        0x80,
        0x63    // Value to write to reset reg to reset device
};

#endif

mpr121_t Touch;

void mpr121_t::Init() {
    Filtering.Write();
    Thresholds.Write();
    FilterCfg.Write();
    AutoCfg.Write();

    ElectrodeCfg.Write();
}
