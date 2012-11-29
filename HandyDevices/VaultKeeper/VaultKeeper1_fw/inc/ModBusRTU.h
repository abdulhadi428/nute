/*
 * ModBusRTU.h
 *
 *  Created on: 25.11.2012
 *      Author: Stefan
 */

#ifndef MODBUSRTU_H_
#define MODBUSRTU_H_
#include <stdint.h>
#include "RS485Class.h"

#define MODBUS_DATA_BUF_SIZE 	250
#define MODBUS_CRC16_INIT		0x00

#define MODBUS_RX_IDLE				0
#define MODBUS_RX_WAITE_NEXT_BYTE	1
#define MODBUS_RX_END				2
#define MODBUS_TX_IDLE				3
#define MODBUS_TX_GET_BUS			4
#define MODBUS_TX_END_TIMEOUT		5
#define MODBUS_WAIT_TX_END			6

struct ModBusPacket_t {
	uint8_t chAddr;
	uint8_t chFunc;
	uint8_t chDataSize;
	uint8_t chDataBuf[MODBUS_DATA_BUF_SIZE];
	uint16_t iCrc16;
};

void ModBusCrc16Calc(ModBusPacket_t* pModBusPacket);
bool ModBusCrc16Check(ModBusPacket_t* pModBusPacket);

class ModBusRTU_Class {
private:
	//ModBusPacket_t TxPacket;
	//ModBusPacket_t RxPacket;
	uint8_t chTxBuf[254];
	uint8_t chRxBuf[254];
	uint8_t chTxBufSize;
	uint8_t chRxBufSize;
	uint8_t chModBusRxState;
	uint8_t chModBusTxState;
	RS485_Class Rs485;
	uint8_t chSendReq;
public:
    uint16_t        i;
    void Init(void);
    void Task(void);
    void SendData (void);
    void GetData (void);
    void SendComand(ModBusPacket_t* pModBusPacket);
};

#endif /* MODBUSRTU_H_ */
