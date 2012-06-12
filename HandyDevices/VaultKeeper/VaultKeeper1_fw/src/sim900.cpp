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
#include "comline.h"
#include "kl_string.h"

sim900_t Mdm;
//extern LedBlink_t Led;

void sim900_t::On() {
    Com.Printf("Starting Mdm...\r");
    GPIOInit();
    USARTInit();
    IrqEnable();
    State = erError;
    WriteIndx = 0;
    ReadIndx = 0;

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
        if(DisableCellBrc()     != erOk) continue;  // Disable cell broadcast
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
    klSPrintf(TxLine, "AT+CMGS=\"%S\"", ANumber);
    BufReset();
    SendString(TxLine);
    if (WaitChar('>', MDM_RX_TIMEOUT) == erOk) {    // Invitation received
        // Send message
        char c;
        while ((c = *AMsg++) != 0) WriteByte(c);
        WriteByte(26);  // Send Ctrl-Z
        // Wait for echo
        BufReset();
        if (WaitString("OK", 2, MDM_SMS_TIMEOUT) == erOk) {
            Com.Printf("SMS sent\r");
            return erOk;
        }
    } // if >
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
    Com.Printf("GPRS connected\r");
    return erOk;
}
Error_t sim900_t::GprsOff(void) {
    Command("AT+SAPBR=0,1", MDM_NETREG_TIMEOUT);
    Error_t r = Command("AT+CGATT=0", MDM_NETREG_TIMEOUT);
    Com.Printf("GPRS off\r");
    return r;
}

Error_t sim900_t::HttpInit(const char *AUrl) {
    if(Command("AT+HTTPINIT") == erOk) {                // Init HTTP service
        if(Command("AT+HTTPPARA=\"CID\",1") == erOk) {  // CID = 1 (bearer id)
            klSPrintf(TxLine, "AT+HTTPPARA=\"URL\",\"%S\"", AUrl);  // Set parameters: AT+HTTPPARA="URL","ya.ru"
            return Command(TxLine);
        }
    }
    return erError;
}

Error_t sim900_t::GET(const char *AUrl) {
    Com.Printf("GET: %S\r", AUrl);
    Error_t r = erError;
    if (HttpInit(AUrl) == erOk) {
        // Start GET session
        if(Command("AT+HTTPACTION=0") == erOk) {    // 0 => GET
            // Get server reply: +HTTPACTION:0,200,13652
            if (WaitString("+HTTPACTION", 11, MDM_NETREG_TIMEOUT) == erOk) {
                // Check error code
                for(uint8_t i=0; i<3; i++) TxLine[i] = RxLine[14+i];
                TxLine[3] = 0;
                Com.Printf("Rpl code: %S\r", TxLine);
                if (strcmp(TxLine, "200") == 0) {
                    // Calculate reply length
                    strncpy(TxLine, &RxLine[18], 9);
                    uint32_t LenToRx = atoi(TxLine);
                    if (LenToRx != 0) {     // Read data
                        LenToRx = (LenToRx < MDM_DATA_LEN)? LenToRx : MDM_DATA_LEN;
                        klSPrintf(TxLine, "AT+HTTPREAD=0,%u", LenToRx);
                        Com.Printf("LenToRx: %u\r", LenToRx);
                        if(Command(TxLine, MDM_NETREG_TIMEOUT, "+HTTPREAD", 9) == erOk) {    // Next line(s) contain data
                            if (ReadRawData(DataString, LenToRx, MDM_NETREG_TIMEOUT) == erOk) {
                                WaitString("OK", 2, MDM_RX_TIMEOUT);
                                r = erOk;
                            }
                        } // if read
                    } // Len>0
                } // if 200
            } // srv reply
        } // Start session
    } // HTTP init
    // Terminate HTTP service
    Command("AT+HTTPTERM");
    if(r == erOk) Com.Printf("Ok: %S\r", DataString);
    else Com.Printf("Error\r");
    return r;
}

