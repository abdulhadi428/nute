/*
 * application.h
 *
 *  Created on: 16.01.2013
 *      Author: kreyl
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

// Uart Command Codes
#define CMD_PING        0x01
#define CMD_RTX         0x11
#define CMD_PILL_WRITE  0x30
#define CMD_PILL_READ   0x31

#define RPL_ACK         0x90
#define RPL_RTX         0xA1
#define RPL_RRX         0xA4
#define RPL_PILL_READ   0xC1

class App_t {
private:

public:
    void Init();
};

extern App_t App;

#endif /* APPLICATION_H_ */
