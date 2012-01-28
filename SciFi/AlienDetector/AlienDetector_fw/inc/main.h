/*
 * main.h
 *
 *  Created on: 28.01.2012
 *      Author: kreyl
 */

#ifndef MAIN_H_
#define MAIN_H_

#define NOSIGNAL_DELAY  999

class Signal_t {
private:
    uint32_t ITimer[8];
    bool IFlag[8];
public:
    void Init(void);
    void Task(void);
    void Display(uint8_t AChannel, uint8_t RSSI);
};

extern Signal_t Signal;

void EVENT_NewPacket(void);
void EVENT_NewBatteryState(void);
void GeneralInit(void);

#endif /* MAIN_H_ */
