/*
 * main.h
 *
 *  Created on: 28.01.2012
 *      Author: kreyl
 */

#ifndef MAIN_H_
#define MAIN_H_

#define BUF_SIZE    144
class UartBuf_t {
private:
    uint8_t IBuf[BUF_SIZE];
    uint8_t WCounter, RCounter;
public:
    void Init(void) { WCounter = 0; RCounter = 0; }
    void Add(uint8_t AByte);
    void Task(void);
};

// Prototypes
void GeneralInit(void);
void KLUartInit(void);

// IRQs
extern "C" {
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
}

#endif /* MAIN_H_ */
