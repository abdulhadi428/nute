/*
 * sd.h
 *
 *  Created on: 13.02.2013
 *      Author: kreyl
 */

#ifndef SD_H_
#define SD_H_

#include "ff.h"
#include "ch.h"
#include "hal.h"

// See SDIO clock divider in halconf.h

class sd_t {
private:
    FATFS SDC_FS;
    uint8_t fbuff[1024];
public:
    bool IsReady;
    FIL File;
    void Init();
};

extern sd_t SD;

// ============================= File operations ===============================
// return false in case of failure, true in case of success
#define FILE    FIL
bool OpenFile (FILE* file, const char* filename, bool bCreate); //syscall opens/create file
int ReadFile  (FILE* file, char* buf, int len);                 //syscall returns length read
int WriteFile (FILE* file, char* buf, int len);                 //syscall returns length written
int AppendFile(FILE* file, char* buf, int len);                 //syscall return length written


// =========================== ini file operations =============================
/*
 * ini file has the following structure:
 *
 * # This is Comment: comment uses either '#' or ';' symbol
 * ; This is Comment too
 *
 * [Section]    ; This is name of section
 * Count=6      ; This is key with value of uint
 * Volume=-1    ; int
 * SoundFileName=phrase01.wav   ; string
 *
 * [Section2]
 * Key1=1
 * ...
 */

#define USE_INI_FILES

#ifdef USE_INI_FILES
uint8_t iniReadString(const char *ASection, const char *AKey, const char *AFileName, char *AOutput, uint32_t AMaxLen);
uint8_t iniReadInt32 (const char *ASection, const char *AKey, const char *AFileName, int32_t *AOutput);
uint8_t iniReadUint32(const char *ASection, const char *AKey, const char *AFileName, uint32_t *AOutput);
#endif


#endif /* SD_H_ */
