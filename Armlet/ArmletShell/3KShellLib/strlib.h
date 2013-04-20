#pragma once

#include "Dimensions.h"
#include <string.h>
#include "fresult.h"

// returns index of first occurance of pattern in string starting from startIndex
int InStr(const char* string, const char* pattern, int startIndex);

// returns string length
int Length(const char* string);

// checks if strings are equal (note: NULL=NULL)
int StringEquals(const char* src, const char* cmp);
//adds add string to src. returns newly allocated string
char* StringAdd(const char* src, const char* add);

//Copies copyLenth chars from src string to dest string at destIndex
fresult StrCopy(char* dest, int destIndex, const char* src, int copyLength);

//pads dest with char from destIndex length times
fresult StrPad(char* dest, int destIndex, const char patten, int length);