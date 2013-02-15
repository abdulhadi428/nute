/*
 * kl_sd.cpp
 *
 *  Created on: 13.02.2013
 *      Author: kreyl
 */

#include "kl_sd.h"
#include "sdc.h"
#include "sdc_lld.h"
#include <string.h>
#include "kl_lib_f2xx.h"

sd_t SD;

void sd_t::Init() {
    IsReady = FALSE;

    // Bus pins
    PinSetupAlterFunc(GPIOC,  8, omPushPull, pudPullUp, AF12, ps25MHz);
    PinSetupAlterFunc(GPIOC,  9, omPushPull, pudPullUp, AF12, ps25MHz);
    PinSetupAlterFunc(GPIOC, 10, omPushPull, pudPullUp, AF12, ps25MHz);
    PinSetupAlterFunc(GPIOC, 11, omPushPull, pudPullUp, AF12, ps25MHz);
    PinSetupAlterFunc(GPIOC, 12, omPushPull, pudNone,   AF12, ps25MHz);
    PinSetupAlterFunc(GPIOD,  2, omPushPull, pudPullUp, AF12, ps25MHz);
    // Power pin
    PinSetupOut(GPIOC, 4, omPushPull, pudNone);
    PinClear(GPIOC, 4); // Power on
    Delay_ms(450);

    FRESULT err;
    sdcInit();
    sdcStart(&SDCD1, NULL);
    if (sdcConnect(&SDCD1)) {
        Uart.Printf("SD connect error\r");
        return;
    }
    else {
        Uart.Printf("SD capacity: %u\r", SDCD1.capacity);
    }

    err = f_mount(0, &SDC_FS);
    if (err != FR_OK) {
        Uart.Printf("SD mount error\r");
        sdcDisconnect(&SDCD1);
        return;
    }
    IsReady = TRUE;

//    FRESULT rslt;
//    FIL IFile;
//    // Open file
//    rslt = f_open(&IFile, "settings.ini", FA_READ+FA_OPEN_EXISTING);
//    Uart.Printf("OpenFile: %u\r", (uint8_t)rslt);
}

// ========================== ini files operations =============================
#ifdef USE_INI_FILES
#include <stdlib.h>

#define INI_BUF_SIZE    512
char IBuf[INI_BUF_SIZE];
char FBuf[64];

// ==== Inner use ====
static char* skipleading(char *S) {
    while (*S != '\0' && *S <= ' ') S++;
    return (char*)S;
}
static char* skiptrailing(char *S, const char *base) {
    while ((S > base) && (*(S-1) <= ' ')) S--;
    return (char*)S;
}
static char* striptrailing(char *S) {
    char *ptr = skiptrailing(strchr(S, '\0'), S);
    *ptr='\0';
    return (char*)S;
}

// ======================== Implementation =====================================
bool iniReadString(const char *ASection, const char *AKey, const char *AFileName, char *AOutput, uint32_t AMaxLen) {
    FRESULT rslt;
    // Open file
    rslt = f_open(&SD.File, AFileName, FA_READ+FA_OPEN_EXISTING);
    if (rslt != FR_OK) {
        Uart.Printf(AFileName);
        if (rslt == FR_NO_FILE) Uart.Printf(": file not found\r");
        else Uart.Printf(": openFile error: %u", rslt);
        return false;
    }
    // Check if zero file
    if (SD.File.fsize == 0) {
        f_close(&SD.File);
        Uart.Printf("Empty file\r");
        return false;
    }

    // Move through file one line at a time until a section is matched or EOF.
    char *StartP, *EndP;
    uint32_t len = strlen(ASection);
    do {
        if (!f_gets(IBuf, INI_BUF_SIZE, &SD.File)) {
            f_close(&SD.File);
            return false;
        }
        StartP = skipleading(IBuf);
        EndP = strchr(StartP, ']');
    } while ((*StartP != '[') or (EndP == NULL) or ((uint32_t)(EndP-StartP-1) != len or strnicmp(StartP+1, ASection, len) != 0));

    // Section found, find the key
    len = strlen(AKey);
    do {
        if (!f_gets(IBuf, INI_BUF_SIZE, &SD.File) or *(StartP = skipleading(IBuf)) == '[') {
            f_close(&SD.File);
            return false;
        }
        StartP = skipleading(IBuf);
        EndP = strchr(StartP, '='); /* Parse out the equal sign */
    } while ((*StartP == ';') or (*StartP == '#') or (EndP == NULL) or ((uint32_t)(skiptrailing(EndP, StartP)-StartP) != len or strnicmp(StartP, AKey, len) != 0));

    // Copy up to ALength chars to AOutput
    StartP = skipleading(EndP + 1);
    // Remove a trailing comment
    uint8_t isstring = 0;
    for (EndP = StartP; (*EndP != '\0') and (((*EndP != ';') and (*EndP != '#')) or isstring) and ((uint32_t)(EndP - StartP) < AMaxLen); EndP++) {
        if (*EndP == '"') {
            if (*(EndP + 1) == '"') EndP++;     // skip "" (both quotes)
            else isstring = !isstring; // single quote, toggle isstring
        }
        else if (*EndP == '\\' && *(EndP + 1) == '"') EndP++; // skip \" (both quotes)
    } // for
    *EndP = '\0';   // Terminate at a comment
    striptrailing(StartP);
    strcpy(AOutput, StartP);
    f_close(&SD.File);
    return &AOutput[EndP-StartP-2]; // Pointer to last '\0'
}

bool iniReadInt32 (const char *ASection, const char *AKey, const char *AFileName, int32_t *AOutput) {
    if (iniReadString(ASection, AKey, AFileName, FBuf, 64)) {
        *AOutput = strtol(FBuf, NULL, 10);
        return true;
    }
    else return false;
}
bool iniReadUint32(const char *ASection, const char *AKey, const char *AFileName, uint32_t *AOutput) {
    if (iniReadString(ASection, AKey, AFileName, FBuf, 64)) {
        *AOutput = strtol(FBuf, NULL, 10);
        return true;
    }
    else return false;
}

#endif

// ============================== Hardware =====================================
#ifdef __cplusplus
extern "C" {
#endif

bool_t sdc_lld_is_card_inserted(SDCDriver *sdcp) {
    (void)sdcp;
    return TRUE;
}

bool_t sdc_lld_is_write_protected(SDCDriver *sdcp) {
    (void)sdcp;
    return FALSE;
}

#ifdef __cplusplus
}
#endif
