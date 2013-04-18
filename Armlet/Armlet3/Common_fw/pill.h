/*
 * pill.h
 *
 *  Created on: Apr 17, 2013
 *      Author: g.kruglov
 */

#ifndef PILL_H_
#define PILL_H_

#include "cmd_uart.h"
#include "ch.h"

#define EEADDR              0x50    // Standard address start of EEPROM - 0x01010aaa
#define PILL_START_ADDR     0       // Address of data, common for all
#define PILL_CNT            8       // Number of simultaneously connected pills

// I2C & hardware
#define PERIPH_PWR_GPIO     GPIOB
#define PERIPH_PWR_PIN      12
#define PILL_I2C_GPIO       GPIOB
#define PILL_SCL_PIN        10
#define PILL_SDA_PIN        11

#define PILL_I2C            I2C2
#define PILL_I2C_BITRATE_HZ 200000



class Pill_t {
private:
    uint8_t IAddr;
    BinarySemaphore ISem;
public:
    bool Connected;
    void Init(uint8_t IcAddr);
    uint8_t Read(uint8_t *Ptr, uint8_t Length);
    uint8_t Write(uint8_t *Ptr, uint8_t Length);
};

void PillInit();
void PillRegisterEvtChange(EventListener *PEvtLstnr, uint8_t EvtMask);

extern Pill_t Pill[PILL_CNT];


#endif /* PILL_H_ */
