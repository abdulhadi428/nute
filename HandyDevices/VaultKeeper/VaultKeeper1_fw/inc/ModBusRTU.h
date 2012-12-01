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
#include "kl_time.h"
#include "DBG_Unit.h"

/* ----- коды ошибок ------*/
#define MODBUS_ERROR_CODE_NO_ERROR  		0
#define MODBUS_ERROR_CODE_BUS_NOT_READY  	1 // UART не готов к передаче.
#define MODBUS_ERROR_CODE_TX_FAIL  			2 // ѕередача длитьс€ слишком долго. ¬озможны ошибки в UART-е
#define MODBUS_ERROR_CODE_RX_CRC_ERROR		3 // ќшибка в CRC16 прин€того пакета


#define MODBUS_DATA_BUF_SIZE 	250
#define MODBUS_CRC16_INIT		0x00
/*Ќемного об интервалах (речь идЄт о Serial Modbus RTU): при скорости 9600 и 11 битах
 в кадре (стартовый бит + 8 бит данных + бит контрол€ чЄтности + стоп-бит):
 3.5 * 11 / 9600 = 0,00401041(6), то есть более 4 мс;
 1.5 * 11 / 9600 = 0,00171875, то есть более 1 мс.*/

#define MODBUS_RTU_MIN_TIMEOUT		2  // 1.5 символа, 1мс. минимальна€ пауза между байтами, после которой начинаетс€ прием следующего пакета.
#define MODBUS_RTU_MAX_TIMEOUT		(MODBUS_RTU_MIN_TIMEOUT+3)  // 3.5 символа, 4мс. максимальна€ пауза после передачи, чтобы гарантировано зафиксировать конец передачи.

#define MODBUS_BUS_WAITE_TIMEOUT    	100 // врем€ ожидани€ освобождени€ UART-а
#define MODBUS_TX_END_WAITE_TIMEOUT		100 // врем€ ожидани€ конца передачи.

#define MODBUS_RX_IDLE				0
#define MODBUS_RX_WAITE_NEXT_BYTE	1
#define MODBUS_RX_END				2
#define MODBUS_TX_IDLE				3
#define MODBUS_TX_GET_BUS			4
#define MODBUS_TX_END_TIMEOUT		5
#define MODBUS_WAIT_TX_END			6




class ModBusRTU_Class {
private:
	uint8_t chTxBuf[254];
	uint8_t chRxBuf[254];
	uint8_t chTxBufSize;
	uint8_t chRxBufSize;
	uint8_t chModBusRxState;
	uint8_t chModBusTxState;
	RS485_Class Rs485;
	uint8_t chSendReq;
	uint32_t iRxTimer;
	uint32_t iTxTimer;
	void (*CallBackFunc)(uint8_t* pchBuf,uint16_t iDataSize,uint8_t chErrorCode);
	bool ModBusCrc16Check(uint8_t* pchBuf,uint16_t iDataSize);
public:
    uint16_t        i;
    void Init(void);
    void SetCallBackFunc(void (*pCallBackFunc)(uint8_t* pchBuf,uint16_t iDataSize,uint8_t chErrorCode));
    void Task(void);
    void SendData (void);
    void GetData (void);
    void SendComand(uint8_t chAddr,uint8_t chFunc,uint8_t* pchBuf,uint16_t iDataSize);
};

extern DBG_Message_Class DbgMessage;

extern "C" {
unsigned short crc16_tbl_buf(unsigned char * buf, unsigned short crc,unsigned long len);
}
#endif /* MODBUSRTU_H_ */
