#include "project.h"
#include "TextField.h"
#include "Dimensions.h"
#include "IntegrationWithKeltur.h"
#include "ScrollableControlBase.h"
#include "strlib.h"



uword_t TextField::GetBuffIndex( Position pos )
{
	if (pos.Top*_textBuffSizeTx.Width+pos.Left < _textBuffSizeTx.Height*_textBuffSizeTx.Width)
	{
		return pos.Top*_textBuffSizeTx.Width+pos.Left;
	}
	else
	{
		return -1;
	}
}

fresult TextField::PutCharToBuff( Position pos, char charToPut)
{
	uword_t index = GetBuffIndex(pos);
	if (index ==-1)
	{
		return GENERAL_ERROR;
	}

	if (! ((pos.Left < _textBuffSizeTx.Width) && (pos.Top < _textBuffSizeTx.Height)))
	{
		return GENERAL_ERROR;
	}

	if (pos.Top > _lastUsedLine)
	{
		_lastUsedLine = pos.Top;
	}

	_textBuff[index] = charToPut;
	return SUCCESS;
}

#pragma region getters

TextFormat* TextField::GetTextFormat()
{
	return &_Format;
}
#pragma endregion //Getters

//Setters

void TextField::SetTextFormat( TextFormat* format )
{
	if (format!=NULL)
		_Format = *format;
}


fresult TextField::AppendText(const char* text)
{
	fresult fres = SUCCESS;
	ubyte_t wrapLimit;
	ubyte_t wrapBacktrack;
	//calculate wrap

	ubyte_t lineLengthToDrawTx = _Size.Width / _Format.Font.GlyphSize.Width;
	if (lineLengthToDrawTx <1)
	{
		return GENERAL_ERROR;
	}

	//that may exceed the buff - then we'll need to wrap to buff width, not the size width
	if (_textBuffSizeTx.Width < lineLengthToDrawTx || !_WordWrap)
	{
		wrapLimit = _textBuffSizeTx.Width;
		//obviously - not backtrack
		wrapBacktrack = 0;
	}
	else
	{
		//word wrap in effect
		wrapLimit = lineLengthToDrawTx;
		//backtrack up to half of control width
		wrapBacktrack = lineLengthToDrawTx /2;
	}

	Position buffWritePosition;
	buffWritePosition.data = _textBuffCarretPositionTx.data;
	if (! (buffWritePosition.Left*buffWritePosition.Top < _buffLength))
	{
		return GENERAL_ERROR;
	}

	uword_t strReadIndex=0;
	uword_t buffLength = _textBuffSizeTx.Height*_textBuffSizeTx.Width;

	while (text[strReadIndex]!=0)
	{
		//read ahead to next word end
		ubyte_t readAheadIndex =0;
		for (readAheadIndex = 0; readAheadIndex+buffWritePosition.Left < wrapLimit;readAheadIndex++)
		{
			if (text[strReadIndex+readAheadIndex] ==0 ||
				text[strReadIndex+readAheadIndex] == ' ' ||
				text[strReadIndex+readAheadIndex] == '\n')
			{
				break;
			}
		}

		//word limit at readAheadIndex chars from current position. 
		//Decide whether to wrap
		if (buffWritePosition.Left + readAheadIndex >= wrapLimit)
		{
			//next word border exceeds wrap limit
			//decide to wrap
			if (readAheadIndex < wrapBacktrack)
			{
				//Wrapping to next line
				//move caret to next line
				buffWritePosition.Top++;
				buffWritePosition.Left=0;
				//strReadIndex remains the same
				continue;
			}
		} 

		//copy found word to existing line
		ubyte_t i =0; 
		do
		{
			if (text[strReadIndex] != 0)
			{
				if (text[strReadIndex]!='\n')
				{
					//put the char to buff and move left in buff and in text
					fres = PutCharToBuff(buffWritePosition, text[strReadIndex]);
					if (fres!=SUCCESS)
					{
						//Just clip!
						return SUCCESS;	
					}
					strReadIndex++;
					//we touched the wrap limit tightly
					if (buffWritePosition.Left == wrapLimit-1)
					{
						//moving to next line
						buffWritePosition.Top ++;
						buffWritePosition.Left=0;
						//if next is space or \n - skip it
						if (text[strReadIndex] !=0 && (text[strReadIndex] ==' ' || text[strReadIndex]=='\n'))
						{
							strReadIndex++;
						}
						else
						{
							buffWritePosition.Left=0;
						}
					}
					else
					{
						buffWritePosition.Left++;
					}
					
				}
				else
				{
					//move line down and move text left
					strReadIndex++;
					buffWritePosition.Top++;
					buffWritePosition.Left=0;
				}
			}
			i++;
		} while (i< readAheadIndex);
		
	}

	//adjust caret pos
	_textBuffCarretPositionTx.data = buffWritePosition.data;

	return SUCCESS;
}