Error_t sim900_t::POST(const char *AUrl, const char *AData) {
    Com.Printf("POST: %S; %S\r", AUrl, AData);
    Error_t r = erError;
    uint32_t Len = strlen(AData);
    if (HttpInit(AUrl) == erOk) {
        // Set data length and latency
        klSPrintf(TxLine, "AT+HTTPDATA=%u,9999", Len);
        if(Command(TxLine, MDM_NETREG_TIMEOUT, "DOWNLOAD", 8) == erOk) {
            for(uint32_t i=0; i<Len; i++) WriteByte(AData[i]);
            if (WaitString("OK", 2, MDM_NETREG_TIMEOUT) == erOk) {
                // Start POST session
                if(Command("AT+HTTPACTION=1") == erOk) {    // 1 => POST
                    // Get server reply: +HTTPACTION:0,200,13652
                    if (WaitString("+HTTPACTION", 11, MDM_NETREG_TIMEOUT) == erOk) {
                        // Check error code
                        for(uint8_t i=0; i<3; i++) TxLine[i] = RxLine[14+i];
                        TxLine[3] = 0;
                        Com.Printf("Rpl code: %S\r", TxLine);
                        if (strcmp(TxLine, "200") == 0) r = erOk;
                    } // if reply rcvd
                } // if action start
            } //
        }
    } // HTTP init
    // Terminate HTTP service
    Command("AT+HTTPTERM");
    if(r == erOk) Com.Printf("Ok\r");
    else Com.Printf("Error\r");
    return r;
}

Error_t sim900_t::POST1(const char *AUrl, const char *AData) {
    Com.Printf("POST1: %S; %S\r", AUrl, AData);
    Error_t r = erError;
    //uint32_t Len = strlen(AData);

    klSPrintf(TxLine, "AT+CIPSTART=\"TCP\",\"%S\",\"80\"", AUrl);
    if (Command(TxLine) == erOk) {
        if(WaitString("CONNECT", 7, MDM_NETREG_TIMEOUT) == erOk) {
            if (strcmp(&RxLine[8], "OK") == 0) {
                BufReset();
                SendString("AT+CIPSEND");
                if (WaitChar('>', MDM_RX_TIMEOUT) == erOk) {
                    SendString("POST /request.php HTTP/1.1");
                    SendString("Host: numenor2012.ru");
                    SendString("Content-Type: application/x-www-form-urlencoded");
                    SendString("Content-Length: 18");
                    SendString("");
                    char c;
                    while ((c = *AData++) != 0) WriteByte(c);
                    WriteByte(26);  // Send Ctrl-Z
                    // Wait for echo
                    BufReset();
                    if (WaitString("SEND OK", 2, MDM_NETREG_TIMEOUT) == erOk) {
                        r = erOk;
                    }
                }
            } // if connect ok
        } // if wait connect
    } // if cmd
    Command("AT+CIPCLOSE", MDM_NETREG_TIMEOUT);
    if(r == erOk) Com.Printf("Ok\r");
    else Com.Printf("Error\r");
    return r;
}

// ========================== Commands and strings =============================
void sim900_t::SendString(const char *S) {
    Delay.ms(99);   // to prevent transmission corruption
    char c;
    while ((c = (*S++)) != 0) WriteByte(c);
    WriteByte('\r');    // Send CR (13 or 0x0D)
    WriteByte('\n');    // Send LF (10 or 0x0A)
}

//Error_t sim900_t::WaitString(uint32_t ATimeout) {
//    uint32_t Tmr;
//    Delay.Reset(&Tmr);
//    while (!Delay.Elapsed(&Tmr, ATimeout)) {
//        if (NewLineReceived) {
//            Com.Printf("> %S\r", RxLine);
//            return erOk;
//        }
//    } // while
//    return erTimeout;
//}

