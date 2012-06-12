/*
 * comline.h
 *
 *  Created on: 02.06.2012
 *      Author: kreyl
 */

#ifndef COMLINE_H_
#define COMLINE_H_

#include "stm32f10x_usart.h"

class ComLine_t {
private:
    void SendChar(char c) { USART2->DR = c; while(!(USART2->SR & USART_FLAG_TC)); }
    void IPrintUint(uint32_t ANumber);
    void IPrintInt (int32_t ANumber);
    void IPrintString (const char *S) { while (*S != '\0') SendChar(*S++); }
    void IPrintAsHex(uint32_t ANumber);
    void IPrint8HexArray(uint8_t *Arr, uint32_t ALength);
public:
    void Init(void);
    void Printf(const char *S, ...);
};

extern ComLine_t Com;

#endif /* COMLINE_H_ */
