/*
 * ModBusRTU.h
 *
 *  Created on: 25.11.2012
 *      Author: Stefan
 */

#ifndef MODBUSRTU_H_
#define MODBUSRTU_H_
#include <stdint.h>

struct ModBusPacket_t {
	uint8_t Addr;
	uint8_t Addr;
	uint8_t Addr;

};

class ModBusRTU_Class {
private:

public:
    uint16_t        i;
    void Init(void);
    void Task(void);
    void SendData (void);
    void GetData (void);
};

#endif /* MODBUSRTU_H_ */
