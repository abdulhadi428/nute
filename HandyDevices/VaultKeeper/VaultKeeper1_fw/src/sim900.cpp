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
#include <stdarg.h>
#include "comline.h"
#include "kl_string.h"

sim900_t Mdm;
//extern LedBlink_t Led;

void sim900_t::Init() {
    GPIOInit();
    USARTInit();
    IrqEnable();
    PowerOff();
    PwrKeyHi();
}

void sim900_t::On() {
    Com.Printf("Starting Mdm...\r");
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
        Com.Printf("1.");
        if(Command("AT")        != erOk) continue;  // Send AT and wait for Ok
        Com.Printf("2.");
        if(ProcessSim()         != erOk) continue;  // Wait for SIM to activate
        Com.Printf("3.");
        if(Command("AT+CMGF=1") != erOk) continue;  // SMS in text mode
        Com.Printf("4.");
        if(DisableCellBrc()     != erOk) continue;  // Disable cell broadcast
        Com.Printf("5.");
        if(NetRegistration()    != erOk) continue;  // Wait for modem to connect
        Com.Printf("6.");
        //Command("AT+CSQ");                          // Get signal parameters
        Command("AT+CMGD=1,4");                     // Delete all sms

        State = erOk;
    } // while
    Com.Printf("\rMdm ready\r");
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
    // Surround number with double quotes
    BufReset();
    MPrintf("AT+CMGS=\"%S\"\r\n", ANumber);
    if (WaitChar('>', MDM_RX_TIMEOUT) == erOk) {    // Invitation received
        MPrintf("%S%c", AMsg, 26);                  // Send message and finishing Ctrl-Z
        // Wait for echo
        BufReset();
        if (WaitString("OK", MDM_SMS_TIMEOUT) == erOk) {
            Com.Printf("SMS sent\r");
            return erOk;
        }
    } // if >
    Com.Printf("SMS failed\r");
    return erError;
}

