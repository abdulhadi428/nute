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
#include <string.h>
#include "misc.h"
#include "stm32f10x_usart.h"


// PA0=CTS, PA1=RTS, PA2=TX, PA3=RX, PA4=PWR_KEY

#define MDM_LINE_LEN    54
#define MDM_LINE_COUNT  4
#define MDM_RX_TIMEOUT  450 // ms

class mdm_t {
private:
    Error_t State;
    bool NewLineReceived;
    char Line[54];
    uint8_t CharCounter;
    Error_t Reply;
    // Init
    void GPIOInit(void);
    void USARTInit(void);
    // Pins operations
    void PwrKeyHi(void) { GPIOA->BSRR = GPIO_Pin_4; }
    void PwrKeyLo(void) { GPIOA->BRR  = GPIO_Pin_4; }
    // UART operations
    void SendString(char *S);
    Error_t SendRequest(const char *ARequest);
    Error_t SendAndWaitString(const char *ACmd, const char *AReply);
    void DisableRxIrq(void) { USART_ITConfig(USART2, USART_IT_RXNE, DISABLE); }
    void EnableRxIrq (void) { USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); }
    // Card, call, sms
    Error_t ProcessSIM(void);
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
