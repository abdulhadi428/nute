/*
 * RS485Class.h
 *
 *  Created on: 25.11.2012
 *      Author: Stefan
 */

#ifndef RS485CLASS_H_
#define RS485CLASS_H_

#include <stdint.h>
#include "DBG_Unit.h"
#include "UARTClass.h"
#include "GPIO_config.h"


#define    RS485_UART_OFF     			1
#define    RS485_WAITE_UART_READY 		2
#define    RS485_UART_READY_TO_TX      	3
#define    RS485_WAITE_TX_END 			4







class RS485_Class {
private:
	uint8_t ch485State;
	uint16_t iDataSizeToSend;
	uint8_t* pchDataBuf;
	uint8_t chSendReq;
	uint8_t chGetBusReq;
	uint8_t chFreeBusReq;
public:
    uint16_t        i;
    void Init(void);
    void Task(void);
    void SendData (uint16_t iDataSize,uint8_t* pchBuf );
    uint8_t ReadByte(uint8_t* pData);
    void GetBus(void);
    void FreeBus(void);
    uint8_t BusIsReady(void );
};


extern DBG_Message_Class DbgMessage;
extern UART_Class DbgUART;

#endif /* RS485CLASS_H_ */
