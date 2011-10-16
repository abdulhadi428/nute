/*
 * mdm.h
 *
 *  Created on: 12.10.2011
 *      Author: Kreyl
 *      This is class for GSM modem QUECTEL M10
 */

#ifndef MDM_H_
#define MDM_H_

// PA0=CTS, PA1=RTS, PA2=TX, PA3=RX, PA4=PWR_KEY

class mdm_t {
private:
    // Pins operations
    void PwrKeyHi(void) { GPIOA->BSRR = GPIO_Pin_4; }
    void PwrKeyLo(void) { GPIOA->BRR  = GPIO_Pin_4; }
    // UART operations
    void Send(char *AString);
public:
    void Init(void);
    void EnterPowersave(void);
    void PowerDown(void) { Send("AT+CFUN=0"); } // Enter minimum functionality
    void PowerUp(void)   { Send("AT+CFUN=1"); } // Enter full functionality
    void SendSMSWithTime(char* AStrNumber, char *AMsg);
};

extern mdm_t Mdm;

#endif /* MDM_H_ */
