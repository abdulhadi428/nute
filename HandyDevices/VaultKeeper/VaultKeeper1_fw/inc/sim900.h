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
#define MDM_DATA_LEN        54
#define MDM_RX_TIMEOUT      450   // ms
#define MDM_SIM_TIMEOUT     4005  // ms
#define MDM_NETREG_TIMEOUT  20007 // ms
#define MDM_SMS_TIMEOUT     18000 // ms

enum NetState_t {nsNotRegNoSearch=0, nsRegistered=1, nsSearching=2, nsRegDenied=3, nsUnknown=4, nsRegRoaming=5};


class sim900_t {
private:
    bool NewLineReceived, InviteReceived, RawDataRx;
    Error_t RplKind;
    NetState_t NetState;
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
    void WriteByte(uint8_t AByte) { USART1->DR = AByte; while (!(USART1->SR & USART_FLAG_TC)); }
    void IrqDisable(void) { USART_ITConfig(USART1, USART_IT_RXNE, DISABLE); }
    void IrqEnable (void) { USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); }
    void SendString(const char *S);
    // Commands
    Error_t Command(const char *ACmd, uint32_t ATimeout, const char *AReply, uint8_t ARplLen);
    Error_t Command(const char *ACmd, uint32_t ATimeout);
    Error_t Command(const char *ACmd);
    Error_t WaitString(const char *AString, uint8_t ALen, uint32_t ATimeout);
    Error_t WaitString(uint32_t ATimeout);
    // Inner use commands
    Error_t NetRegistration(void);
    Error_t ProcessSim(void);
public:
    Error_t State;  // Modem state
    void On(void);
    void Off(void);
    // Hi-level
    char DataString[MDM_DATA_LEN+1], *PD;
    uint32_t DataLen;
    Error_t SendSMS(const char *ANumber, const char *AMsg);
    Error_t GprsOn(void);
    Error_t GprsOff(void);
    Error_t GET(const char *AUrl);
    // IRQ
    void IRQHandler(void);
};

extern sim900_t Mdm;

// USART RX IRQ
extern "C" {
void USART1_IRQHandler(void);
}


#endif /* SIM900_H_ */