// ==== RX ====
Error_t sim900_t::WaitString(const char *AString, uint8_t ALen, uint32_t ATimeout) {
    uint32_t Tmr;
    char c;
    uint32_t Indx = 0;
    Delay.Reset(&Tmr);
    while (!Delay.Elapsed(&Tmr, ATimeout)) {
        // Read line from buffer
        if (!BufIsEmpty()) {
            c = BufRead();
            if (c == '\r') continue;    // <cr>
            else if (c == '\n') { // <lf> Line received
                if(Indx != 0) {    // Something received
                    RxLine[Indx] = 0;  // end of string
                    Com.Printf("> %S\r", RxLine);
                    if      (strncmp(RxLine, AString, ALen) == 0) return erOk;
                    else if (strncmp(RxLine, "ERROR", 5) == 0) return erError;
                    else Indx = 0;
                }
            }
            else RxLine[Indx++] = c;
        } // if not empty
    } // while
    return erTimeout;
}

Error_t sim900_t::WaitChar(const char AChar, uint32_t ATimeout) {
    uint32_t Tmr;
    Delay.Reset(&Tmr);
    while (!Delay.Elapsed(&Tmr, ATimeout)) {
        if (!BufIsEmpty())
            if (BufRead() == AChar)
                return erOk;
    } // while
    return erTimeout;
}

Error_t sim900_t::ReadRawData(char *Dst, uint32_t ALen, uint32_t ATimeout) {
    uint32_t Tmr, Indx = 0;
    Delay.Reset(&Tmr);
    while (!Delay.Elapsed(&Tmr, ATimeout) and (Indx < ALen)) {
        if (!BufIsEmpty()) Dst[Indx++] = BufRead();
    } // while
    Dst[Indx] = 0;   // Terminate string
    //Com.Printf(">> %S\r", Dst);
    return (Indx == ALen)? erOk : erTimeout;
}

// ==== Commands ====

Error_t sim900_t::Command(const char *ACmd, uint32_t ATimeout, const char *AReply, uint8_t ARplLen) {
    BufReset();
    SendString(ACmd);
    return WaitString(AReply, ARplLen, ATimeout);
}

Error_t sim900_t::ProcessSim() {
    Com.Printf("Sim init ");
    uint32_t Tmr;
    Delay.Reset(&Tmr);
    while (!Delay.Elapsed(&Tmr, MDM_SIM_TIMEOUT)) {
        Com.Printf("#");
        if (Command("AT+CPIN?", MDM_RX_TIMEOUT, "+CPIN: READY", 12) == erOk) {
            WaitString("OK", 2, MDM_RX_TIMEOUT);
            Com.Printf("\rSim Ok\r");
            return erOk;
        }
        Delay.ms(450);
    } // while !elapsed
    Com.Printf("\rSim error\r");
    return erTimeout;
}

Error_t sim900_t::DisableCellBrc(void) {
    Com.Printf("DisableCellBrc ");
    uint32_t Tmr;
    Delay.Reset(&Tmr);
    while (!Delay.Elapsed(&Tmr, MDM_NETREG_TIMEOUT)) {
        Com.Printf("#");
        if (Command("AT+CSCB=1") == erOk) {
            Com.Printf("\rOk\r");
            return erOk;
        }
        Delay.ms(1800);
    } // while !elapsed
    Com.Printf("\rError\r");
    return erTimeout;
}

Error_t sim900_t::NetRegistration() {
    Com.Printf("Net registration ");
    uint32_t FTimer;
    Delay.Reset(&FTimer);
    while (!Delay.Elapsed(&FTimer, MDM_NETREG_TIMEOUT)) {
        Com.Printf("#");
        if (Command("AT+CREG?", MDM_RX_TIMEOUT, "+CREG:", 6) == erOk) {
            char c = RxLine[9];
            WaitString("OK", 2, MDM_RX_TIMEOUT);
            uint8_t k = c - '0';    // convert char to digit
            switch (k) {
                case 1: // Registered
                case 5: // Roaming
                    Com.Printf("\rNet connected\r");
                    return erOk;
                    break;
                case 3: // Denied
                    Com.Printf("\rDenied\r");
                    return erError;
                    break;
                default: break;
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
void USART1_IRQHandler(void) {
    if(USART_GetITStatus(USART1, USART_IT_RXNE) == RESET) return;
    Mdm.BufWrite(USART1->DR);
}
