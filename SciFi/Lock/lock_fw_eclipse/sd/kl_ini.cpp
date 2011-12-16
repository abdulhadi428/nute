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

char IBuf[INI_STR_BUF_SIZE];

void WriteString(const char *ASection, const char *AKey,const char *AInput, const char *AFileName)
{
  FRESULT rslt;
  UINT result;
  FIL IFile;
  uint32_t iParamOffset;
  uint32_t iParamLen;
  uint32_t iInputLen;
  // Open file
  rslt = f_open(&IFile, AFileName, FA_WRITE+FA_READ+FA_OPEN_EXISTING );
  if (rslt != FR_OK) {
      UART_PrintString(AFileName);
      if (rslt == FR_NO_FILE) UART_PrintString("File not found\r");
      else UART_StrUint("OpenFile error: ", rslt);
      return;
  }
  // Check if zero file
  if (IFile.fsize == 0) {
      f_close(&IFile);
      UART_PrintString("Empty file\r");
      return;
  }
  // Move through file 1 line at a time until a section is matched or EOF.
  char *StartP, *EndP;
  int16_t len = strlen(ASection);
  do {
      if (!f_gets(IBuf, INI_STR_BUF_SIZE, &IFile)) {
          f_close(&IFile);
          return;
      }
      StartP = skipleading(IBuf);
      EndP = strchr(StartP, ']');
  } while (*StartP != '[' || EndP == NULL || ((int)(EndP-StartP-1) != len || strnicmp(StartP+1, ASection, len) != 0));

  // Section found, find the key
  len = strlen(AKey);
  do {
      if (!f_gets(IBuf, INI_STR_BUF_SIZE, &IFile) || *(StartP = skipleading(IBuf)) == '[') {
          f_close(&IFile);
          return;
      }
      StartP = skipleading(IBuf);
      EndP = strchr(StartP, '='); /* Parse out the equal sign */
  } while (*StartP == ';' || *StartP == '#' || EndP == NULL || ((int)(skiptrailing(EndP, StartP)-StartP) != len || strnicmp(StartP, AKey, len) != 0));

  /************начинаем записывать новый параметр *********************/
  //result=f_tell(&IFile);
  //UART_StrUint("end param pos=", result);
  iParamLen=strlen(IBuf)-(EndP-IBuf)-1;// длинна значения параметра. (длинна строки)-(название параметра)-(знак равно)
 // UART_StrUint("param len=", iParamLen);
  iParamOffset=f_tell(&IFile)-iParamLen;        // это будет указатель на начало значения параметра
  //UART_StrUint("start param pos=", iParamOffset);
  iInputLen=strlen (AInput)+2; // +2 - это чтобы добавить символы \n\r

  if (iInputLen!=iParamLen) // длинна нового параметра не равна длинне старого => нужно сдвигать весь файл
    {
      uint32_t iTempBufLen=f_size (&IFile)-f_tell(&IFile);
      if (iTempBufLen>=INI_FILE_BUF_SIZE)
        {
          UART_PrintString("INI file to large \r");
          f_close(&IFile);
          return;
        }
      char pchTempBuf[INI_FILE_BUF_SIZE];
      rslt=f_read(&IFile,pchTempBuf,iTempBufLen,&result); //считываем в него фрагмент файла с текущей позиции
      UART_StrUint("Read byte=", result);
      if (rslt != FR_OK)
        {
          UART_PrintString("Error read file\r");
          f_close(&IFile);
          return;
        }
      rslt=f_lseek(&IFile,iParamOffset+iInputLen); // указатель в конец нового параметра (новый еще не записан, но мы знаем его размер :-)
      rslt=f_write(&IFile,pchTempBuf,iTempBufLen,&result); // записываем фрагмент файла на новое место.
      UART_StrUint("Write byte=", result);
      rslt=f_truncate (&IFile);  //The f_truncate function truncates the file size to the current file read/write point.
    }
  rslt=f_lseek(&IFile,iParamOffset);
  //result=f_tell(&IFile);
  //UART_StrUint("start new param=", result);
  rslt=f_write(&IFile,AInput,iInputLen-2,&result); // записываем новый параметр
  //UART_PrintString(AInput);
  //UART_StrUint("Writed new param size=", result);
  rslt=f_write(&IFile,"\r\n",2,&result);                //и перевод строки

  f_close(&IFile);

}

//для проверки существования данного параметра
// можно сильно оптимизировать позже
bool ParamIsExists(const char *ASection, const char *AKey,const char *AFileName)
{
  char str[INI_STR_BUF_SIZE];
  char* pstr;
  pstr=ReadString(ASection,AKey,str,INI_STR_BUF_SIZE,AFileName);
  if (pstr==NULL) return false;
  return true;
}

char* ReadString(const char *ASection, const char *AKey, char *AOutput, uint32_t AMaxLength, const char *AFileName) {
    FRESULT rslt;
    FIL IFile;
    // Open file
    rslt = f_open(&IFile, AFileName, FA_READ+FA_OPEN_EXISTING);
    if (rslt != FR_OK) {
        UART_PrintString(AFileName);
        if (rslt == FR_NO_FILE) UART_PrintString("File not found\r");
        else UART_StrUint("OpenFile error: ", rslt);
        return NULL;
    }
    // Check if zero file
    if (IFile.fsize == 0) {
        f_close(&IFile);
        UART_PrintString("Empty file\r");
        return NULL;
    }

    // Move through file 1 line at a time until a section is matched or EOF.
    char *StartP, *EndP;
    int16_t len = strlen(ASection);
    do {
        if (!f_gets(IBuf, INI_STR_BUF_SIZE, &IFile)) {
            f_close(&IFile);
            return NULL;
        }
        StartP = skipleading(IBuf);
        EndP = strchr(StartP, ']');
    } while (*StartP != '[' || EndP == NULL || ((int)(EndP-StartP-1) != len || strnicmp(StartP+1, ASection, len) != 0));

    // Section found, find the key
    len = strlen(AKey);
    do {
        if (!f_gets(IBuf, INI_STR_BUF_SIZE, &IFile) || *(StartP = skipleading(IBuf)) == '[') {
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

void PrintFileToUART (const char *AFileName)
{
  FRESULT rslt;
  FIL IFile;
  // Open file
  rslt = f_open(&IFile, AFileName, FA_READ+FA_OPEN_EXISTING);
  if (rslt != FR_OK) {
      UART_PrintString(AFileName);
      if (rslt == FR_NO_FILE) UART_PrintString("File not found\r");
      else UART_StrUint("OpenFile error: ", rslt);
      return ;
  }
  // Check if zero file
  if (IFile.fsize == 0) {
      f_close(&IFile);
      UART_PrintString("Empty file\r");
      return;
  }

  // Move through file 1 line at a time until a EOF.
  do {
      if (!f_gets(IBuf, INI_STR_BUF_SIZE, &IFile))
        {
          f_close(&IFile);
          return ;
        }
      UART_PrintString(IBuf);
  } while (1);

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

