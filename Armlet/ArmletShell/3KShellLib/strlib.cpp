#include "strlib.h"
#include "fresult.h"

int InStr(const char* string, const char* pattern, int startIndex)
{
	int index = startIndex;
	int matchIndex=0;
	bool found = 0;
	int result = -1;
	int matchStart = 0;

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

int Length(const char* string)
{
	//play safe
	if (string==NULL)
		return 0;

	return strlen(string);
}

//returns:
// 0 - equal
// >0 bigger
// <0 lesser
int StringEquals(const char* src, const char* cmp)
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
fresult StrCopy( char* dest, int destIndex, const char* src, int copyLength )
{
	for (int i = 0;i < copyLength; i++ )	
	{
		dest[destIndex+i] = src[i];
	}

	return SUCCESS;
}

fresult StrPad(char* dest, int destIndex, const char patten, int length)
{
	for (int i = 0;i < length; i++ )	
	{
		dest[destIndex+i] = patten;
	}

	return SUCCESS;
}