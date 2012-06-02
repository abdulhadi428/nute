/*
 * sim900.cpp
 *
 *  Created on: 01.06.2012
 *      Author: kreyl
 */

#include "sim900.h"
#include "stm32f10x.h"
#include "misc.h"
#include "string.h"
#include "led.h"
#include "comline.h"

sim900_t Mdm;
//extern LedBlink_t Led;

void sim900_t::Init() {
    GPIOInit();
    USARTInit();
    PowerOn();
    Delay.ms(1008);
    // ==== Setup modem ====
    State = erError;
    while (State == erError) {
        IrqEnable();
        PwrKeyPulse();      // Switch on
        Com.Printf("Mdm Pwr On\r");
        // Wait and send AT
        Delay.ms(2007);
        if(CmdAt()              != erOk) continue;  // Send AT and wait for Ok
        //if(Command("ATE0")      != erOk) continue;  // Switch echo off
        if(Command("AT+CMGF=1") != erOk) continue;  // SMS in text mode
        if(Command("AT+CSCB=1") != erOk) continue;  // Disable cell broadcast
        if(ProcessSim()         != erOk) continue;  // Wait for SIM to activate
        if(NetRegistration()    != erOk) continue;  // Wait for modem to connect
        Command("AT+CSQ");              // Get signal parameters
        //Command("AT+CUSD=1,\"#100#\""); // Get balance
        Command("AT+CMGD=1,4");         // Delete all sms

        Delay.ms(2700);
        if (SendSMS("+79169895800", "Mdm is online") == erOk) Com.Printf("SMS sent\r");

        State = erOk;
    } // while
}

// ============================= Call, SMS etc. ================================
Error_t sim900_t::SendSMS(const char *ANumber, const char *AMsg) {
    uint32_t FTimer;
    Delay.Reset(&FTimer);
    // Build command: surround number with double quotes
    uint8_t L = strlen(ANumber);
    strcpy(TxLine, "AT+CMGS=\"");
    strcpy(&TxLine[9], ANumber);
    TxLine[9+L]  = '\"';
    TxLine[10+L] = 0;
    // Send cmd
    SendString(TxLine);
    Delay.ms(27);
    // Send message
    while (*AMsg != 0) {
        USART1->DR = *AMsg++;
        while (!(USART1->SR & USART_FLAG_TC));
    }
    // Send Ctrl-Z
    USART1->DR = 26;
    // Wait for echo
    while (!Delay.Elapsed(&FTimer, MDM_SMS_TIMEOUT)) {
        if (NewLineReceived) {
            NewLineReceived = false;
            if      (strcmp (RxLine, "OK")    == 0) return erOk;
            else if (strcmp (RxLine, "ERROR") == 0) return erError;
        } // if Echo rcvd
    } // while
    return erError;
}


// ========================== Commands and strings =============================
void sim900_t::SendString(const char *S) {
    //UART_Print('>');
    while (*S != 0) {
        //UART_Print(*S);
        USART1->DR = *S++;
        while (!(USART1->SR & USART_FLAG_TC));
    }
    USART1->DR = '\r';   // Send CR (13 or 0x0D)
    while (!(USART1->SR & USART_FLAG_TC));
}

Error_t sim900_t::Command(const char *ACmd) {
    Delay.ms(99);   // to prevent transmission corruption
    strcpy(TxLine, ACmd);
    uint32_t Tmr;
    Delay.Reset(&Tmr);
    ResetRxLine();
    SendString(ACmd);
    // Wait for reply
    while (!Delay.Elapsed(&Tmr, MDM_RX_TIMEOUT)) {
        if (NewLineReceived) {
            if      (strncmp(RxLine, "AT", 2) == 0) ResetRxLine(); // Echo, wait next line
            else if (strcmp (RxLine, "OK")    == 0) return erOk;
            else if (strcmp (RxLine, "ERROR") == 0) return erError;
            else return erOther;
        }
    } // while
    return erTimeout;
}

Error_t sim900_t::ProcessSim() {
    uint32_t Tmr, TryCounter = 4;
    Delay.Reset(&Tmr);
    while (!Delay.Elapsed(&Tmr, MDM_SIM_TIMEOUT) and (TryCounter-- != 0)) {
        if (Command("AT+CPIN?") == erOther) {   // ERROR or not ERROR
            if (strcmp (RxLine, "+CPIN: READY") == 0) {
                Com.Printf("Sim Ok\r");
                return erOk;
            }
        }
        Delay.ms(450);
    } // while !elapsed
    Com.Printf("Sim error\r");
    return erTimeout;
}

Error_t sim900_t::NetRegistration() {
    uint32_t FTimer;
    Delay.Reset(&FTimer);
    while (!Delay.Elapsed(&FTimer, MDM_NETREG_TIMEOUT)) {
        NetState = nsUnknown;
        if (Command("AT+CREG?") == erOther) {   // ERROR or not ERROR
            if (strncmp(RxLine, "+CREG:", 6) == 0) {    // Parse reply
                char c = RxLine[9];
                uint8_t k = c - '0';    // convert char to digit
                if ((k>=0) && (k<=5)) NetState = (NetState_t)k;
                switch (NetState) {
                    case nsRegistered:
                    case nsRegRoaming:
                        Com.Printf("Net connected\r");
                        return erOk;
                        break;
                    case nsRegDenied:
                        return erError;
                        break;
                    case nsNotRegNoSearch:
                    case nsUnknown:     // Not answered yet
                    case nsSearching:   // Searching
                        break;
                } // switch
            } // if needed reply
        } // if reply to cmd
        Delay.ms(1800);
    } // while !elapsed
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
    if (b == '\n') return;  // ignore LF
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
