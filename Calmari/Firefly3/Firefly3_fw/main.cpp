/*
 * File:   main.cpp
 * Author: Kreyl
 * Project: Armlet2South
 *
 * Created on Feb 05, 2013, 20:27
 */

#include "kl_lib_f100.h"
#include "ch.h"
#include "hal.h"
#include "adc_f100.h"
#include "cmd_uart_f10x.h"
#include "cmd.h"
#include "led_rgb_f100.h"
#include "LedSequence.h"

#define LED_EMPTY_COLOR           ((Color_t){4, 4, 4})
LedSequence_t Seq;

void OnUartCmd();

#define AUTO_OFF    FALSE

#if AUTO_OFF
Adc_t Adc;
IWDG_t Iwdg;
#define ADC_VALUE_TO_OFF    540
#define ADC_VALUE_TO_ON     198
static inline void GoSleep();
#endif

#if 1 // ========================== Flash ======================================
#define FLASH_PAGE_SIZE     1024
#define FLASH_KEY1          ((uint32_t)0x45670123)
#define FLASH_KEY2          ((uint32_t)0xCDEF89AB)
#define CR_LOCK_Set         ((uint32_t)0x00000080)
#define CR_PER_Set          ((uint32_t)0x00000002)
#define CR_PER_Reset        ((uint32_t)0x00001FFD)
#define CR_STRT_Set         ((uint32_t)0x00000040)
#define CR_PG_Set           ((uint32_t)0x00000001)
#define CR_PG_Reset         ((uint32_t)0x00001FFE)
#define EraseTimeout        ((uint32_t)0x000B0000)
#define ProgramTimeout      ((uint32_t)0x00002000)

const uint32_t MyBigUint __attribute__ ((section("MyFlash"), aligned(FLASH_PAGE_SIZE))) = 0;

#endif

#if 1 // =========================== Main ======================================
int main(void) {
    // ==== Init clock system ====
    Clk.SetupBusDividers(ahbDiv4, apbDiv1, apbDiv1);
    Clk.UpdateFreqValues();
    // ==== Init OS ====
    halInit();
    chSysInit();
    // ==== Init Hard & Soft ====
    JtagDisable();
    Uart.Init(115200);
    Led.Init();
#if AUTO_OFF
    // Set white and print info only when switch on, not after watcdog reset.
    if(!Iwdg.ResetOccured()) {
#endif
    Uart.Printf("\rFirefly3  AHB=%u; APB1=%u; APB2=%u\r\n", Clk.AHBFreqHz, Clk.APB1FreqHz, Clk.APB2FreqHz);
    Led.SetColor(LED_EMPTY_COLOR);
#if AUTO_OFF
    }
    Adc.Init();
    while(true) {
        chThdSleepMilliseconds(450);

        uint32_t rslt = 0;
        Adc.Enable();
        Adc.Calibrate();
        for(uint8_t i=0; i<8; i++) {
            Adc.StartConversion();
            while(!Adc.ConversionCompleted()) chThdSleepMilliseconds(20);
            rslt += Adc.Result();
        }
        Adc.Disable();
        rslt >>= 3;
        Uart.Printf("Adc: %u\r", rslt);
        if(rslt > ADC_VALUE_TO_OFF) Led.SetColorSmoothly(clBlack);
        if(rslt < ADC_VALUE_TO_ON)  Led.SetColorSmoothly(LED_COLOR);
        // Check if sleep
        if(Led.IsOff()) GoSleep();
    }
#else
    while(true) {
        chThdSleepMilliseconds(99);
        Uart.PollRx(&OnUartCmd);
    }
#endif
}
#endif

// ==== Uart cmd ====
#if UART_RX_ENABLED
void OnUartCmd() {
    UartCmd_t *PCmd = &Uart.Cmd;
    __attribute__((unused)) int32_t dw32 = 0;  // May be unused in some configurations
//    Uart.Printf("\r%S\r", PCmd->Name);
    // Handle command
    if(PCmd->NameIs("#Ping")) Uart.Ack(OK);

    else if(PCmd->NameIs("#ffGet")) {
        Uart.Printf("#ff");
        for(uint32_t i=0; i < Seq.Cnt; i++) {
            LedChunk_t *PChunk = &Seq.Chunk[i];
            switch(PChunk->ChunkSort) {
                case csSetColor:
                    Uart.Printf(",RGB,%u,%u,%u,%u", PChunk->Color.R, PChunk->Color.G, PChunk->Color.B, PChunk->MorphMS);
                    break;
                case csWait:
                    Uart.Printf(",Wait,%u",PChunk->Time_ms);
                    break;
                case csGoto:
                    Uart.Printf(",Goto,%u",PChunk->ChunkToJumpTo);
                    break;
                case csEnd:
                    Uart.Printf(",End");
                    break;
            }
        }
        Uart.Printf("\r\n");
    }

    else if(PCmd->NameIs("#ffSet")) {
        Led.Stop();
        Seq.Reset();
        LedChunk_t *PChunk;
        while(PCmd->GetNextToken() == OK) {
            if(PCmd->TokenIs("RGB")) {  // Read R, G, B
                PChunk = &Seq.Chunk[Seq.Cnt++];
                PChunk->ChunkSort = csSetColor;
                // Color
                if(PCmd->GetNextTokenAndConvertToUint8(&PChunk->Color.R) != OK) { Uart.Ack(CMD_ERROR); return; }
                if(PCmd->GetNextTokenAndConvertToUint8(&PChunk->Color.G) != OK) { Uart.Ack(CMD_ERROR); return; }
                if(PCmd->GetNextTokenAndConvertToUint8(&PChunk->Color.B) != OK) { Uart.Ack(CMD_ERROR); return; }
                // MorphMS
                if(PCmd->GetNextTokenAndConvertToInt32(&PChunk->MorphMS) != OK) { Uart.Ack(CMD_ERROR); return; }
            }

            if(PCmd->TokenIs("Wait")) {
                PChunk = &Seq.Chunk[Seq.Cnt++];
                PChunk->ChunkSort = csWait;
                if(PCmd->GetNextTokenAndConvertToInt32(&PChunk->Time_ms) != OK) { Uart.Ack(CMD_ERROR); return; }
            }

            if(PCmd->TokenIs("Goto")) {
                PChunk = &Seq.Chunk[Seq.Cnt++];
                PChunk->ChunkSort = csGoto;
                if(PCmd->GetNextTokenAndConvertToInt32(&PChunk->ChunkToJumpTo) != OK) { Uart.Ack(CMD_ERROR); return; }
            }

            if(Seq.Cnt >= LED_CHUNK_CNT-1) { Uart.Ack(CMD_ERROR); return; }
        } // while GetNextToken
        // Add final token if not end or goto
        PChunk = &Seq.Chunk[Seq.Cnt++];
        if(!(PChunk->ChunkSort == csGoto or PChunk->ChunkSort == csEnd)) {
            PChunk->ChunkSort = csEnd;
        }
        Led.StartSequence(&Seq.Chunk[0]);
        Uart.Ack(OK);
    }

    else if(*PCmd->Name == '#') Uart.Ack(CMD_UNKNOWN);  // reply only #-started stuff
}
#endif

