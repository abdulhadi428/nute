/*
 * keyboard.h
 *
 *  Created on: 09.11.2011
 *      Author: Stefan
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <inttypes.h>

#define KEY_DELAY       255 // задержка дл€ защиты от дребезга. ћакс значение 255.

/* ROW - используютс€ дл€ опроса */
#define ROW_1_PORT             GPIOC
#define ROW_1_CLK              RCC_APB2Periph_GPIOC
#define ROW_1_Pin              GPIO_Pin_3
#define ROW_1_ON()             ROW_1_PORT->BSRR = ROW_1_Pin
#define ROW_1_OFF()            ROW_1_PORT->BRR  = ROW_1_Pin

#define ROW_2_PORT             GPIOC
#define ROW_2_CLK              RCC_APB2Periph_GPIOC
#define ROW_2_Pin              GPIO_Pin_4
#define ROW_2_ON()             ROW_2_PORT->BSRR = ROW_2_Pin
#define ROW_2_OFF()            ROW_2_PORT->BRR  = ROW_2_Pin

#define ROW_3_PORT             GPIOC
#define ROW_3_CLK              RCC_APB2Periph_GPIOC
#define ROW_3_Pin              GPIO_Pin_5
#define ROW_3_ON()             ROW_3_PORT->BSRR = ROW_3_Pin
#define ROW_3_OFF()            ROW_3_PORT->BRR  = ROW_3_Pin

/* COL - чтение данных */
#define COL_1_PORT             GPIOC
#define COL_1_CLK              RCC_APB2Periph_GPIOC
#define COL_1_Pin              GPIO_Pin_13
#define COL_1_PIN_IS_PRESSED() (GPIO_ReadInputDataBit(COL_1_PORT, COL_1_Pin))

#define COL_2_PORT             GPIOB
#define COL_2_CLK              RCC_APB2Periph_GPIOB
#define COL_2_Pin              GPIO_Pin_6
#define COL_2_PIN_IS_PRESSED() (GPIO_ReadInputDataBit(COL_2_PORT, COL_2_Pin))

#define COL_3_PORT             GPIOB
#define COL_3_CLK              RCC_APB2Periph_GPIOB
#define COL_3_Pin              GPIO_Pin_7
#define COL_3_PIN_IS_PRESSED() (GPIO_ReadInputDataBit(COL_3_PORT, COL_3_Pin))

#define COL_4_PORT             GPIOC
#define COL_4_CLK              RCC_APB2Periph_GPIOC
#define COL_4_Pin              GPIO_Pin_1
#define COL_4_PIN_IS_PRESSED() (GPIO_ReadInputDataBit(COL_4_PORT, COL_4_Pin))



class Keyboard_t {
private:
  uint8_t mTemp [4][5];
  uint8_t mState [4][5];
  void KeyStateChange(uint8_t chRow, uint8_t chCol, bool bValue);
  void PinStateLow (uint8_t chRow, uint8_t chCol);
  void PinStateHigh (uint8_t chRow, uint8_t chCol);
public:
  void Init(void);
  void Task(void);
};

void EVENT_KeyPressed(uint8_t chRow,uint8_t chCol);

extern Keyboard_t EKeyboard;

#endif /* KEYBOARD_H_ */
