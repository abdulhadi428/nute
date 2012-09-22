/*
 * File:   i2c_ee.h
 * Author: Kreyl
 *
 * Created on Sep 1, 2012, 11:17 AM
 */

#ifndef ACC_MMA_H
#define	ACC_MMA_H

#include "i2c_mgr.h"

/*
 * Beware! Byte write time is about 5ms.
 * Therefore, device is unavailable right after Write Cmd.
 */

#define BUSY_TIMEOUT    400 // ms; after this switch to Failure state
#define SEARCH_DELAY    999 // ms; search every this delay
#define DATA_DELAY      5   // ms
#define PILL_ADDR       0b1010000
#define INNER_EE_ADDR   0b1010111   // Inner EEPROM addr is 0b1010111

enum eeState_t {esFailure, esNew, esBusy, esReady};

class ee_t {
private:
	I2C_Cmd_t IDataCmd;
public:
	eeState_t GetState(void) {return (IDataCmd.State == CmdSucceded)? esReady : esFailure; }
	void Init();
	void Read(uint16_t MemoryAddr, uint8_t *PBuf, uint32_t ACount);
	void Write(uint16_t MemoryAddr, uint8_t *PBuf, uint32_t ACount);
};

class Pill_t {
private:
    I2C_Cmd_t IDataCmd, ISearchCmd;
    uint32_t ITmr;
    bool DataXch;
    uint8_t IDevAddr;
public:
    eeState_t State;
    void Init(uint8_t DevAddr);
    void Task();
    void Read(uint16_t MemoryAddr, uint8_t *PBuf, uint32_t ACount);
    void Write(uint16_t MemoryAddr, uint8_t *PBuf, uint32_t ACount);
};


extern Pill_t Pill;


#endif

