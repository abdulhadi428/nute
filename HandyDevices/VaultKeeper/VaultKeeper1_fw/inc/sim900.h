/*
 * sim900.h
 *
 *  Created on: 01.06.2012
 *      Author: kreyl
 */

#ifndef SIM900_H_
#define SIM900_H_

#include <stdint.h>
#include "kl_lib.h"
#include "stm32f10x_usart.h"

#define MDM_LINE_LEN        54
#define MDM_RX_TIMEOUT      450   // ms
#define MDM_SIM_TIMEOUT     4005  // ms
#define MDM_NETREG_TIMEOUT  20007 // ms
#define MDM_SMS_TIMEOUT     18000 // ms

enum NetState_t {nsNotRegNoSearch=0, nsRegistered=1, nsSearching=2, nsRegDenied=3, nsUnknown=4, nsRegRoaming=5};


class sim900_t {
private:
    bool NewLineReceived;
    Error_t RplKind;
    NetState_t NetState;
    // Strings
    char TxLine[MDM_LINE_LEN];  // Command
    char RxLine[MDM_LINE_LEN];  // Received line
    uint8_t RxCounter;
    void ResetRxLine(void) { RxCounter = 0; NewLineReceived = false; }
    // Init
    void GPIOInit(void);
    void USARTInit(void);
    // Pins operations
    void PwrKeyHi(void) { klGpioSetByMsk(GPIOA, GPIO_Pin_15); }
    void PwrKeyLo(void) { klGpioClearByMsk(GPIOA, GPIO_Pin_15); }
    void PwrKeyPulse(void) { PwrKeyLo(); Delay.ms(1008); PwrKeyHi(); }
    void PowerOn(void)  { klGpioClearByMsk(GPIOB, GPIO_Pin_13); }
    void PowerOff(void) { klGpioSetByMsk  (GPIOB, GPIO_Pin_13); }
    // UART operations
    void IrqDisable(void) { USART_ITConfig(USART1, USART_IT_RXNE, DISABLE); }
    void IrqEnable (void) { USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); }
    void SendString(const char *S);
    // Commands and strings
    Error_t Command(const char *ACmd);
    Error_t CmdAt(void) { return (Command("AT") == erOk)? erOk : erError; }
    // Card, call, sms
    Error_t NetRegistration(void);
    Error_t ProcessSim(void);
public:
    Error_t State;  // Modem state
    void Init(void);
    Error_t SendSMS(const char *ANumber, const char *AMsg);
    // IRQ
    void IRQHandler(void);
};

extern sim900_t Mdm;

// USART RX IRQ
extern "C" {
void USART1_IRQHandler(void);
}


#endif /* SIM900_H_ */
