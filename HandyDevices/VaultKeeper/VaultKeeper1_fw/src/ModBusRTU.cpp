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
	Delay.Reset(&iRxTimer);
	Delay.Reset(&iTxTimer);
}
void ModBusRTU_Class :: Task(void)
{
	Rs485.Task();
	uint8_t chByte;
	//uint16_t iCrc;
	switch (chModBusRxState)
	{
	case MODBUS_RX_IDLE:
		if (Rs485.ReadByte(&chByte)==1) // принят первый байт
		{
			chModBusRxState=MODBUS_RX_WAITE_NEXT_BYTE;
			chRxBuf[0]=chByte;
			chRxBufSize=1;
			Delay.Reset(&iRxTimer);
		}
		break;
	case MODBUS_RX_WAITE_NEXT_BYTE:
		if (Delay.Elapsed(&iRxTimer,MODBUS_RTU_MIN_TIMEOUT))// если привышен таймаут, то передача закончилась
		{
			chModBusRxState=MODBUS_RX_END;
		}

		if (Rs485.ReadByte(&chByte)==1)
		{
			chRxBuf[chRxBufSize]=chByte;
			chRxBufSize++;
			Delay.Reset(&iRxTimer);
		}
		break;
	case MODBUS_RX_END:
		if (chRxBufSize>=4) // длинна пакета не должна быть меньше 4-х байт
		{
		if (ModBusCrc16Check(chRxBuf,chRxBufSize)) // сравниваем CRC16
			{
				CallBackFunc(chRxBuf,chRxBufSize,MODBUS_ERROR_CODE_NO_ERROR);// если сошлось - вызываем CALLBACK
			}
			else
			{
				CallBackFunc(chRxBuf,chRxBufSize,MODBUS_ERROR_CODE_RX_CRC_ERROR); // можно добавить ошибку CRC ERROR
			}
		}
		chModBusRxState=MODBUS_RX_IDLE;
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
			Delay.Reset(&iTxTimer);
		}
		break;
	case MODBUS_TX_GET_BUS:
		if (Rs485.BusIsReady())// ожидаем освобождения UART-а для передачи.
		{
			Rs485.SendData(chTxBufSize,chTxBuf); // пакет ушел.
			chModBusTxState=MODBUS_WAIT_TX_END;
			Delay.Reset(&iTxTimer);
		}
		if (Delay.Elapsed(&iTxTimer,MODBUS_BUS_WAITE_TIMEOUT)) // слишком долго ждем UART-а
		{
			CallBackFunc(NULL,0,MODBUS_ERROR_CODE_BUS_NOT_READY); // UART не готов к передаче.
			chModBusTxState=MODBUS_TX_IDLE; // отменяем передачу
		}
		break;
	case MODBUS_WAIT_TX_END:
		if (Rs485.BusIsReady())// ждем когда закончиться текущая передача
		{
			chModBusTxState=MODBUS_TX_END_TIMEOUT;
			Delay.Reset(&iTxTimer);
		}
		if (Delay.Elapsed(&iTxTimer,MODBUS_TX_END_WAITE_TIMEOUT))
		{
			CallBackFunc(NULL,0,MODBUS_ERROR_CODE_TX_FAIL); //Передача длиться слишком долго. Возможны ошибки в UART-е
			chModBusTxState=MODBUS_TX_IDLE; // прекращаем передачу
		}
		break;
	case MODBUS_TX_END_TIMEOUT:// выжидаем таймаут после передачи, чтобы нода гарантированно зафиксировала конец пакета
		if (Delay.Elapsed(&iTxTimer,MODBUS_RTU_MAX_TIMEOUT))
		{
			chModBusTxState=MODBUS_TX_IDLE;
			Rs485.FreeBus();
		}
		break;
	}
}

bool ModBusRTU_Class :: ModBusCrc16Check(uint8_t* pchBuf,uint16_t iDataSize)
{
	uint16_t* piRxCrc;
	uint16_t iCalcCrc;
	piRxCrc=(uint16_t*)(pchBuf+(iDataSize-2)); // указатель будет указывать на последние два байта в принятом пакете, как на 16-и битный CRC
	iCalcCrc=crc16_tbl_buf(pchBuf,MODBUS_CRC16_INIT,(iDataSize-2));
	//DbgMessage.PrintF("\rRxCRC16=%d CalCRC16=%d \r", (*piRxCrc),iCalcCrc);
	if ((*piRxCrc)==iCalcCrc) return true;
	return false;
}

void ModBusRTU_Class :: SendComand(uint8_t chAddr,uint8_t chFunc,uint8_t* pchBuf,uint16_t iDataSize)
{
	//memcpy(&TxPacket,pModBusPacket,sizeof (ModBusPacket_t)) ;
	//ModBusCrc16Calc(pModBusPacket);
	uint16_t iCrc;
	chTxBuf[0]=chAddr;
	chTxBuf[1]=chFunc;
	chTxBufSize=2;
	memcpy((chTxBuf+chTxBufSize),pchBuf,iDataSize); // копируем отправляемые данные в буфер передачи
	chTxBufSize+=iDataSize;
	iCrc=crc16_tbl_buf(chTxBuf,MODBUS_CRC16_INIT,chTxBufSize);// считаем CRC16
	memcpy((chTxBuf+chTxBufSize),&iCrc,2); // копируем CRC16 в буфер передачи, два байта
	chTxBufSize+=2;
	chSendReq=1;
}
void ModBusRTU_Class :: GetData(void)
{

}

void ModBusRTU_Class ::SetCallBackFunc(void (*pCallBackFunc)(uint8_t* pchBuf,uint16_t iDataSize,uint8_t chErrorCode))
{
	CallBackFunc=pCallBackFunc;
}



