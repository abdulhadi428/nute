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
#define MDM_SIM_TIMEOUT     4000  // ms
#define MDM_NETREG_TIMEOUT  20000 // ms

typedef enum {flOk, flError, flNone, flRepeatNeeded} flReply_t;
typedef enum {ssReady, ssPinNeeded, ssPukNeeded, ssUnknown} SimState_t;
typedef enum {nsNotRegNoSearch=0, nsRegistered=1, nsSearching=2, nsRegDenied=3, nsUnknown=4, nsRegRoaming=5} NetState_t;

class mdm_t {
private:
    uint32_t Timer;
    // State flags
    bool fRDY, fEchoRcvd;
    flReply_t fReply;
    uint8_t fCFUN;

    SimState_t SimState;
    NetState_t NetState;
    Error_t State;  // Modem state
    // Strings
    char Cmd[54];   // Command to compare with
    char Line[54];  // Received line
    uint8_t CharCounter;

    // Init
    void GPIOInit(void);
    void USARTInit(void);
    // Pins operations
    void PwrKeyHi(void) { GPIOA->BSRR = GPIO_Pin_4; }
    void PwrKeyLo(void) { GPIOA->BRR  = GPIO_Pin_4; }
    // UART operations
    void SendString(const char *S);
    void DisableRxIrq(void) { USART_ITConfig(USART2, USART_IT_RXNE, DISABLE); }
    void EnableRxIrq (void) { USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); }
    // commands and strings
    Error_t Command(const char *ACmd);
    void ProcessRxLine(void);
    void ProcessCMEError(uint32_t AErr);
    void ParseCPIN(char *S);
    // Card, call, sms
    Error_t WaitForNetRegistration(void);
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
