/*
 * uart_cmd.h
 *
 *  Created on: 20.02.2012
 *      Author: kreyl
 */

#ifndef UART_CMD_H_
#define UART_CMD_H_

#include "stm32f10x_usart.h"
#include "kl_lib.h"

class CmdUnit_t {
private:
    uint32_t ITimer;
    klPwmChannel_t CoilA, CoilB;
    void WriteByte(uint8_t AByte) { USART2->DR = AByte; }
    bool ReadyToWrite(void) { return (USART2->SR & USART_FLAG_TXE); }
public:
    void Init(void);
    void Task(void);
};

extern CmdUnit_t CmdUnit;

#endif /* UART_CMD_H_ */
