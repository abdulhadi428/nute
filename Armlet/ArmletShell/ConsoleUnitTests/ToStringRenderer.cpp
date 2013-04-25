#include "ToStringRenderer.h"
#include "TextHelper.h"
#include "strlib.h"
#include "PlatformAPI.h"

fresult ToStringRenderer::DrawString( Position pospx, const char* str, int length, TextFormat* format)
{
	fresult fres;
	char* tempString = _allocBytesTest(length+1);
	char* tempFormatString = _allocBytesTest(length+1);
	tempString[length] = 0;
	tempFormatString[length] = 0;
	StrCopy(tempString,0,str,length);
	char representor = GetFormatRepresenterChar(format);
	StrPad(tempFormatString,0,representor,length);

	//no multiline!
	if (InStr(tempString, "\n",0)!=-1)
	{
		return GENERAL_ERROR;
	}

	//Convert screen to text coords
	Position posTx;
	posTx.Top = pospx.Top / format->Font.GlyphSize.Height;
	posTx.Left = pospx.Left / format->Font.GlyphSize.Width;


	fres = SetTextInBuff(tempString, posTx, _buff, _sizetx);
	if (fres!=SUCCESS)
	{
		return fres;
	}

	
	if (representor == NULL)
	{
		return GENERAL_ERROR;
	}

	fres = SetTextInBuff(tempFormatString, posTx, _formatBuff, _sizetx);
	delete tempString;
	delete tempFormatString;

	return fres;
}

fresult ToStringRenderer::Init(Size size)
{
	fresult fres = SUCCESS;
	
	_sizetx = size;
	_buff = AllocStringBuffer(size, true);
	if (_buff==NULL)
	{
		return GENERAL_ERROR;
	}


	_formatBuff = AllocStringBuffer(size, true);
	if (_buff==NULL)
	{
		return GENERAL_ERROR;
	}
	fres = Cls();

	//init format buffer
	_representorsCount = 2;
	_representors = new FormatStub[_representorsCount];
	_representors[0].BgColor = clBlack;
	_representors[0].FgColor = clWhite;
	_representors[0].Representor = '~';

	_representors[1].BgColor = clGrey;
	_representors[1].FgColor = clRed;
	_representors[1].Representor = '!';


	return fres;
}

fresult ToStringRenderer::Cls()
{
	FillStringBuffer(_buff, _sizetx, '\0');
	return FillStringBuffer(_formatBuff, _sizetx, '\0');
}

char* ToStringRenderer::GetScreen()
{
	return AllocAndConvertBuffAsPrintable(_buff,_sizetx, '$');
}

char* ToStringRenderer::GetFormatScreen()
{
	return AllocAndConvertBuffAsPrintable(_formatBuff,_sizetx, '$');
}

char ToStringRenderer::GetFormatRepresenterChar( TextFormat* format )
{
	for (int i =0; i < _representorsCount; i++)
	{
		if (_representors[i].BgColor == format->BgColor && 
			_representors[i].FgColor == format->FgColor)
		{
			return _representors[i].Representor;
		}
	}

	return '*';
}

fresult ToStringRenderer::DrawRect( Position pospx, Size sizepx, Color color )
{
	//TODO: implement
	return GENERAL_ERROR;
}