Error_t sim900_t::GprsOn() {
    Com.Printf("Connecting GPRS...\r");
    if (Command("AT+CGATT=1", MDM_NETREG_TIMEOUT) != erOk)          return erError;
    if (Command("AT+CGATT?", MDM_RX_TIMEOUT, "+CGATT: 1") != erOk)  return erError;
    WaitString("OK", MDM_RX_TIMEOUT);
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


Error_t sim900_t::OpenConnection(const char *AHost) {
    Com.Printf("Connecting %S ", AHost);
    klSPrintf(TxLine, "AT+CIPSTART=\"TCP\",\"%S\",\"80\"", AHost);
    if (Command(TxLine) == erOk) {
        Com.Printf("#");
        if(WaitString("CONNECT", MDM_NETREG_TIMEOUT) == erOk) {
            if (strcmp(&RxLine[8], "OK") == 0) {
                Com.Printf("#\rOk\r");
                return erOk;
            }
        }
    }
    Com.Printf("\r");
    return erError;
}

Error_t sim900_t::CloseConnection(void) {
    Com.Printf("Disconnecting\r");
    return Command("AT+CIPCLOSE", MDM_NETREG_TIMEOUT);
}


enum LengthEncoding_t {leLength, leChunks, leNone};
Error_t sim900_t::GET(const char *AHost, const char *AUrl, char *AData, uint32_t ALen, bool AKeepAlive) {
    Com.Printf("GET: %S%S\r", AHost, AUrl);
    Error_t r = erError;
    BufReset();
    SendString("AT+CIPSEND");
    if (WaitChar('>', MDM_RX_TIMEOUT) == erOk) {
        MPrintf("GET %S HTTP/1.1\r\n", AUrl);
        if (AKeepAlive) MPrintf("Connection: Keep-Alive\r\n");
        else MPrintf("Connection: close\r\n");
        MPrintf("Host: %S\r\n\r\n%c", AHost, 26);
        // Wait for echo
        BufReset();
        if (WaitString("SEND OK", MDM_NETREG_TIMEOUT) == erOk) {
            Com.Printf("#");
            uint32_t FLen = 0;

            // ==== Receive header ====
            LengthEncoding_t LengthEncoding = leNone;
            while((r = WaitAnyString(MDM_NETREG_TIMEOUT)) == erOk) {
                if (klStrStartWith(RxLine, "Content-Length")) {     // Get length
                    LengthEncoding = leLength;
                    FLen = atoi(&RxLine[16]);
                    if (FLen != 0) FLen = (FLen < ALen)? FLen : ALen;
                }
                else if (klStrStartWith(RxLine, "Transfer-Encoding: chunked")) {
                    LengthEncoding = leChunks;
                }
                else if (RxLine[0] == 0) break;
            } // while wait any

            // ==== Receive data ====
            if (r == erOk) {    // will be ok in case of successful empty line receive
                uint32_t Tmr, Indx=0;
                if (LengthEncoding == leLength) {
                    Delay.Reset(&Tmr);
                    while((Indx < FLen) and (r == erOk)) {
                        if (!BufIsEmpty()) AData[Indx++] = BufRead();
                        if (Delay.Elapsed(&Tmr, MDM_NETREG_TIMEOUT)) r = erTimeout;
                    } // while
                }

                else if (LengthEncoding == leChunks) {
                    while ((Indx < ALen-1) and (r == erOk)) {
                        if((r = WaitAnyString(MDM_NETREG_TIMEOUT)) == erOk) { // Receive chunk length
//                                        Com.Printf("Rx: %S\r", RxLine);
                            if (klHexToUint(RxLine, &FLen)) {           // Try to convert length
//                                            Com.Printf("FLen: %u\r", FLen);
                                if (FLen == 0) break;                   // End of data
                                Delay.Reset(&Tmr);                      // Receive chunk
                                while((Indx < FLen) and (r == erOk)) {
                                    if (!BufIsEmpty()) AData[Indx++] = BufRead();
                                    if (Delay.Elapsed(&Tmr, MDM_NETREG_TIMEOUT)) r = erTimeout;
                                } // while
                            } // if hex2uint
                            else r = erError;
                        } // if waitstring
//                                    Com.Printf("Indx2: %u\r", Indx);
                    } // while indx < alen
                }

                else if (LengthEncoding == leNone) {
                    while(Indx < ALen-1) {
                        if(WaitAnyString(MDM_NETREG_TIMEOUT) == erOk) {
                            if (klStrStartWith(RxLine, "CLOSED")) break;
                            else {
                                AData = klStrCpy(AData, RxLine);
                                Indx += strlen(RxLine);
                            }
                        }
                    }
                }

                if (r == erOk) {
                    AData[Indx] = 0; // Terminate string
                    if (Indx > 0) r = erOk;
                }
            } // if ok
        } // Send ok
    } // if >
    if(r == erOk) Com.Printf("\rOk: %S\r", AData);
    else Com.Printf("\rError\r");
    return r;
}

// numenor2012.ru, /request.php, data=aga
Error_t sim900_t::POST(const char *AHost, const char *AUrl, const char *AData, bool AKeepAlive) {
    Com.Printf("POST: %S%S; %S\r", AHost, AUrl, AData);
    Error_t r = erError;
    BufReset();
    SendString("AT+CIPSEND");
    if (WaitChar('>', MDM_RX_TIMEOUT) == erOk) {
        MPrintf("POST %S HTTP/1.1\r\n", AUrl);
        MPrintf("Host: %S\r\n", AHost);
        if (AKeepAlive) MPrintf("Connection: Keep-Alive\r\n");
        else MPrintf("Connection: close\r\n");
        MPrintf("Content-Type: application/x-www-form-urlencoded\r\n"); // Mandatory
        MPrintf("Content-Length: %u\r\n\r\n", strlen(AData));           // Add empty line
        MPrintf("%S%c", AData, 26);                                     // Send data and finishing Ctrl-Z
        // Wait for echo
        BufReset();
        r = WaitString("SEND OK", MDM_NETREG_TIMEOUT);
    } // if >
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

void sim900_t::MPrintUint(uint32_t ANumber) {
    uint8_t digit = '0';
    bool ShouldPrint = false;
    const uint32_t m[9] = {1000000000, 100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10};
    for(uint8_t i=0; i<9; i++) {
        while (ANumber >= m[i]) {
            digit++;
            ANumber -= m[i];
        }
        if (digit != '0' || ShouldPrint) {
            WriteByte(digit);
            ShouldPrint = true;
        }
        digit = '0';
    } // for
    WriteByte((uint8_t)('0'+ANumber));
}
void sim900_t::MPrintf(const char *S, ...) {
    Delay.ms(99);   // to prevent transmission corruption
    char c;
    bool WasPercent = false;
    va_list Arg;
    va_start(Arg, S);    // Set pointer to first argument
    while ((c = *S) != 0) {
        if (c == '%') {
            if (WasPercent) {
                WriteByte(c);  // %%
                WasPercent = false;
            }
            else WasPercent = true;
        }
        else { // not %
            if (WasPercent) {
                if (c == 'c') WriteByte((uint8_t)va_arg(Arg, int32_t));
                else if (c == 'u') MPrintUint(va_arg(Arg, uint32_t));
                else if ((c == 's') || (c == 'S')) MPrintString(va_arg(Arg, char*));
                WasPercent = false;
            } // if was percent
            else WriteByte(c);
        }
        S++;
    } // while
    va_end(Arg);
}

// ==== RX ====
Error_t sim900_t::WaitString(const char *AString, uint32_t ATimeout) {
    uint32_t Tmr;
    char c;
    uint32_t Indx = 0;
    Delay.Reset(&Tmr);
    while (!Delay.Elapsed(&Tmr, ATimeout)) {
        if (!BufIsEmpty()) {
            c = BufRead();
            if (c == '\r') continue;    // <cr>
            else if (c == '\n') { // <lf> Line received
                if(Indx != 0) {    // Something received
                    RxLine[Indx] = 0;  // end of string
                    //Com.Printf("> %S\r", RxLine);
                    if      (strncmp(RxLine, AString, strlen(AString)) == 0) return erOk;
                    else if (strncmp(RxLine, "ERROR", 5) == 0) return erError;
                    else Indx = 0;
                }
            }
            else RxLine[Indx++] = c;
        } // if not empty
    } // while
    return erTimeout;
}
Error_t sim900_t::WaitEmptyString(uint32_t ATimeout) {
    uint32_t Tmr;
    char c;
    bool WasEmpty = false;
    Delay.Reset(&Tmr);
    while (!Delay.Elapsed(&Tmr, ATimeout)) {
        if (!BufIsEmpty()) {
            c = BufRead();
            if (c == '\r') continue;    // <cr>
            else if (c == '\n') { // <lf> Line received
                if (WasEmpty) return erOk;
                else WasEmpty = true;   // end of line, new one begins
            }
            else WasEmpty = false;  // some symbol
        } // if not empty
    } // while
    return erTimeout;
}

// Receive string of any length including zero
Error_t sim900_t::WaitAnyString(uint32_t ATimeout) {
    uint32_t Tmr;
    char c;
    uint32_t Indx = 0;
    Delay.Reset(&Tmr);
    while (!Delay.Elapsed(&Tmr, ATimeout)) {
        if (!BufIsEmpty()) {
            c = BufRead();
            if (c == '\r') continue;    // <cr>
            else if (c == '\n') {       // <lf> Line received
                RxLine[Indx] = 0;   // end of string
                //Com.Printf("> %S\r", RxLine);
                return erOk;
            }
            else RxLine[Indx++] = c;
            if(Indx >= MDM_LINE_LEN) return erError;
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
Error_t sim900_t::Command(const char *ACmd, uint32_t ATimeout, const char *AReply) {
    BufReset();
    SendString(ACmd);
    return WaitString(AReply, ATimeout);
}

Error_t sim900_t::ProcessSim() {
    Com.Printf("Sim init ");
    uint32_t Tmr;
    Delay.Reset(&Tmr);
    while (!Delay.Elapsed(&Tmr, MDM_SIM_TIMEOUT)) {
        Com.Printf("#");
        if (Command("AT+CPIN?", MDM_RX_TIMEOUT, "+CPIN: READY") == erOk) {
            WaitString("OK", MDM_RX_TIMEOUT);
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
        if (Command("AT+CREG?", MDM_RX_TIMEOUT, "+CREG:") == erOk) {
            char c = RxLine[9];
            WaitString("OK", MDM_RX_TIMEOUT);
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
