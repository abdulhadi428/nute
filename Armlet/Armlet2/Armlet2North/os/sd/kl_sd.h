/*
 * sd.h
 *
 *  Created on: 13.02.2013
 *      Author: kreyl
 */

#ifndef SD_H_
#define SD_H_

#include "ff.h"
#include "ch.h"
#include "hal.h"

class sd_t {
private:
    FATFS SDC_FS;
    uint8_t fbuff[1024];
public:
    bool IsReady;
    void Init();
};

extern sd_t SD;

#endif /* SD_H_ */
