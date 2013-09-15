#pragma once
#include "ArmletAppSDK.h"

//adds add string to src. returns newly allocated string
char* StringAdd(const char* src, const char* add);

//allocates a non-zero terminated buff of given size. 
//Note AllocZeroTerminatedStringBuffer
char* AllocStringBuffer(Size size, bool zeroTerminate);

fresult FillStringBuffer(char* buff, Size size, char padChar);
//WARNING: may partially fill the buff and then fail on multiline
fresult SetTextInBuff(const char* string, Position pos, char* pBuff, Size buffSize);

char* AllocAndConvertBuffAsPrintable(const char* buff, Size buffSize, char nullReplace);
fresult AllocSquareBuffFromString(const char* string, char** poResult, Size* poSize, char padChar);

fresult getFormatBuffer(char* buff, Size* size, char defaultFormat);
