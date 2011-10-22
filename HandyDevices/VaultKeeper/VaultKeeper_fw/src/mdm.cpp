/*
 * mdm.cpp
 *
 *  Created on: 12.10.2011
 *      Author: Kreyl
 *
  */
#include "stm32f10x_rcc.h"
#include "arm_common.h"
#include "mdm.h"
#include "delay_util.h"
#include <stdlib.h>
#include "uart.h"

#define MDM_VERBOSE_IO

mdm_t Mdm;

void mdm_t::Init() {
    GPIOInit();
    PwrKeyHi();
    //BufLength = 0;
    USARTInit();

    // ==== Setup modem ====
    State = erBusy;
    Delay.ms(1008);
    Error_t r = erError;
    while (r != erOk) {     // Try to setup modem
        fRDY = false;
        while (!fRDY) {     // Cycle PwrKey and send AT
            UART_PrintString("#\r");
            fRDY = false;
            PwrKeyLo();
            DisableRxIrq();
            // Init variables
            fCFUN = 0;
            SimState = ssUnknown;
            NetState = nsUnknown;
            Cmd[0] = 0;
            // Wait
            Delay.ms(540);
            PwrKeyHi();
            EnableRxIrq();
            Delay.ms(540);
            for(uint8_t i=0; i<4; i++) {
                SendString("AT");
                Delay.ms(7);
                if (fRDY) break;
            }
        } // while !RDY
        Delay.ms(720);
        UART_PrintString("== Init ==\r");
        // Send initialization commands
        if((r = Command("AT+IPR=9600")) != erOk) continue;  // Setup speed
        if((r = Command("ATX0"))        != erOk) continue;  // CONNECT result code returned only
        if(ProcessSIM()             != erOk) continue;  // Wait for SIM to activate
        if(WaitForNetRegistration() != erOk) continue;  // Wait for modem to connect

    } // while r
    UART_PrintString("Modem is ready\r");
    State = erOk;
}

// ========================== Card, call, sms =================================
void mdm_t::SendSMSWithTime(const char* AStrNumber, const char *AMsg) {

}

Error_t mdm_t::ProcessSIM() {
    Error_t r;
    Delay.Reset(&Timer);
    while (!Delay.Elapsed(&Timer, MDM_SIM_TIMEOUT)) {
        SimState = ssUnknown;
        if ((r = Command("AT+CPIN?")) != erOk) return r;
        switch (SimState) {
            case ssReady:
                return erOk;
                break;
            default: break;
        }
        Delay.ms(450);
    } // while !elapsed
    return erTimeout;;
}

Error_t mdm_t::WaitForNetRegistration() {
    Error_t r;
    Delay.Reset(&Timer);
    while (!Delay.Elapsed(&Timer, MDM_NETREG_TIMEOUT)) {
        NetState = nsUnknown;
        if ((r = Command("AT+CREG?")) != erOk) return r;
        switch (NetState) {
            case nsRegistered:
            case nsRegRoaming:
                return erOk;
                break;
            case nsNotRegNoSearch:
            case nsRegDenied:
                return erError;
                break;
            case nsUnknown:     // Not answered yet
            case nsSearching:   // Searching
                break;
        } // switch
        Delay.ms(1800);
    } // while !elapsed
    return erTimeout;
}

// =========================== AT commands ====================================
Error_t mdm_t::Command(const char *ACmd) {
    strcpy(Cmd, ACmd);  // Valid echo needed
    do {
        fEchoRcvd = false;
        fReply = flNone;
        SendString(ACmd);
        for (uint32_t i=0; i<MDM_RX_TIMEOUT; i++) {
            Delay.ms(4);
            if (fEchoRcvd) {    // Check fReply only if valid echo received
                if (fReply == flRepeatNeeded) Delay.ms(108);
                if (fReply != flNone) break;    // Get out of for()
            } // if Echo rcvd
        } // for
    } while(fReply == flRepeatNeeded);
    // Do not wait echo now
    Cmd[0] = 0;
    switch (fReply) {
        case flOk:   return erOk; break;
        case flNone: return erTimeout; break;
        default:     return erError; break;
    }
}

void mdm_t::SendString(const char *S) {
    //UART_Print('>');
    while (*S != '\0') {
        //UART_Print(*S);
        USART_SendData(USART2, *S++);
        // Loop until the end of transmission
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
    }
    USART_SendData(USART2, '\r');   // Send CR (13 or 0x0D)
}

