/*
 * sim900.cpp
 *
 *  Created on: 01.06.2012
 *      Author: kreyl
 */

#include "sim900.h"
#include "stm32f10x.h"
#include "misc.h"
#include <string.h>
#include <stdlib.h>
#include "led.h"
#include "comline.h"

sim900_t Mdm;
//extern LedBlink_t Led;

void sim900_t::On() {
    Com.Printf("Starting Mdm...\r");
    GPIOInit();
    USARTInit();
    IrqEnable();
    State = erError;
    InviteReceived = true;  // Do not react on '>'
    RawDataRx = false;
    uint8_t TryCounter = 0;
    // ==== Setup modem ====
    while (State == erError) {
        if (TryCounter == 0) {  // Switch power off and on
            TryCounter = 4;
            PowerOff();
            Delay.ms(2700);
            PowerOn();
            Delay.ms(1008);
        }
        TryCounter--;
        PwrKeyPulse();      // Switch on
        // Wait and send AT
        Delay.ms(999);
        if(Command("AT")        != erOk) continue;  // Send AT and wait for Ok
        if(ProcessSim()         != erOk) continue;  // Wait for SIM to activate
        if(Command("AT+CMGF=1") != erOk) continue;  // SMS in text mode
        if(Command("AT+CSCB=1") != erOk) continue;  // Disable cell broadcast
        if(NetRegistration()    != erOk) continue;  // Wait for modem to connect
        //Command("AT+CSQ");                          // Get signal parameters
        Command("AT+CMGD=1,4");                     // Delete all sms
        State = erOk;
    } // while
    Com.Printf("Mdm ready\r");
}

void sim900_t::Off() {
    Command("AT+CPOWD=1");
    Delay.ms(4500);
    PowerOff();
    Com.Printf("Mdm off\r");
}

// ============================= Call, SMS etc. ================================
Error_t sim900_t::SendSMS(const char *ANumber, const char *AMsg) {
    Com.Printf("Send SMS: %S; %S\r", ANumber, AMsg);
    // Build command: surround number with double quotes
    uint32_t Tmr;
    Delay.Reset(&Tmr);
    char S[MDM_LINE_LEN];
    uint8_t L = strlen(ANumber);
    memcpy(&S[0], "AT+CMGS=\"", 9);
    memcpy(&S[9], ANumber, L);
    S[9+L]  = '\"';
    S[10+L] = 0;
    ResetRxLine();
    InviteReceived = false; // wait invitation
    SendString(S);
    while (!Delay.Elapsed(&Tmr, MDM_RX_TIMEOUT) and !InviteReceived);
    if (InviteReceived) {
        // Send message
        char c;
        while ((c = *AMsg++) != 0) WriteByte(c);
        // Send Ctrl-Z
        WriteByte(26);
        // Wait for echo
        ResetRxLine();
        if (WaitString(MDM_SMS_TIMEOUT) == erOk) {
            if (strcmp (RxLine, "OK") == 0) {
                Com.Printf("SMS sent\r");
                return erOk;
            }
        }
    }
    Com.Printf("SMS failed\r");
    return erError;
}

Error_t sim900_t::GprsOn() {
    Com.Printf("Connecting GPRS...\r");
    if (Command("AT+CGATT=1", MDM_NETREG_TIMEOUT) != erOk)            return erError;
    if (Command("AT+CGATT?", MDM_RX_TIMEOUT, "+CGATT: 1", 9) != erOk) return erError;
    WaitString("OK", 2, MDM_RX_TIMEOUT);
    // GPRS is on, setup IP context
    if(Command("AT+SAPBR=3,1,\"Contype\",\"GPRS\"") != erOk)        return erError;
    if(Command("AT+SAPBR=3,1,\"APN\",\"internet.mts.ru\"") != erOk) return erError;
    if(Command("AT+SAPBR=1,1", MDM_NETREG_TIMEOUT) != erOk)         return erError;     // Open GPRS context
    //if(Mdm.Command("AT+SAPBR=2,1", MDM_NETREG_TIMEOUT) != erOk) return erError;

//    if(Command("AT+CSTT=\"internet.mts.ru\"")!= erOk)  return erError;
//    if(Command("AT+CIICR", MDM_NETREG_TIMEOUT)!= erOk) return erError;  // Start GPRS connection
//    Command("AT+CIFSR");    // Get local IP address. Needed for successfull proceeding
    //if(Command("AT+CIPHEAD=1", MDM_RX_TIMEOUT)!= erOk)  return erError; // Add IP header
    Com.Printf("GPRS connected\r");
    return erOk;
}
Error_t sim900_t::GprsOff(void) {
    Command("AT+SAPBR=0,1", MDM_NETREG_TIMEOUT);
    Error_t r = Command("AT+CGATT=0", MDM_NETREG_TIMEOUT);
    Com.Printf("GPRS off\r");
    return r;
}

