/*
 * DBG_Unit.cpp
 *
 *  Created on: 25.11.2012
 *      Author: Stefan
 */

#include "DBG_Unit.h"
/*
void DBG_Message_Class :: Init(void)
{

}
void DBG_Message_Class :: Task(void)
{

}
void DBG_Message_Class :: SendData(void)
{

}
*/
void DBG_Message_Class :: PrintF(const char *fmt, ...)
{
	if (DbgEnableFlag==0) return;
	  char bp[PRINTF_BUF_SIZE]; // строка, хранит текст сообщения
	  int count;
	  va_list args;
	  va_start(args, fmt);
	  tiny_vsprintf(bp, fmt, args);
	  va_end(args);
	  count = strlen(bp);
	  DbgUART.SendDataBuf(count,(uint8_t*) bp);
}
