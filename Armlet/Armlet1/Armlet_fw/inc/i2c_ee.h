/*
 * File:   i2c_ee.h
 * Author: Kreyl
 *
 * Created on Sep 1, 2012, 11:17 AM
 */

#ifndef ACC_MMA_H
#define	ACC_MMA_H

#include "i2c_mgr.h"

#define EE_BUFSIZE	99

/*
 * Beware! Byte write time is about 5ms.
 * Therefore, device is unavailable right after Write Cmd.
 */

class ee_t {
private:
	I2C_Cmd_t ICmd;
	uint8_t IBuf[EE_BUFSIZE+1];
public:
	uint8_t *Buf;
	void Init(void) {
		Buf = &IBuf[1];
		ICmd.DataToRead.Buf = Buf;
		ICmd.DataToWrite.Buf = IBuf;
	}
    uint8_t Read(uint16_t AAddr, uint32_t ACount);
    uint8_t Write(uint16_t AAddr, uint32_t ACount);
};

extern ee_t ee;


#endif

