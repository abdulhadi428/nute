/*
 * mdm.h
 *
 *  Created on: 12.10.2011
 *      Author: Kreyl
 *      This is class for GSM modem QUECTEL M10
 */

#ifndef MDM_H_
#define MDM_H_

#include "arm_common.h"
#include "misc.h"
#include "stm32f10x_usart.h"


// PA0=CTS, PA1=RTS, PA2=TX, PA3=RX, PA4=PWR_KEY

#define MDM_BUF_SIZE    250
#define MDM_RX_TIMEOUT  450 // ms

class mdm_t {
private:
    //uint8_t Buf[MDM_BUF_SIZE];
    uint32_t RxCounter;
    // Init
    void GPIOInit(void);
    void USARTInit(void);
    // Pins operations
    void PwrKeyHi(void) { GPIOA->BSRR = GPIO_Pin_4; }
    void PwrKeyLo(void) { GPIOA->BRR  = GPIO_Pin_4; }
    // UART operations
    void SendString(const char *S);
    Error_t ReceiveByte(uint8_t *AByte);
    Error_t SendAndWaitOk(const char *S);
    void DisableRxIrq(void) { USART_ITConfig(USART2, USART_IT_RXNE, DISABLE); }
    void EnableRxIrq (void) { USART_ClearFlag(USART2, USART_IT_RXNE); USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); }
public:
    void Init(void);
    void EnterPowersave(void);
    //void PowerDown(void) { SendString("AT+CFUN=0"); } // Enter minimum functionality
    //void PowerUp(void)   { SendString("AT+CFUN=1"); } // Enter full functionality
    void SendSMSWithTime(const char* AStrNumber, const char *AMsg);
    void IRQHandler(void);
};

extern mdm_t Mdm;

// USART RX IRQ
extern "C" {
void USART2_IRQHandler(void);
}

#endif /* MDM_H_ */