void mdm_t::ProcessRxLine(void) {
    //UART_PrintString(Line);
    //UART_NewLine();
    char *S, c;
    uint32_t k;
    // Check RDY
    if (fRDY == false) {
        if (strcmp(Line, "RDY") == 0) {
            UART_PrintString("> RDY rcvd\r");
            fRDY = true;
        }
        return; // only RDY is an option
    }
    // Check if echo
    if (strncmp(Line, "AT", 2) == 0) {
        //UART_PrintString("> Echo\r");
        if (Cmd[0] != 0) {  // waiting for echo
            if (strcmp(Line, Cmd) == 0) {
                fEchoRcvd = true;
                fReply = flNone;
            }
        }
        return;
    }
    // Check OK/ERROR
    if (fReply == flNone) {
        if (strcmp(Line, "OK") == 0) {
            //UART_PrintString("> OK rcvd\r");
            fReply = flOk;
            return;
        }
        if (strcmp(Line, "ERROR") == 0) {
            //UART_PrintString("> ERR rcvd\r");
            fReply = flError;
            return;
        }
        if (strncmp(Line, "+CME ERROR:", 11) == 0) {
            S = &Line[12];  // first char of number
            k = atoi(S);
            UART_StrUint(" cme err=", k);
            ProcessCMEError(k);
            return;
        }

    } // if flNone
    // ==== Other stuff ====
    // Functionality
    if (strncmp(Line, "+CFUN:", 6) == 0) {
        //UART_PrintString("> CFUN\r");
        c = Line[7];
        k = c - 48; // convert char to digit
        if ((k==0) || (k==1) || (k==4)) {
            fCFUN = (uint8_t)k;
            UART_StrUint(" cfun=", k);
        }
        return;
    }

    else if (strncmp(Line, "+CPIN:", 6) == 0) ParseCPIN(&Line[7]);      // SIM state
    // Network registration: "+CREG: n,s"
    else if (strncmp(Line, "+CREG:", 6) == 0) {
        //UART_PrintString("> CFUN\r");
        c = Line[9];    // <stat>
        k = c - 48; // convert char to digit
        if ((k>=0) && (k<=5)) {
            NetState = (NetState_t)k;
            UART_StrUint(" NetState=", k);
        }
    }
}

// Parsing
void mdm_t::ProcessCMEError(uint32_t AErr) {
    switch (AErr) {
        // SIM
        case 10:    // SIM not inserted
        case 11:    // SIM failure
            fReply = flError;
            break;
        case 14:    // SIM busy
            fReply = flRepeatNeeded;
            break;

        default: break;
    } // switch
}

void mdm_t::ParseCPIN(char *S) {
    if (strcmp(S, "READY") == 0) SimState = ssReady;
    else if (strcmp(S, "SIM PIN") == 0) SimState = ssPinNeeded;
    else if (strcmp(S, "SIM PUK") == 0) SimState = ssPukNeeded;
    else SimState = ssUnknown;
}

// ================================ Low-level =================================
void mdm_t::GPIOInit() {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    // PA0=CTS, PA1=RTS, PA2=TX, PA3=RX, PA4=PWR_KEY
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    // Configure USART Tx and RTS as alternate function push-pull
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2 | GPIO_Pin_1;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // Configure USART Rx and CTS as input floating
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_3;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // Configure PWR_KEY as push-pull output
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void mdm_t::USARTInit() {
    // Clocks
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    // USART
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    //USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_RTS_CTS;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
    // Enable USART
    USART_Cmd(USART2, ENABLE);
    // Setup interrupt
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

// Interrupt
void mdm_t::IRQHandler() {
    if(USART_GetITStatus(USART2, USART_IT_RXNE) == RESET) return;
    // Read one byte from the receive data register
    uint8_t b = USART_ReceiveData(USART2);
#ifdef MDM_VERBOSE_IO
    USART_SendData(USART1, b);
#endif
    // Process the byte
    if (b == '\n') return;  // ignore LF
    if (b == '\r') {
        if (CharCounter != 0) {
            Line[CharCounter] = 0;  // \0 at line end
            // Process received line
            ProcessRxLine();
            // Reset line
            CharCounter = 0;
        }
        return;
    }
    Line[CharCounter++] = b;
}
void USART2_IRQHandler(void) {
    Mdm.IRQHandler();
}
