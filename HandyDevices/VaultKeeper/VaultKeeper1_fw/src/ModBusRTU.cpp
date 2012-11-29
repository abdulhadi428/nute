/*
 * ModBusRTU.cpp
 *
 *  Created on: 25.11.2012
 *      Author: Stefan
 */
#include "ModBusRTU.h"
#include "crc_8_16_32.h"
#include <string.h>
void ModBusRTU_Class :: Init(void)
{
	Rs485.Init();
	chModBusRxState=MODBUS_RX_IDLE;
	chModBusTxState=MODBUS_TX_IDLE;
	chSendReq=0;
	chTxBufSize=0;

}
void ModBusRTU_Class :: Task(void)
{
	uint8_t chByte;
	switch (chModBusRxState)
	{
	case MODBUS_RX_IDLE:
		if (Rs485.ReadByte(&chByte)==1) // принят первый байт
		{
			chModBusRxState=MODBUS_RX_WAITE_NEXT_BYTE;
			chRxBuf[0]=chByte;
			chRxBufSize=1;
		}
		break;
	case MODBUS_RX_WAITE_NEXT_BYTE:
		// если привышен таймаут, то передача закончилась MODBUS_RX_END
		if (Rs485.ReadByte(&chByte)==1)
		{
			chRxBuf[chRxBufSize]=chByte;
			chRxBufSize++;
		}
			break;
	case MODBUS_RX_END:
		/* сравниваем CRC16, если сошлось - вызываем CALLBACK, если нет - то осбасываем пакет.
		 * можно добавить ошибку CRC ERROR */
			break;
	}

	switch (chModBusTxState)
	{
	case MODBUS_TX_IDLE:
		if (chSendReq==1)
		{
			Rs485.GetBus();
			chSendReq=0;
			chModBusTxState=MODBUS_TX_GET_BUS;
		}
		break;
	case MODBUS_TX_GET_BUS:
		if (Rs485.BusIsReady())// добавить выход по таймауту
		{
			Rs485.SendData(chTxBufSize,chTxBuf);
			chModBusTxState=MODBUS_WAIT_TX_END;
		}
		break;
	case MODBUS_WAIT_TX_END:
		if (Rs485.BusIsReady())// добавить выход по таймауту
		{
			chModBusTxState=MODBUS_TX_END_TIMEOUT;
		}
		break;
	case MODBUS_TX_END_TIMEOUT:// ожидаем таймаут после передачи
		chModBusTxState=MODBUS_TX_IDLE;
		Rs485.FreeBus();
		break;
	}
}
void ModBusRTU_Class :: SendComand(ModBusPacket_t* pModBusPacket)
{
	//memcpy(&TxPacket,pModBusPacket,sizeof (ModBusPacket_t)) ;
	ModBusCrc16Calc(pModBusPacket);
	chTxBuf[0]=pModBusPacket->chAddr;
	chTxBuf[1]=pModBusPacket->chFunc;
	memcpy((chTxBuf+2),pModBusPacket->chDataBuf,pModBusPacket->chDataSize); // копируем данные, chDataSize
	memcpy((chTxBuf+2+pModBusPacket->chDataSize),&(pModBusPacket->iCrc16),2); // копируем CRC16, два байта
	chTxBufSize=2+pModBusPacket->chDataSize+2;
	chSendReq=1;
}
void ModBusRTU_Class :: GetData(void)
{

}
void ModBusCrc16Calc(ModBusPacket_t* pModBusPacket)
{
	/* тут нужно подсчитать CRC16 и запонить это поле в пакете.*/
	pModBusPacket->iCrc16=crc16_tbl_buf(pModBusPacket->chDataBuf,MODBUS_CRC16_INIT,pModBusPacket->chDataSize);
}
bool ModBusCrc16Check(ModBusPacket_t* pModBusPacket)
{
	/* тут нужно проверить CRC16 */
	if (pModBusPacket->iCrc16==crc16_tbl_buf(pModBusPacket->chDataBuf,MODBUS_CRC16_INIT,pModBusPacket->chDataSize)) return true;
	return false;
}
