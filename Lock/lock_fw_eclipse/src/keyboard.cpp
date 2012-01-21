/*
 * keyboard.cpp
 *
 *  Created on: 09.11.2011
 *      Author: Stefan
 */

#include "lock_action.h"
#include "delay_util.h"

#include "keyboard.h"

Keyboard_t EKeyboard;

void Keyboard_t::Init()
{
  RCC_APB2PeriphClockCmd(
      ROW_1_CLK |
      ROW_2_CLK |
      ROW_3_CLK |
      COL_1_CLK |
      COL_2_CLK |
      COL_3_CLK |
      COL_4_CLK |
      RCC_APB2Periph_AFIO,
      ENABLE );

  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure ROW_1_Pin as Output */
  GPIO_InitStructure.GPIO_Pin =  ROW_1_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init( ROW_1_PORT, &GPIO_InitStructure );
  ROW_1_ON();

  /* Configure ROW_2_Pin as Output */
  GPIO_InitStructure.GPIO_Pin =  ROW_2_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init( ROW_2_PORT, &GPIO_InitStructure );
  ROW_2_ON();

  /* Configure ROW_3_Pin as Output */
  GPIO_InitStructure.GPIO_Pin =  ROW_3_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init( ROW_3_PORT, &GPIO_InitStructure );
  ROW_3_ON();

  /* Configure COL_1_Pin as Input pull down*/
  GPIO_InitStructure.GPIO_Pin =  COL_1_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init( COL_1_PORT, &GPIO_InitStructure );

  /* Configure COL_2_Pin as Input pull down*/
  GPIO_InitStructure.GPIO_Pin =  COL_2_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init( COL_2_PORT, &GPIO_InitStructure );

  /* Configure COL_3_Pin as Input pull down*/
  GPIO_InitStructure.GPIO_Pin =  COL_3_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init( COL_3_PORT, &GPIO_InitStructure );

  /* Configure COL_4_Pin as Input pull down*/
  GPIO_InitStructure.GPIO_Pin =  COL_4_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init( COL_4_PORT, &GPIO_InitStructure );
}

void Keyboard_t::Task()
{
  ROW_1_OFF();
  if (COL_1_PIN_IS_PRESSED()) PinStateHigh(1,1); else PinStateLow(1,1);
  if (COL_2_PIN_IS_PRESSED()) PinStateHigh(1,2); else PinStateLow(1,2);
  if (COL_3_PIN_IS_PRESSED()) PinStateHigh(1,3); else PinStateLow(1,3);
  if (COL_4_PIN_IS_PRESSED()) PinStateHigh(1,4); else PinStateLow(1,4);
  ROW_1_ON();

  ROW_2_OFF();
  if (COL_1_PIN_IS_PRESSED()) PinStateHigh(2,1); else PinStateLow(2,1);
  if (COL_2_PIN_IS_PRESSED()) PinStateHigh(2,2); else PinStateLow(2,2);
  if (COL_3_PIN_IS_PRESSED()) PinStateHigh(2,3); else PinStateLow(2,3);
  if (COL_4_PIN_IS_PRESSED()) PinStateHigh(2,4); else PinStateLow(2,4);
  ROW_2_ON();

  ROW_3_OFF();
  if (COL_1_PIN_IS_PRESSED()) PinStateHigh(3,1); else PinStateLow(3,1);
  if (COL_2_PIN_IS_PRESSED()) PinStateHigh(3,2); else PinStateLow(3,2);
  if (COL_3_PIN_IS_PRESSED()) PinStateHigh(3,3); else PinStateLow(3,3);
  if (COL_4_PIN_IS_PRESSED()) PinStateHigh(3,4); else PinStateLow(3,4);
  ROW_3_ON();
}

void Keyboard_t::PinStateLow (uint8_t chRow, uint8_t chCol)
{
  if (mTemp[chRow][chCol]==KEY_DELAY ) return;
  mTemp[chRow][chCol]++;
  if (mTemp[chRow][chCol]==KEY_DELAY )
    {
      if (mState[chRow][chCol]==0) EVENT_KeyPressed(chRow,chCol);
      mState[chRow][chCol]=1;
    }
}

void Keyboard_t::PinStateHigh  (uint8_t chRow, uint8_t chCol)
{
  if (mTemp[chRow][chCol]==0 ) return;
  mTemp[chRow][chCol]--;
  if (mTemp[chRow][chCol]==0 )
    {
         // может понадобиться
        //if (mState[chRow][chCol]==1) EVENT_KeyReleased(chRow,chCol);
        mState[chRow][chCol]=0;
    }
}





