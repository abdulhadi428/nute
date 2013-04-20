#include "project.h"
#include "TextField.h"
#include "Dimensions.h"
#include "IntegrationWithKeltur.h"
#include "ScrollableControlBase.h"



int TextField::GetBuffIndex( Position pos )
{
	if (pos.Top*_BuffSizeTx.Width+pos.Left < _BuffSizeTx.Height*_BuffSizeTx.Width)
	{
		return pos.Top*_BuffSizeTx.Width+pos.Left;
	}
	else
	{
		return -1;
	}
	
}

#pragma region getters
const char* TextField::GetText()
{
	return _readOnlyText;
};

Size TextField::GetTextSize()
{
	return _BuffSizeTx;
};

bool TextField::GetReadOnly()
{
	return true;
}

TextFormat* TextField::GetTextFormat()
{
	return &_Format;
}
#pragma endregion //Getters

//Setters

void TextField::SetTextFormat( TextFormat* format )
{
	_Format = *format;
}


//Setting the text;
fresult TextField::SetText( const char* text, Size textSize, bool readOnly, TextFormat* pFormat )
{
	_readOnlyText = text;
	if (pFormat)
		_Format = *pFormat;
	_BuffSizeTx = textSize;
	//TODO: format

	return SUCCESS;
}

Position TextField::GetScrollPosition()
{
	return _ScrollPositionTx;
}

//Setting the scroll window position intelligently (try find best fit if it will go off TextSize limits)
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

	if(position.Left >= _BuffSizeTx.Width)
	{
		result = GENERAL_WARNING;
		position.Left = _BuffSizeTx.Width - 1;
	}

	if(position.Top >= _BuffSizeTx.Height)
	{
		result = GENERAL_WARNING;
		position.Top = _BuffSizeTx.Height - 1;
	}

	_ScrollPositionTx = position;

	return result;
}


//Initialization;
fresult TextField::Init(Size size, Position position, IRender* renderer )
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

	_readOnlyText = NULL;
	_render = renderer;

	return TRUE;
};

//Logic

//Gets Line staring at specified point. 
//it returns pointer to the original buff, so it MUST be treated as const
//NOTE:
//If required length exceeds textSize.Width - returns GENERAL_WARNING and length is set to actual length
//If required pos.Top is outside textSize.Height - returns GENERAL_WARNING and oLine is NULL, and length is 0;
fresult TextField::GetLineAtXY( Position pos, int* ioLength, char** oLine )
{
	fresult fres = SUCCESS;
	
	if (!(pos.Left < _BuffSizeTx.Width))
	{
		return GENERAL_ERROR;
	}

	//If requested pos.Top is outside _textSize.Height
	if (!(pos.Top < _BuffSizeTx.Height))
	{
		*oLine = NULL;
		(*ioLength) = 0;
		return GENERAL_WARNING;
	}

	//check whether we exceed textSize
	if ((pos.Left + *ioLength) > _BuffSizeTx.Width)
	{
		(*ioLength) = _BuffSizeTx.Width - pos.Left;
		fres = GENERAL_WARNING;
	}

	int indexInText = GetBuffIndex(pos);
	if (indexInText < 0)
	{
		//out of bounds
		return GENERAL_ERROR;
	}
	
	//Get pointer to line in readonly text
	*oLine = (char*)&_readOnlyText[indexInText];

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

	int writeIndex=0;

	char* lineGot=NULL;

	int lineLengthToDrawTx = drawSizePx.Width / _Format.Font.GlyphSize.Width;
	//we may request length that exceeds the buff, so we will need to draw partially
	int lastReadLineLength = drawSizePx.Width / _Format.Font.GlyphSize.Width;

	int visibleHeightTx = drawSizePx.Height / _Format.Font.GlyphSize.Height;

	Position readBufPosTx;

	//Find the char at the position
	readBufPosTx.Top = _ScrollPositionTx.Top + (drawPositionPx.Top - _Position.Top) / _Format.Font.GlyphSize.Height;
	readBufPosTx.Left = _ScrollPositionTx.Left + (drawPositionPx.Left- _Position.Left ) / _Format.Font.GlyphSize.Width;

	
	for (int lineIndex= 0; lineIndex < visibleHeightTx; lineIndex++ )
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







