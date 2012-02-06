/*
 * main.h
 *
 *  Created on: 28.01.2012
 *      Author: kreyl
 */

#ifndef MAIN_H_
#define MAIN_H_

void GeneralInit(void);
void KLUartInit(void);


// IRQs
extern "C" {
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
}

#endif /* MAIN_H_ */
