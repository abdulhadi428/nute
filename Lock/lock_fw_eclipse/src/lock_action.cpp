/*
 * lock_action.cpp
 *
 *  Created on: 06.11.2011
 *      Author: Stefan
 */
#include "lock_action.h"
#include "delay_util.h"
//#include "lcd110x.h"
#include "media.h"
//#include "sensors.h"
#include "kl_ini.h"
//#include "lca.h"

#include "ff.h"
#include "diskio.h"
//#include "uart.h"
#include <stdio.h>

Lock_t ELock;

void Lock_t::Init()
{
	chLockState=LOCK_NORMAL;
	chNormState=NORMAL_START;
	ReadPasswordFromSD();
}

void Lock_t::Task()
{
    switch (chLockState)
    {
	case LOCK_NORMAL:
	  NormalTask();
	  break;
	case LOCK_BREAKED:
	  //BreakedTask();
      break;
    case LOCK_HACKED:
      //HackedTask();
       break;
    case LOCK_RESTART:
      //RestartTask();
      break;
	}
}
/*
принимает код нажатой кнопки, возвращает 0-успешно 1- переполнение
 */
uint8_t Lock_t::KeyPressed(uint8_t chKey)
{
  if (bKeyFlag) return 1;
    else bKeyFlag=true;
  chNewKey=chKey;
  return 0;
}
/*
 * Автомат работы замка в нормальном режиме
 * принимаем нажатые кнопки открываем/закрываем замок
 */
void Lock_t::NormalTask()
{
  switch (chNormState)
  {
    case NORMAL_START:
      chKeyCount=0;
      chErrorCount=0;
      if (ESnd.State!=sndPlaying) chNormState=NORMAL_ON; //закончились все сообщения
      break;
    case NORMAL_ON: // исходное состояние, замок закрыт
      if (bKeyFlag) // была нажата кнопка
      {
          bKeyFlag=false;
          if (chNewKey==chPassword[chKeyCount]) // Правильная кнопка
            {
              ESnd.Play(SOUND_BEEP_PASS);
              chKeyCount++;
            }
          else                                  // неправильная кнопка
            {
              ESnd.Play(SOUND_BEEP_FAIL);
              //if (chKeyCount>1)  ESnd.Play(SOUND_CODE_RESET);
              chKeyCount=0;
              chErrorCount++;
            }
          if (chKeyCount==chPasswordLength)
            {
              ESnd.Play(SOUND_LOCK_OPEN);
              chNormState=NORMAL_OPEN;
              Open();
              Delay.Reset(&wOpenTimer);
            }
          if (chErrorCount==chMaxCodeError) // слишком много ошибок при наборе кода
            {
              chNormState=NORMAL_BLOCK;
              ESnd.Play(SOUND_LOCK_BLOCK_LONG);
            }
      }
      break;
    case NORMAL_OPEN:  // замок открыт некоторое время
      if (Delay.Elapsed(&wOpenTimer, LOCK_OPEN_TIME))
        {
          ESnd.Play(SOUND_LOCK_CLOSE);
          chNormState=NORMAL_CLOSE;
        }
      break;
    case NORMAL_CLOSE: // замок закрывается после предупреждения
      if (ESnd.State!=sndPlaying)
        {
          Close();
          chNormState=NORMAL_START;
        }
      break;
    case NORMAL_BLOCK:           // замок заблокирован
      if (ESnd.State==sndPlaying) bKeyFlag=false; // игонорим кнопки
      if (bKeyFlag) // была нажата кнопка
      {
          ESnd.Play(SOUND_LOCK_BLOCK_SHORT);
          bKeyFlag=false; // хотя, при нормальной работе аудио, это не нужно вовсе.
      }
      if (Delay.Elapsed(&wBlockTimer, LOCK_BLOCK_TIME)) // пришло время разблокировать замок
      {
          ESnd.Play(SOUND_LOCK_UNBOCK);
          chNormState=NORMAL_START;
      }
      break;

  }
}

void Lock_t::Close()
{
}
void Lock_t::Open()
{
}

/*****  UsbRecData *******************/
/* вызывается извне, заполняет внутренний буфер
   данными, полученными по USB */
/*************************************/
void Lock_t::UsbRecData(uint8_t* pData, uint8_t iSize)
{
  if (pData==NULL) return;
  uint8_t i=0;
  while (i<iSize)
    {
     if (chUsbRxCounter==USB_RX_BUF_SIZE) return; // чтобы буфер не переполнился
     chUsbRxBuf[chUsbRxCounter]=*pData;
     pData++;
     chUsbRxCounter++;
     i++;
    }
  return;
}



void Lock_t::ReadPasswordFromSD()
{
  chPassword[0]=0;
  chPassword[1]=0;
  chPassword[2]=0;
  chPassword[3]=0;
  //chPasswordLength=4;// но не более MAX_PASS_LEN
  chPasswordLength = ReadInt32("PASSWORD", "PASSLENGTH","lock.ini");
  if (chPasswordLength>MAX_PASS_LEN)  chPasswordLength=MAX_PASS_LEN;
  ReadString("PASSWORD", "PASS", chPassword, chPasswordLength , "lock.ini");

  chMaxCodeError=5;
}
