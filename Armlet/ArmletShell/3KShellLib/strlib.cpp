#include "strlib.h"
#include "fresult.h"

sword_t InStr(const char* string, const char* pattern, uword_t startIndex)
{
	uword_t index = startIndex;
	uword_t matchIndex=0;
	bool found = 0;
	sword_t result = -1;
	uword_t matchStart = 0;

	while (string[index] != NULL)
	{
		matchIndex=0;
		matchStart = index;
		while (string[index] == pattern[matchIndex])
		{
			//continue loooking to next char;
			matchIndex ++;
			index ++;

			//if gone till end, then we found the match
			if (pattern[matchIndex] == NULL)
			{
				found = true;
				break;
			}
		}

		if (found==true)
		{
			result = matchStart;
			break;
		}
		index++;
	}

	return result;
}

uword_t Length(const char* string)
{
	//play safe
	if (string==NULL)
		return 0;

	uword_t i=0;
	while (string[i++] != 0);
	i--;
	return i;
}

//returns:
// 0 - equal
// >0 bigger
// <0 lesser
sbyte_t StringEquals(const char* src, const char* cmp)
{
	uword_t index =0;

	while (src[index]!=0)
	{
		if (src[index]==cmp[index])
		{
			index++;
		}
		else
		{
			return 0;
		}
	}

	if(src[index] == cmp[index])
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


//Copies copyLength chars from src string to dest string at destIndex
fresult StrCopy( char* dest, uword_t destIndex, const char* src, uword_t copyLength )
{
	for (uword_t i = 0;i < copyLength; i++ )	
	{
		dest[destIndex+i] = src[i];
	}

	return SUCCESS;
}

fresult StrPad(char* dest, uword_t destIndex, const char patten, uword_t length)
{
	for (uword_t i = 0;i < length; i++ )	
	{
		dest[destIndex+i] = patten;
	}

	return SUCCESS;
}