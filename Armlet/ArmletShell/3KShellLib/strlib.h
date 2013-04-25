#pragma once

#include "Dimensions.h"
#include <string.h>
#include "fresult.h"

// returns index of first occurence of pattern in string starting from startIndex
sword_t InStr(const char* string, const char* pattern, uword_t startIndex);

// returns string length
uword_t Length(const char* string);

// checks if strings are equal (note: NULL=NULL)
sbyte_t StringEquals(const char* src, const char* cmp);
//adds add string to src. returns newly allocated string
char* StringAdd(const char* src, const char* add);

//Copies copyLenth chars from src string to dest string at destIndex
fresult StrCopy(char* dest, uword_t destIndex, const char* src, uword_t copyLength);

//pads dest with char from destIndex length times
fresult StrPad(char* dest, uword_t destIndex, const char patten, uword_t length);