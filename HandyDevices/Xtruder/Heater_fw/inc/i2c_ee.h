/*
 * File:   i2c_ee.h
 * Author: Kreyl
 *
 * Created on Sep 1, 2012, 11:17 AM
 */

#ifndef ACC_MMA_H
#define	ACC_MMA_H

#include "i2c_mgr.h"

class ee_t {
private:
public:
    void Init(void) {}
    //void Task(void);
    uint8_t Read(uint16_t AAddr, uint8_t *PBuffer, uint32_t ABufSize);
    //uint8_t Write(uint32_t AAddr, uint32_t ACount, uint8_t *PArr);
};

extern ee_t ee;


#endif

