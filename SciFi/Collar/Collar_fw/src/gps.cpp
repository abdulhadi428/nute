/*
 * gps.cpp
 *
 *  Created on: 27.05.2012
 *      Author: kreyl
 */

#include "gps.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_dma.h"
#include "string.h"
#include "stdlib.h"

GpsR_t Gps;
extern Situation_t *Situation;

void GpsR_t::Init() {
    // ==== Clocks init ====
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);      // UART clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    // ==== GPIO init ====
    klGpioSetupByN(GPIOA, 3, GPIO_Mode_IN_FLOATING); // RX2
    // ==== USART configuration ====
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx;
    USART_Init(USART2, &USART_InitStructure);

    // ==== DMA ====
    DMA_InitTypeDef DMA_InitStructure;
    DMA_DeInit(DMA1_Channel6);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &USART2->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t) &IBuf[0];
    DMA_InitStructure.DMA_BufferSize         = GPS_BUF_SIZE;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel6, &DMA_InitStructure);
    // Start DMA
    USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
    DMA_Cmd(DMA1_Channel6, ENABLE);   // Enable DMA channel
    // Enable USART
    USART_Cmd(USART2, ENABLE);
    NewMeasurement = false;
}

/*
$PMTK011,MTKGPS*08
$PMTK010,001*2E
$GPGGA,235945.037,,,,,0,0,,,M,,M,,*40
$GPGLL,,,,,235945.037,V,N*72
$GPGSA,A,1,,,,,,,,,,,,,,,*1E
$GPGSV,1,1,00*79
$GPRMC,235945.037,V,,,,,0.00,0.00,050180,,,N*49
$GPVTG,0.00,T,,M,0.00,N,0.00,K,N*32
$GPZDA,235945.037,05,01,1980,,*5A
*/

enum MsgState_t {msFindS, msCheckHdr, msReadData, msCompleted};

void GpsR_t::Task() {
    static uint8_t LetterCounter=0;
    static MsgState_t MsgState = msFindS;
    static char *PMsg;

    if (DMA_GetFlagStatus(DMA1_FLAG_TC6)) {
        DMA_ClearFlag(DMA1_FLAG_TC6);
        char FBuf[GPS_BUF_SIZE];
        memcpy(FBuf, IBuf, GPS_BUF_SIZE);
        //klPrintf("Buf: \r%S\r", FBuf);
        // Parse buffer
        char *S = &FBuf[0];
        //klPrintf("0:%c\r", *S);
        bool DoSpin = true;
        while(DoSpin) {
            switch (MsgState) {
                case msFindS:   // Find $
                    //klPrintf("1\r");
                    S = strchr(S, '$');
                    // Check if found
                    if (S != 0) MsgState = msCheckHdr;
                    else DoSpin = false;
                    break;

                case msCheckHdr: // Iterate letters
                    //klPrintf("%c\r", *S);
                    if (*S == MsgHdr[LetterCounter++]) {
                        // Check if header search completed
                        if (LetterCounter >= (MsgHdrSz-1)) {
                            MsgState = msReadData;
                            PMsg = &IMsg[0];    // Set pointer to start of msg string
                            LetterCounter = 0;
                        }
                    }
                    else {  // not a needed header, reset search
                        MsgState = msFindS;
                        LetterCounter = 0;
                    }
                    break;

                case msReadData:
                    //klPrintf("r %c\r", *S);
                    if (*S == '\n') {   // end of message
                        MsgState = msCompleted;
                    }
                    else *PMsg++ = *S;
                    break;

                case msCompleted:
                    IParseGPGGAMsg();
                    MsgState = msFindS;
                    break;
            } // switch
            // Check if last
            if (S != &FBuf[GPS_BUF_SIZE-1]) S++;
            else DoSpin = false; // Last char
        } // while spin
    } // if get flag status
}

