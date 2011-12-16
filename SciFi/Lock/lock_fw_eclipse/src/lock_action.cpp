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
#include "uart.h"

#include "ff.h"
#include "diskio.h"
//#include "uart.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Lock_t ELock;

void Lock_t::Init()
{
	chLockState=LOCK_NORMAL;
	chNormState=NORMAL_START;
	ReadSettingsFromSD();
}

void Lock_t::Task()
{
  if (bNewCommandFlag)  //пришла нова€ команда по UART-у
    {
      if (chLockState==LOCK_NORMAL) UART_PrintString("SERV MODE  \r");
      chLockState=LOCK_HACKED; // режим взлома
      chNormState=NORMAL_START; // сбрасываем автомат нормальной работы
      Delay.Reset(&wHackTimer);
    }

  switch (chLockState)
    {
      case LOCK_NORMAL:
	  NormalTask();
	  break;
      case LOCK_HACKED:
          if (Delay.Elapsed(&wHackTimer, HACK_DELAY_TIME)) // слишком долго небыло команд
          {
            chLockState=LOCK_NORMAL;
            UART_PrintString("NORMAL MODE  \r");
          }
          HackedTask();
          break;

      case LOCK_BREAKED:
          //BreakedTask();
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
 * јвтомат работы замка в нормальном режиме
 * принимаем нажатые кнопки открываем/закрываем замок
 */
void Lock_t::NormalTask()
{
  switch (chNormState)
  {
    case NORMAL_START:
      chKeyCount=0;
      chErrorCount=0;
      if (ESnd.State!=sndPlaying) chNormState=NORMAL_ON; //закончились все сообщени€
      break;
    case NORMAL_ON: // исходное состо€ние, замок закрыт
      if (ESnd.State==sndPlaying) bKeyFlag=false; // игонорим кнопки, пока проигрываетс€ звук
      if (bKeyFlag) // была нажата кнопка
      {
          bKeyFlag=false;
          if (chNewKey==chPassword[chKeyCount]) // ѕравильна€ кнопка
            {
              ESnd.Play(SOUND_BEEP_PASS);
              chKeyCount++;
              chErrorCount=0;
            }
          else                                  // неправильна€ кнопка
            {
              ESnd.Play(SOUND_BEEP_FAIL);
              chKeyCount=0;
              chErrorCount++;
            }
      }
      if ((chKeyCount==(chPasswordLength-1)) & (ESnd.State!=sndPlaying))
        {
          ESnd.Play(SOUND_LOCK_OPEN);
          chNormState=NORMAL_OPEN;
          Open();
          Delay.Reset(&wOpenTimer);
        }

      if ((chErrorCount>=chMaxCodeError) & (ESnd.State!=sndPlaying)) // слишком много ошибок при наборе кода
      {
          chNormState=NORMAL_BLOCK;
          ESnd.Play(SOUND_LOCK_BLOCK_LONG);
          Delay.Reset(&wBlockTimer);
      }

      break;
    case NORMAL_OPEN:  // замок открыт некоторое врем€
      if (ESnd.State==sndPlaying) break;
      if (Delay.Elapsed(&wOpenTimer, wTimeToOpen))
        {
          ESnd.Play(SOUND_LOCK_CLOSE);
          chNormState=NORMAL_CLOSE;
        }
      break;
    case NORMAL_CLOSE: // замок закрываетс€ после предупреждени€
      if (ESnd.State!=sndPlaying)
        {
          Close();
          chNormState=NORMAL_START;
        }
      break;
    case NORMAL_BLOCK:           // замок заблокирован
      if (ESnd.State==sndPlaying)
        {
          bKeyFlag=false; // игонорим кнопки
          break;    // не очень красиво, но работает.
        }
      if (bKeyFlag) // была нажата кнопка
      {
          ESnd.Play(SOUND_LOCK_BLOCK_SHORT);
          bKeyFlag=false; // хот€, при нормальной работе аудио, это не нужно вовсе.
      }
      if (Delay.Elapsed(&wBlockTimer, wTimeToBlock)) // пришло врем€ разблокировать замок
      {
          ESnd.Play(SOUND_LOCK_UNBOCK);
          chNormState=NORMAL_START;
      }
      break;

  }
}

void Lock_t::Close()
{
  UART_PrintString("Lock Close \r");
}
void Lock_t::Open()
{
  UART_PrintString("Lock Open \r");
}

/*************** UsartRxEvent *************************
 * Ёта функци€ вызываетс€ в прерывании UART-а
 * и принимает байт из UART-а
 * ¬Ќ»ћјЌ»≈, в ней нельз€ вызывать UART_Print
 *****************************************************/
void Lock_t::UsartRxEvent(uint8_t chByte)
{
  //UART_Print(chByte);

  *(strRxBuf+iRxCounter)=chByte;
  if ((chByte=='\r') | (chByte=='\n') | (chByte=='\0')) // конец строки
    {
      *(strRxBuf+iRxCounter+1)=0;
      ParsCommand();                      // можно парсить
      iRxCounter=0;
    }
  else
    {
        iRxCounter++;
        if (iRxCounter==(MAX_HACK_RX_BUF_SIZE-1)) iRxCounter=0;  // чтоб не переполн€лось
    }
}


void Lock_t::ParsCommand(void)
{
  char *pchPrefix, *pchPostfix, *pch;
  pchPrefix = strstr (strRxBuf,HACK_COMMAND_PREFIX);
  pchPostfix = strstr (strRxBuf,HACK_COMMAND_POSTFIX);
  if (pchPrefix==NULL) return;
  if (pchPostfix==NULL) return;
  uint8_t iCommandLen=pchPostfix-pchPrefix;
  if (iCommandLen>0)
    {
      strncpy(strCommand,pchPrefix+HACK_COMMAND_PREFIX_LEN,iCommandLen);
      strCommand[iCommandLen]='\0';
      bNewCommandFlag=true;
      iNewCommandCode=HACK_COMMAND_CODE_BLANK;
      //UART_PrintString(strCommand);
      pch=strstr(strCommand,HACK_COMMAND_TEST);
      //UART_PrintString(pch);
      if (pch==strCommand) iNewCommandCode=HACK_COMMAND_CODE_TEST;
    }
}

/*
 * јвтомат работы замка в режиме взлома
 * принимаем команды, выполн€ем действи€
 */
void Lock_t::HackedTask()
{
  if (bNewCommandFlag==false) return; // нет новых команд
  bNewCommandFlag=false;

  switch (iNewCommandCode)
    {
      case HACK_COMMAND_CODE_TEST:
        UART_PrintString("get HACK_COMMAND_TEST \r");
        break;
    }
  iNewCommandCode=HACK_COMMAND_CODE_BLANK;
 }

/*****  UsbRecData *******************/
/* вызываетс€ извне, заполн€ет внутренний буфер
   данными, полученными по USB */
/*************************************/
//void Lock_t::UsbRecData(uint8_t* pData, uint8_t iSize)
//{
//  if (pData==NULL) return;
//  uint8_t i=0;
//  while (i<iSize)
//    {
//     if (chUsbRxCounter==USB_RX_BUF_SIZE) return; // чтобы буфер не переполнилс€
//     chUsbRxBuf[chUsbRxCounter]=*pData;
//     pData++;
//     chUsbRxCounter++;
//     i++;
//    }
//  return;
//}



void Lock_t::ReadSettingsFromSD()
{
 // получаем длинну парол€
  if (ParamIsExists("PASSWORD", "PASSLENGTH","lock.ini"))
    {
      chPasswordLength = ReadInt32("PASSWORD", "PASSLENGTH","lock.ini");
      if (chPasswordLength>MAX_PASS_LEN)  chPasswordLength=MAX_PASS_LEN;
    }
  else chPasswordLength=4;// не более MAX_PASS_LEN

// получаем пароль
  if (ParamIsExists("PASSWORD", "PASS", "lock.ini"))
    {
      ReadString("PASSWORD", "PASS", chPassword, chPasswordLength , "lock.ini");
    }
  else
    {
      uint16_t i=0;
      while (i<MAX_PASS_LEN)
        {
          chPassword[i]='0'; // по умолчанию 4 нул€
          i++;
        }
    }
   chPassword[chPasswordLength]=0; // конец строки

   // получаем максимальное допустимое количество ошибок
   if (ParamIsExists("SECURITY", "MAXERROR", "lock.ini"))
     {
       chMaxCodeError = ReadInt32("SECURITY", "MAXERROR","lock.ini");
     }
   else  chMaxCodeError=5; // по умолчанию

   // получаем врем€, которое замок будет открыт
   if (ParamIsExists("SECURITY", "OPEN_TIME", "lock.ini"))
     {
       wTimeToOpen = ReadInt32("SECURITY", "OPEN_TIME","lock.ini");
     }
   else  wTimeToOpen=5000; // по умолчанию
   // получаем врем€, на которое замок будет блокироватьс€
   if (ParamIsExists("SECURITY", "BLOCK_TIME", "lock.ini"))
     {
       wTimeToBlock = ReadInt32("SECURITY", "BLOCK_TIME","lock.ini");
     }
   else  wTimeToBlock=20000; // по умолчанию 10000=примерно 15 сек
}
