/*
 * RS485Class.cpp
 *
 *  Created on: 25.11.2012
 *      Author: Stefan
 */

#include "RS485Class.h"


void RS485_Class :: Init(void)
{
	ch485State=RS485_UART_OFF;
	 iDataSizeToSend=0;
	 pchDataBuf=NULL;
	 chSendReq=0;
	 chGetBusReq=0;
	 chFreeBusReq=0;
}
void RS485_Class :: Task(void)
{
	switch (ch485State)
	{
	case RS485_UART_OFF:
		if (chGetBusReq)
		{
			DbgMessage.DbgDisable();
			ch485State=RS485_WAITE_UART_READY;
			chGetBusReq=0;
		}
		break;
	case RS485_WAITE_UART_READY:
		if (DbgUART.TxCompleteFlag==1)ch485State=RS485_UART_READY_TO_TX;
		break;
	case RS485_UART_READY_TO_TX:
		if (chSendReq) // поступил запрос на передачу данных
		{
			Driver485TxEnable();// включаем 485 микросхему на передачу
			DbgUART.SendDataBuf(iDataSizeToSend,pchDataBuf);
			ch485State=RS485_WAITE_TX_END;
			chSendReq=0;
		}
		if (chFreeBusReq) // поступил запрос на отклчение 485
		{
			DbgMessage.DbgEnable(); //включаем дебаг обратно
			ch485State=RS485_UART_OFF;
			chFreeBusReq=0;
			chSendReq=0;
			chGetBusReq=0;
		}
		break;

	case RS485_WAITE_TX_END: // ждем, когда закончиться передача последнего байта из UART
		if (DbgUART.TxCompleteFlag==1)
		{
			Driver485TxDisable(); // драйвер 485 переводим в режим приема
			ch485State=RS485_UART_READY_TO_TX;
		}
		break;

	}
}

void RS485_Class :: SendData(uint16_t iDataSize,uint8_t* pchBuf )
{
	if (iDataSize==0) return;
	iDataSizeToSend=iDataSize;
	pchDataBuf=pchBuf;
	chSendReq=true;
}

uint8_t RS485_Class :: ReadByte(uint8_t* pData)
{
	return DbgUART.FIFO_RxData.ReadByte(pData);
}

void RS485_Class :: GetBus(void){chGetBusReq=true;}

void RS485_Class :: FreeBus(void){chFreeBusReq=1;}

uint8_t RS485_Class :: BusIsReady(void)
{
	if (ch485State==RS485_UART_READY_TO_TX) return 1;
	else return 0;
}
/*
uint8_t RS485_Class :: TxIsComplete(void)
{
	if (ch485State==RS485_UART_READY_TO_TX) return 1;
	else return 0;
}
*/