// Convert N numeric symbols, or till ','. Fill with zeroes until N.
int GpsStrnToInt(char **InputS, uint8_t N, bool AppendZeroes) {
    char Token[11], *S = *InputS;
    uint8_t i=0;
    // Copy to token
    while ((i < N) and (*S != ',')) {
        Token[i++] = *S++;
        if (*S == '.') S++;
    }
    if (AppendZeroes and (i < N-1)) {
        memset(&Token[i], '0', N-i);
        i = N-1;
    }
    Token[i] = 0;
    *InputS = S;
    //klPrintf("%S\r", Token);
    return atoi(Token);
}

/*
GPGAA,hhmmss.ss,ddmm.mmmm,n,dddmm.mmmm,e,q,ss,y.y,a.a,z,g.g,z,t.t,iii*CC

hhmmss.ss    : Coordinated Universal Time (UTC), also known as GMT
ddmm.mmmm,n  : Latitude in degrees, minutes and cardinal sign
dddmm.mmmm,e : Longitude in degrees, minutes and cardinal sign
q            : Quality of fix.  1 = there is a fix
ss           : Number of satellites being used
y.y          : Horizontal dilution of precision
a.a,M        : GPS antenna altitude in meters

GPGGA,181058.000,5542.2389,N,03741.60637,E,1,8,1.34,115.0,M,14.6,M,,*5
GPGGA,235947.036,,,,,0,0,,,M,,M,,*43
*/

void GpsR_t::IParseGPGGAMsg() {
    CmdUnit.Printf("Msg: %S\r", IMsg);
    char *S = &IMsg[0];
    uint32_t tmp1=0, tmp2;
    // Remove leading ','
    while(*S == ',') S++;
    // ==== Parse time ====
    Situation->Time.H = GpsStrnToInt(&S, 2, false);
    Situation->Time.M = GpsStrnToInt(&S, 2, false);
    Situation->Time.S = GpsStrnToInt(&S, 2, false);
    //CmdUnit.Printf("Time: %u2:%u2:%u2\r", Situation->Time.H, Situation->Time.M, Situation->Time.S);
    while(*S++ != ',');                     // move to next token
    // ==== Lattitude ====
    if (*S != ',') {                        // Data exists
        tmp1 = GpsStrnToInt(&S, 2, false);  // Degrees
        tmp1 *= 10000000;
        tmp2  = GpsStrnToInt(&S, 8, true);  // Minutes * 10^6, 8 places total
        tmp2 /= 6;                          // = Minutes * 10^7 / 60
        tmp1 += tmp2;
    }
    S++;
    // North or south
    if (*S != 'N') tmp1 = -tmp1;            // Lattitude is negative at south
    if (*S != ',') S++;                     // *S is N, S or ','
    S++;                                    // Move to next token
    Situation->Latitude = tmp1;
    // ==== Longtitude ====
    if (*S != ',') {                        // Data exists
        tmp1 = GpsStrnToInt(&S, 3, false);  // Degrees
        tmp1 *= 10000000;
        tmp2  = GpsStrnToInt(&S, 8, true);  // Minutes * 10^6, 8 places total
        tmp2 /= 6;                          // = Minutes * 10^7 / 60
        tmp1 += tmp2;
    }
    S++;
    // East or west
    if (*S != 'E') tmp1 = -tmp1;
    if (*S != ',') S++;                     // *S is E, W or ','
    S++;                                    // Move to next token
    Situation->Longitude = tmp1;
    // Get position fix and sattelites count
    Situation->IsFixed = (*S != '0');
    S+=2;                                   // Move to next token
    Situation->SatCount = GpsStrnToInt(&S, 2, false);
    // Precision
    if (Situation->IsFixed) {
        S++;
        Situation->IsFixed = true;
        Situation->Precision = GpsStrnToInt(&S, 7, false);
        NewMeasurement = true;
        //CmdUnit.Printf("Lat: %i; Lng: %i; SatCount: %u; Precision: %u\r", Situation->Latitude, Situation->Longitude, Situation->SatCount, Situation->Precision);
    }
}
