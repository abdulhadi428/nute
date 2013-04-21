/*
 * application.h
 *
 *  Created on: 16.01.2013
 *      Author: kreyl
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "kl_lib_f2xx.h"

// Uart Command Codes. See https://docs.google.com/document/d/1pGQf9CrQ016ObS0w7PhPLAy92MRPhdBriICflt1YGXA/edit
#define CMD_PING            0x01
#define CMD_SET_GATE_NUM    0x10
#define CMD_RTX             0x11
#define CMD_PILL_STATE      0x30
#define CMD_PILL_WRITE      0x31
#define CMD_PILL_READ       0x32
#define CMD_PIN             0x40

#define RPL_ACK             0x90    // Acknowledge
#define RPL_RTX             0xA1    // TX completed
#define RPL_RRX             0xA4    // RX completed
#define RPL_PILL_READ       0xC2

// ================================= Pin control ===============================
#define PIN_GPIO    GPIOD
#define PIN_NUMBER  14
class Pin_t {
private:
    VirtualTimer ITmr;
public:
    void Init() { PinSetupOut(PIN_GPIO, PIN_NUMBER, omPushPull); }
    void High() { PinSet(PIN_GPIO, PIN_NUMBER); }
    void Low()  { PinClear(PIN_GPIO, PIN_NUMBER); }
    void Pulse(uint32_t ms);
};

extern Pin_t Pin;

void AppInit();


#endif /* APPLICATION_H_ */