Error_t sim900_t::GET(const char *AUrl) {
    Com.Printf("GET: %S\r", AUrl);
    Error_t r = erError;
    DataLen = 0;
    PD = &DataString[0];
    if(Command("AT+HTTPINIT") == erOk) {                // Init HTTP service
        if(Command("AT+HTTPPARA=\"CID\",1") == erOk) {  // Set parameters: AT+HTTPPARA="URL","ya.ru"
            char S[MDM_LINE_LEN];
            uint8_t Len = strlen(AUrl);
            memcpy(&S[0], "AT+HTTPPARA=\"URL\",\"", 19);
            memcpy(&S[19], AUrl, Len);
            strcpy(&S[19+Len], "\"");
            if(Command(S) == erOk) {
                // Start GET session
                if(Command("AT+HTTPACTION=0") == erOk) {    // 0 => GET
                    // Get server reply: +HTTPACTION:0,200,13652
                    if (WaitString("+HTTPACTION", 11, MDM_NETREG_TIMEOUT) == erOk) {
                        // Check error code
                        for(uint8_t i=0; i<3; i++) S[i] = RxLine[14+i];
                        S[3] = 0;
                        Com.Printf("Rpl code: %S\r", S);
                        if (strcmp(S, "200") == 0) {
                            // Calculate reply length
                            strncpy(S, &RxLine[18], 9);
                            uint32_t LenToRx = atoi(S);
                            if (LenToRx != 0) {     // Read data
                                LenToRx = (LenToRx < MDM_DATA_LEN)? LenToRx : MDM_DATA_LEN;
                                memcpy(S, "AT+HTTPREAD=0,", 14);
                                Com.UintToStr(LenToRx, &S[14]);     // Add current length
                                Com.Printf("LenToRx: %S\r", &S[14]);
                                if(Command(S, MDM_NETREG_TIMEOUT, "+HTTPREAD", 9) == erOk) {    // Next line(s) contain data
                                    RawDataRx = true;
                                    uint32_t Tmr;
                                    Delay.Reset(&Tmr);
                                    r = erOk;
                                    while (DataLen <= LenToRx) {
                                        if(Delay.Elapsed(&Tmr, MDM_NETREG_TIMEOUT)) {
                                            r = erError;
                                            break;
                                        }
                                    } // while
                                    RawDataRx = false;  // return to normal life
                                    *PD = 0;    // Terminate received string
                                    WaitString("OK", 2, MDM_RX_TIMEOUT);
                                } // if read
                            } // Len>0
                        } // if 200
                    } // srv reply
                } // Start session
            } // param 2
        } // param 1
    } // init
    // Terminate HTTP service
    Command("AT+HTTPTERM");
    if(r == erOk) Com.Printf("Ok: %S\r", DataString);
    else Com.Printf("Error\r");
    return r;
}

// ========================== Commands and strings =============================
void sim900_t::SendString(const char *S) {
    Delay.ms(99);   // to prevent transmission corruption
    char c;
    while ((c = (*S++)) != 0) WriteByte(c);
    WriteByte('\r');    // Send CR (13 or 0x0D)
}

Error_t sim900_t::WaitString(uint32_t ATimeout) {
    uint32_t Tmr;
    Delay.Reset(&Tmr);
    while (!Delay.Elapsed(&Tmr, ATimeout)) {
        if (NewLineReceived) return erOk;
    } // while
    return erTimeout;
}
Error_t sim900_t::WaitString(const char *AString, uint8_t ALen, uint32_t ATimeout) {
    uint32_t Tmr;
    Delay.Reset(&Tmr);
    while (!Delay.Elapsed(&Tmr, ATimeout)) {
        if (NewLineReceived) {
            if      (strncmp(RxLine, AString, ALen) == 0) return erOk;
            else if (strcmp (RxLine, "ERROR") == 0) return erError;
            else ResetRxLine();
        }
    } // while
    return erTimeout;
}

Error_t sim900_t::Command(const char *ACmd, uint32_t ATimeout, const char *AReply, uint8_t ARplLen) {
    ResetRxLine();
    SendString(ACmd);
    return WaitString(AReply, ARplLen, ATimeout);
}
Error_t sim900_t::Command(const char *ACmd, uint32_t ATimeout) {
    ResetRxLine();
    SendString(ACmd);
    return WaitString("OK", 2, ATimeout);
}
Error_t sim900_t::Command(const char *ACmd) {
    ResetRxLine();
    SendString(ACmd);
    return WaitString("OK", 2, MDM_RX_TIMEOUT);
}