fresult TextField::Clear()
{
	//clean up
	fresult fres;
	fres = StrPad(_textBuff,0,0,_buffLength);
	if (fres!=SUCCESS)
	{
		return fres;
	}
	_ScrollPositionTx.data =0;
	_textBuffCarretPositionTx.data =0;

	return SUCCESS;
}

//Setting the text;
fresult TextField::SetText(const char* text )
{
	fresult fres;
	fres= Clear();
	if (fres!=SUCCESS)
	{
		return fres;
	}

	//append to 0,0
	fres = AppendText(text);
	if (fres!=SUCCESS)
	{
		return fres;
	}

	return SUCCESS;
}

Position TextField::GetScrollPosition()
{
	return _ScrollPositionTx;
}

//Setting the scroll window position ubyte_telligently (try find best fit if it will go off TextSize limits)
//	returns:
//	SUCCESS: 
//	    - if position fits well
//	GENERAL_WARNING
//	    - if position was adjusted to fit in TextSize (*positon parameter will contain adjusted values)
//
fresult TextField::SetScrollPosition (Position position)
{

	//Position may not fit in text, then it should be moved to top and left to fit
	//Logic: it's useless to show empty data. So we will scroll only until at least one line 
	//or column is visible
	fresult result = SUCCESS;

	if(position.Left >= _textBuffSizeTx.Width)
	{
		result = GENERAL_WARNING;
		position.Left = _textBuffSizeTx.Width - 1;
	}
	if (position.Left <0)
	{
		result = GENERAL_WARNING;
		position.Left = 0;
	}

	if(position.Top >= _textBuffSizeTx.Height)
	{
		result = GENERAL_WARNING;
		position.Top = _textBuffSizeTx.Height - 1;
	}
	
	if (position.Top > _lastUsedLine)
	{
		result = GENERAL_WARNING;
		position.Top = _lastUsedLine;
	}

	if (position.Top <0)
	{
		result = GENERAL_WARNING;
		position.Top = 0;
	}
	_ScrollPositionTx = position;

	return result;
}


//Initialization;
fresult TextField::Init(Size size, Position position, char* buff, Size buffSize, IRender* renderer )
{
	fresult fres;
	fres = BaseInit(size, position, renderer);
	if (fres != SUCCESS)
	{
		return fres;
	}
	
	_ScrollPositionTx.Left = 0;
	_ScrollPositionTx.Top = 0;

	
	//Defaulting:
	_Format.BgColor = DEFAULT_BACKGROUND;
	_Format.FgColor = DEFAULT_FOREGROUND;
	//Get Default font
	fres = TODO_KELTUR_GetFontById(DEFAULT_FONT, &(_Format.Font));
	if (fres != SUCCESS)
	{
		return fres;
	}
	
	SetSize(size);

	_Position = position;
	_render = renderer;

	_textBuff = buff;
	_textBuffSizeTx = buffSize;
	_buffLength = buffSize.Height*buffSize.Width;

	_WordWrap = false;

	_textBuffCarretPositionTx.data =0;

	return SUCCESS;
};

//Logic