#if AUTO_OFF
void GoSleep() {
    // Start LSI
    Clk.LsiEnable();
    // Start IWDG
    Iwdg.SetTimeout(4500);
    Iwdg.Enable();
    // Enter standby mode
    SCB->SCR |= SCB_SCR_SLEEPDEEP;
    PWR->CR = PWR_CR_PDDS;
    PWR->CR |= PWR_CR_CWUF;
    __WFI();
}
#endif

// ==== Flash load/save ====



void FLASH_Unlock() {
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;
}
void FLASH_Lock(void) { FLASH->CR |= CR_LOCK_Set; }
void FLASH_ClearFlag(uint32_t FLASH_FLAG) { FLASH->SR = FLASH_FLAG; }

uint8_t FLASH_GetBank1Status(void) {
    if(FLASH->SR & FLASH_SR_BSY) return BUSY;
    else if(FLASH->SR & FLASH_SR_PGERR) return FAILURE;
    else if(FLASH->SR & FLASH_SR_WRPRTERR) return FAILURE;
    else return OK;
}
uint8_t FLASH_WaitForLastOperation(uint32_t Timeout) {
    uint8_t status = OK;
    // Wait for a Flash operation to complete or a TIMEOUT to occur
    do {
        status = FLASH_GetBank1Status();
        Timeout--;
    } while((status == BUSY) and (Timeout != 0x00));
    if(Timeout == 0x00) status = TIMEOUT;
    return status;
}

uint8_t FLASH_ErasePage(uint32_t PageAddress) {
    uint8_t status = FLASH_WaitForLastOperation(EraseTimeout);
    if(status == OK) {
        FLASH->CR |= CR_PER_Set;
        FLASH->AR = PageAddress;
        FLASH->CR |= CR_STRT_Set;
        // Wait for last operation to be completed
        status = FLASH_WaitForLastOperation(EraseTimeout);
        // Disable the PER Bit
        FLASH->CR &= CR_PER_Reset;
    }
    return status;
}

uint8_t FLASH_ProgramWord(uint32_t Address, uint32_t Data) {
    uint8_t status = FLASH_WaitForLastOperation(ProgramTimeout);
    if(status == OK) {
        FLASH->CR |= CR_PG_Set; // program the new first half word
        *(__IO uint16_t*)Address = (uint16_t)Data;
        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation(ProgramTimeout);
        if(status == OK) {
            // program the new second half word
            uint32_t tmp = Address + 2;
            *(__IO uint16_t*)tmp = Data >> 16;
            /* Wait for last operation to be completed */
            status = FLASH_WaitForLastOperation(ProgramTimeout);
            /* Disable the PG Bit */
            FLASH->CR &= CR_PG_Reset;
        }
        else FLASH->CR &= CR_PG_Reset;  // Disable the PG Bit
    }
    return status;
}


void Load(Color_t *PClr) {
}

void Save(Color_t *PClr) {
    uint8_t status = OK;
    uint32_t FAddr = (uint32_t)&MyBigUint;
    FLASH_Unlock();
    // Erase flash
    FLASH_ClearFlag(FLASH_SR_EOP | FLASH_SR_PGERR | FLASH_SR_WRPRTERR);   // Clear All pending flags
    status = FLASH_ErasePage(FAddr);
    //klPrintf("  Flash erase %u: %u\r", i, FLASHStatus);
    if(status != OK) {
        Uart.Printf("  Flash erase error\r");
        return;
    }
    uint32_t *PRAM = (uint32_t*)PClr;    // What to write
    uint32_t DataWordCount = (sizeof(Color_t) + 3) / 4;
    chSysLock();
    for(uint32_t i=0; i<DataWordCount; i++) {
        status = FLASH_ProgramWord(FAddr, *PRAM);
        if(status != OK) {
            Uart.Printf("  Flash write error\r");
            return;
        }
        //else klPrintf("#");
        FAddr += 4;
        PRAM++;
    }
    FLASH_Lock();
    chSysUnlock();
}
