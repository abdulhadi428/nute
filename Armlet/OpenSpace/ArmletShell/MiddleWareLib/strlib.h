#ifndef _NAMESPACE_PROTECT_
	#error "Use ArmletAppSDK.h"
#endif

// returns -1 or index of first occurence of pattern in string starting from startIndex
sword_t InStr(const char* string, const char* pattern, uword_t startIndex);

// returns string length
uword_t Length(const char* string);

// checks if strings are equal (note: NULL!=NULL)
bool_t StringEquals(const char* src, const char* cmp);

//copies copyLenth chars from src string to dest string at destIndex
fresult StrCopy(char* dest, uword_t destIndex, const char* src, uword_t copyLength);

//pads dest with char from destIndex length times
fresult StrPad(char* dest, uword_t destIndex, const char patten, uword_t length);
