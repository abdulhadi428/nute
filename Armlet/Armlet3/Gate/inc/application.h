/*
 * application.h
 *
 *  Created on: 16.01.2013
 *      Author: kreyl
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "kl_lib_f2xx.h"

// Uart Command Codes
#define CMD_PING            0x01
#define CMD_SET_GATE_NUM    0x10
#define CMD_RTX             0x11
#define CMD_PILL_STATE      0x30
#define CMD_PILL_WRITE      0x31
#define CMD_PILL_READ       0x32
#define CMD_PIN             0x40

#define RPL_ACK             0x90
#define RPL_RTX             0xA1
#define RPL_RRX             0xA4
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

class App_t {
private:

public:
    void Init();
};

extern App_t App;

#endif /* APPLICATION_H_ */