//Gets Line staring at specified poubyte_t. 
//it returns poubyte_ter to the original buff, so it MUST be treated as const
//NOTE:
//If required length exceeds textSize.Width - returns GENERAL_WARNING and length is set to actual length
//If required pos.Top is outside textSize.Height - returns GENERAL_WARNING and oLine is NULL, and length is 0;
fresult TextField::GetLineAtXY( Position pos, ubyte_t* ioLength, char** oLine )
{
	fresult fres = SUCCESS;
	
	if (!(pos.Left < _textBuffSizeTx.Width))
	{
		return GENERAL_ERROR;
	}

	//If requested pos.Top is outside _textSize.Height
	if (!(pos.Top < _textBuffSizeTx.Height))
	{
		*oLine = NULL;
		(*ioLength) = 0;
		return GENERAL_WARNING;
	}

	//check whether we exceed textSize
	if ((pos.Left + *ioLength) > _textBuffSizeTx.Width)
	{
		(*ioLength) = _textBuffSizeTx.Width - pos.Left;
		fres = GENERAL_WARNING;
	}

	sword_t indexInText = GetBuffIndex(pos);
	if (indexInText < 0)
	{
		//out of bounds
		return GENERAL_ERROR;
	}
	
	//Get pointer to line in readonly text
	*oLine = (char*)&_textBuff[indexInText];

	return fres;
}


fresult TextField::DrawArea(Position pos, Size size )
{
	fresult fres = SUCCESS;

	if (!_bVisible)
	{
		return SUCCESS;
	}

	//User might request to draw bigger area, we need to put it to what we have
	Position drawPositionPx;
	drawPositionPx.data = pos.data;
	Size drawSizePx;
	drawSizePx.data = size.data;

	if (pos.Left < _Position.Left)
	{
		drawPositionPx.Left = _Position.Left;
	}

	if (pos.Top < _Position.Top)
	{
		drawPositionPx.Top = _Position.Top;
	}
	
	if (size.Height > _Size.Height)
	{
		drawSizePx.Height = _Size.Height;
	}

	if (size.Width > _Size.Width)
	{
		drawSizePx.Width = _Size.Width;
	}

	ubyte_t writeIndex=0;

	char* lineGot=NULL;

	ubyte_t lineLengthToDrawTx = drawSizePx.Width / _Format.Font.GlyphSize.Width;
	//we may request length that exceeds the buff, so we will need to draw partially
	ubyte_t lastReadLineLength = drawSizePx.Width / _Format.Font.GlyphSize.Width;

	ubyte_t visibleHeightTx = drawSizePx.Height / _Format.Font.GlyphSize.Height;

	Position readBufPosTx;

	//Find the char at the position
	readBufPosTx.Top = _ScrollPositionTx.Top + (drawPositionPx.Top - _Position.Top) / _Format.Font.GlyphSize.Height;
	readBufPosTx.Left = _ScrollPositionTx.Left + (drawPositionPx.Left- _Position.Left ) / _Format.Font.GlyphSize.Width;

	//draw background
	_render->DrawRect(drawPositionPx, drawSizePx, _Format.BgColor);
	
	for (ubyte_t lineIndex= 0; lineIndex < visibleHeightTx; lineIndex++ )
	{
		lineLengthToDrawTx = drawSizePx.Width / _Format.Font.GlyphSize.Width;

		//try get string at index
		fres = GetLineAtXY(readBufPosTx, &lineLengthToDrawTx, &lineGot);
		if (fres == GENERAL_ERROR)
		{
			return GENERAL_ERROR;
		} 			

		//We got something or whole line was outside of TextSize
		if (lineLengthToDrawTx != 0)
		{
			//we got something to draw
			_render->DrawString(drawPositionPx, lineGot, lineLengthToDrawTx, &_Format);
		}
		else
		{
			//we are out of buff
			break;
		}
		drawPositionPx.Top += _Format.Font.GlyphSize.Height ;
		readBufPosTx.Top++;
	}

	return SUCCESS;
}

fresult TextField::ScrollUp()
{
	Position pos;
	pos.data = _ScrollPositionTx.data;
	if (pos.Top!=0)
	{
		pos.Top--;
	}
	return SetScrollPosition(pos);
}

fresult TextField::ScrollDown()
{
	Position pos;
	pos.data = _ScrollPositionTx.data;
	pos.Top++;
	return SetScrollPosition(pos);
}