Error_t sim900_t::ProcessSim() {
    uint32_t Tmr, TryCounter = 7;
    Delay.Reset(&Tmr);
    while (!Delay.Elapsed(&Tmr, MDM_SIM_TIMEOUT) and (TryCounter-- != 0)) {
        if (Command("AT+CPIN?", MDM_RX_TIMEOUT, "+CPIN: READY", 12) == erOk) {
            Com.Printf("Sim Ok\r");
            WaitString("OK", 2, MDM_RX_TIMEOUT);
            return erOk;
        }
        Delay.ms(450);
    } // while !elapsed
    Com.Printf("Sim error\r");
    return erTimeout;
}

Error_t sim900_t::NetRegistration() {
    Com.Printf("Waiting for net registration ");
    uint32_t FTimer;
    Delay.Reset(&FTimer);
    while (!Delay.Elapsed(&FTimer, MDM_NETREG_TIMEOUT)) {
        NetState = nsUnknown;
        Com.Printf("#");
        if (Command("AT+CREG?", MDM_RX_TIMEOUT, "+CREG:", 6) == erOk) {
            char c = RxLine[9];
            WaitString("OK", 2, MDM_RX_TIMEOUT);
            uint8_t k = c - '0';    // convert char to digit
            if ((k>=0) && (k<=5)) NetState = (NetState_t)k;
            switch (NetState) {
                case nsRegistered:
                case nsRegRoaming:
                    Com.Printf("\rNet connected\r");
                    return erOk;
                    break;
                case nsRegDenied:
                    Com.Printf("\rDenied\r");
                    return erError;
                    break;
                case nsNotRegNoSearch:
                case nsUnknown:     // Not answered yet
                case nsSearching:   // Searching
                    break;
            } // switch
        } // if needed reply
        Delay.ms(1800);
    } // while !elapsed
    Com.Printf("\rTimeout\r");
    return erTimeout;
}

// ================================== Init =====================================
void sim900_t::GPIOInit() {
    // PA11=CTS, PA12=RTS, PA9=TX, PA10=RX, PA15=PWR_KEY; PC8=Status, PC9 = DCD, PC10=RI, PC11=DTR, PC12=DSR
    // Configure USART Tx and RTS as alternate function push-pull
    klGpioSetupByMsk(GPIOA, GPIO_Pin_9|GPIO_Pin_12, GPIO_Mode_AF_PP);
    // Configure USART Rx and CTS as input floating
    klGpioSetupByMsk(GPIOA, GPIO_Pin_10|GPIO_Pin_11, GPIO_Mode_IN_FLOATING);
    // Configure PWR_KEY as Open-Drain output
    klGpioSetupByMsk(GPIOA, GPIO_Pin_15, GPIO_Mode_Out_OD);
    PwrKeyHi();
    // PowerEnable pin
    klGpioSetupByMsk(GPIOB, GPIO_Pin_13, GPIO_Mode_Out_OD);
    PowerOff();
}

void sim900_t::USARTInit() {
    // Clocks
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    // USART
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    //USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_RTS_CTS;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    // Enable USART
    USART_Cmd(USART1, ENABLE);
    // Setup interrupt
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

// =========================== Interrupt =======================================
void sim900_t::IRQHandler() {
    if(USART_GetITStatus(USART1, USART_IT_RXNE) == RESET) return;
    static char Buf[MDM_LINE_LEN];
    // Read one byte from the receive data register
    uint8_t b = USART_ReceiveData(USART1);
#ifdef MDM_VERBOSE_IO
    //USART_SendData(USART1, b);
#endif
    // Process the byte
    if(RawDataRx) { // do not process, just save
        if(DataLen < MDM_DATA_LEN) {
            *PD++ = b;
            DataLen++;
            return;
        }
    }

    if (b == '\n') return;  // ignore LF
    if ((b == '>') and !InviteReceived) {   // Do not react on '>' if not needed
        InviteReceived = true;
        return;
    }
    if (b == '\r') {
        if (RxCounter != 0) {
            Buf[RxCounter] = 0;  // \0 at line end
            strcpy(RxLine, Buf);
            NewLineReceived = true;
            RxCounter = 0;
        }
    }
    else Buf[RxCounter++] = b;
}
void USART1_IRQHandler(void) {
    Mdm.IRQHandler();
}
