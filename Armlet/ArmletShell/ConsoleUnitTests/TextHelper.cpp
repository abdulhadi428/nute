
#include "TextHelper.h"
#include "PlatformAPI.h"
#include "ArmletShell.h"

char* StringAdd(const char* src, const char* add)
{
	int lenSrc = Length(src);
	int lenAdd = Length(add);
	int buffSize = lenSrc + lenAdd;
	char* result = _allocBytesTest(buffSize+1);
	result[buffSize] = NULL;

	if (result == NULL)
	{
		return NULL;
	}

	for (int i=0;i<lenSrc;i++)
	{
		result[i] = src[i];
	}

	for (int i=0;i<lenAdd;i++)
	{
		result[lenSrc + i] = add[i];
	}

	return result;
}

char* AllocStringBuffer(Size size, bool zeroTerminate )
{
	if (IsEmpty(size))
	{
		return NULL;
	}

	int length = size.Height*size.Width;
	if (zeroTerminate)
	{
		length++;
	}

	char* buff= _allocBytesTest(length);
	if (buff==NULL)
	{
		return NULL;
	}

	if (zeroTerminate)
	{
		buff[size.Height*size.Width] = NULL;
	}

	//zero memory
	FillStringBuffer(buff, size, NULL);

	return buff;
}


fresult FillStringBuffer( char* buff, Size size, char padChar )
{
	if (IsEmpty(size))
	{
		return GENERAL_ERROR;
	}
	
	for (int i = 0; i< size.Height*size.Width;i++)
	{
		buff[i] = padChar;
	}

	return SUCCESS;
}


//WARNING: may partially fill the buff and then fail on miltiline
fresult SetTextInBuff(const char* string, Position pos, char* buff, Size buffSize)
{
#pragma region  playsafe
	if (!IsPositionInsideSize(buffSize, pos))
	{
		return GENERAL_ERROR;
	}
	
	if (buff == NULL)
	{
		return GENERAL_ERROR;
	}

	if (string == NULL)
	{
		return GENERAL_ERROR;
	}
#pragma endregion
	int stringReadIndex=0;
	int buffLength = buffSize.Height*buffSize.Width;
	int buffWriteIndex = pos.Top*buffSize.Width+pos.Left;
	Position buffWritePosition;
	buffWritePosition.Left = pos.Left;
	buffWritePosition.Top = pos.Top;
	char newLineFound = 0;
	
	while (string[stringReadIndex] != NULL)
	{
		newLineFound =0;
		//check for newline in string
		if(string[stringReadIndex] == '\n')
		{
			newLineFound =1;
		}

		if (newLineFound)
		{
			//moved to new line

			buffWritePosition.Top++;
			//check bounds
			if (buffWritePosition.Top>buffSize.Height)
			{
				return GENERAL_ERROR;
			}
			buffWritePosition.Left = 0;
			buffWriteIndex = buffWritePosition.Top*buffSize.Width;
			
			//moving read index to next after/n
			stringReadIndex++;
		}
		else
		{
			//no line move

			//check buff bounds
			if (!(buffWriteIndex < buffLength))
			{
				return GENERAL_ERROR;
			}
			//check width bounds
			else if (!(buffWritePosition.Left < buffSize.Width))
			{
				return GENERAL_ERROR;
			}
			else
			{
				buff[buffWriteIndex]=string[stringReadIndex];
				buffWriteIndex++;
				stringReadIndex++;
				buffWritePosition.Left ++;
			}
		}
	}

	return SUCCESS;
}

char* AllocAndConvertBuffAsPrintable(const char* buff, Size buffSize, char nullReplace)
{
	if (IsEmpty(buffSize))
	{
		return NULL;
	}

	if (buff == NULL)
	{
		return NULL;
	}

	if (nullReplace == NULL)
	{
		return NULL;
	}

	char* printableBuff = _allocBytesTest(buffSize.Height*buffSize.Width + (buffSize.Height-1)  +1); 
	if (printableBuff==NULL)
	{
		return NULL;
	}
	//Add zero termination
	printableBuff[buffSize.Height*buffSize.Width + (buffSize.Height-1)] = NULL;

	int printBuffWriteIndex =0;
	int buffReadIndex =0;

	for (int lineIndex=0;lineIndex<buffSize.Height;lineIndex++)
	{
		for (int charIndex=0;charIndex < buffSize.Width;charIndex++)
		{
			if (buff[buffReadIndex] !=NULL)
			{
				printableBuff[printBuffWriteIndex] = buff[buffReadIndex];
			}
			else
			{
				printableBuff[printBuffWriteIndex] = nullReplace;
			}
			
			printBuffWriteIndex++;
			buffReadIndex++;
		}

		//add /n to all lines but last
		if (lineIndex < buffSize.Height -1)
		{
			printableBuff[printBuffWriteIndex] = '\n';
			printBuffWriteIndex++;
		}
	}

	return printableBuff;
}

//gets sq buffer (allocates it)
fresult AllocSquareBuffFromString( const char* string, char** poResult, Size* poSize, char padChar )
{
	//play safe
	if (poSize == NULL)
	{
		return GENERAL_ERROR;
	}

	if (string==NULL)
	{
		return GENERAL_ERROR;
	}

	(*poSize).Width =0;
	(*poSize).Height =1;

	fresult result = SUCCESS;


	int stringIndex =0;
	int lineBreakIndex = InStr(string, "\n", stringIndex);
	//if it's 1-line
	int lastLineStart = 0;

	uword_t stringLength = Length(string);

	//scan line by line, line is delimited by \n
	while (lineBreakIndex !=-1)
	{
		//find longest string
		if ((*poSize).Width < lineBreakIndex-stringIndex)
		{
			(*poSize).Width = lineBreakIndex-stringIndex;
		}

		//we found the line
		(*poSize).Height++;
		
		//moving index (+1 for found \n)
		stringIndex += lineBreakIndex+1;
		lastLineStart = lineBreakIndex+1;

		//try find next line
		lineBreakIndex = InStr(string, "\n", stringIndex);
	}

	int lastLineLength = stringLength-lastLineStart;
	if ((*poSize).Width < lastLineLength)
	{
		(*poSize).Width = lastLineLength;
	}

	//if there were no lines, we didn't enter the loop, so we need to calc Width


	//allocate buffer
	*poResult = AllocStringBuffer(*poSize,true);
	if (poResult == NULL)
	{
		return GENERAL_ERROR;
	}
 	int resultWriteIndex =0;

	stringIndex=0;
	int linePosition=0;
	int newLineFound=0;

	while (string[stringIndex]!=NULL)
	{
		
		//check for newline in string
		newLineFound = 0;
		if(string[stringIndex] == '\n')
		{
			newLineFound =1;
		}

		//thats' a new line?
		if(newLineFound)
		{
				//new line
				
				//fill rest of the line with pad
				for (;linePosition< (*poSize).Width;linePosition++)
				{
					(*poResult)[resultWriteIndex] = padChar;
					resultWriteIndex++;
				}
				//goto next line;
				linePosition =0;
				//skip /n
				stringIndex+=1;
				newLineFound = 0;
		}
		else
		{
			//not a new line

			//copy char from string
			(*poResult)[resultWriteIndex] = string[stringIndex];
			resultWriteIndex++;
			stringIndex++;
			linePosition++;
		}
	}

	return result;
}

fresult getFormatBuffer( char* buff, Size size, char defaultFormat )
{
	return FillStringBuffer(buff, size, defaultFormat);
}


