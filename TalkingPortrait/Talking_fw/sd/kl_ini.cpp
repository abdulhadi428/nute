/*
 * kl_ini.cpp
 *
 *  Created on: 03.07.2011
 *      Author: Kreyl
 */

#include "kl_ini.h"
#include <string.h>
#include <stdlib.h>

#include "uart.h"

char IBuf[INI_BUF_SIZE];

char* ReadString(const char *ASection, const char *AKey, char *AOutput, uint32_t AMaxLength, const char *AFileName) {
    FRESULT rslt;
    FIL IFile;
    // Open file
    rslt = f_open(&IFile, AFileName, FA_READ+FA_OPEN_EXISTING);
    if (rslt != FR_OK) {
        klPrintf(AFileName);
        if (rslt == FR_NO_FILE) klPrintf("File not found\r");
        else klPrintf("OpenFile error: %u", rslt);
        return NULL;
    }
    // Check if zero file
    if (IFile.fsize == 0) {
        f_close(&IFile);
        klPrintf("Empty file\r");
        return NULL;
    }

    // Move through file 1 line at a time until a section is matched or EOF.
    char *StartP, *EndP;
    int16_t len = strlen(ASection);
    do {
        if (!f_gets(IBuf, INI_BUF_SIZE, &IFile)) {
            f_close(&IFile);
            return NULL;
        }
        StartP = skipleading(IBuf);
        EndP = strchr(StartP, ']');
    } while (*StartP != '[' || EndP == NULL || ((int)(EndP-StartP-1) != len || strnicmp(StartP+1, ASection, len) != 0));

    // Section found, find the key
    len = strlen(AKey);
    do {
        if (!f_gets(IBuf, INI_BUF_SIZE, &IFile) || *(StartP = skipleading(IBuf)) == '[') {
            f_close(&IFile);
            return NULL;
        }
        StartP = skipleading(IBuf);
        EndP = strchr(StartP, '='); /* Parse out the equal sign */
    } while (*StartP == ';' || *StartP == '#' || EndP == NULL || ((int)(skiptrailing(EndP, StartP)-StartP) != len || strnicmp(StartP, AKey, len) != 0));

    // Copy up to AMaxLength chars to AOutput
    StartP = skipleading(EndP + 1);
    // Remove a trailing comment
    uint8_t isstring = 0;
    for (EndP = StartP; *EndP != '\0' && ((*EndP != ';' && *EndP != '#') || isstring); EndP++) {
        if (*EndP == '"') {
            if (*(EndP + 1) == '"') EndP++;     // skip "" (both quotes)
            else isstring = !isstring; // single quote, toggle isstring
        }
        else if (*EndP == '\\' && *(EndP + 1) == '"') EndP++; // skip \" (both quotes)
    } // for
    *EndP = '\0';   // Terminate at a comment
    striptrailing(StartP);
    strcpy(AOutput, StartP);
    f_close(&IFile);
    return &AOutput[EndP-StartP-2]; // Pointer to last '\0'
}

int32_t ReadInt32(const char *ASection, const char *AKey, const char *AFileName) {
    char FBuf[64];
    ReadString(ASection, AKey, FBuf, 64, AFileName);
    return strtol(FBuf, NULL, 10);
}

// ============================== Inner use ====================================
char* skipleading(char *S) {
    while (*S != '\0' && *S <= ' ') S++;
    return (char *)S;
}
char* skiptrailing(char *S, const char *base) {
    while ((S > base) && (*(S-1) <= ' ')) S--;
    return (char *)S;
}
char* striptrailing(char *S) {
    char *ptr = skiptrailing(strchr(S, '\0'), S);
    *ptr='\0';
    return S;
}

