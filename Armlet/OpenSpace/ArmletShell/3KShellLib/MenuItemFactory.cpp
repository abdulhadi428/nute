#include "ThreeKShell.h"
#include "kl_allocator.h"

namespace ThreeKShell {

static Alloc_t<MenuItem, 45> SMenuItemArr;

MenuItem* MenuItemFactory::allocMenuItem()
{
	return SMenuItemArr.Allocate(); // new MenuItem();
}

fresult MenuItemFactory::Init( IRender* render, Repositories* reps, TextFieldFactory* tfFactory, PictureBoxFactory* pbxFactory, PanelFactory* pnlFactory )
{
	fresult fres;
	_render = render;
	_tfFactory = tfFactory;
	_pbxFactory = pbxFactory;
	_pnlFactory = pnlFactory;
	_repositories = reps;

	//get text formats
	DefaultTextFormatHandle = TF_MENU;
	CurrentTextFormatHandle = DefaultTextFormatHandle;

	TextFormat *tf;
	fres = _repositories->TextFormats->GetTextFormat(CurrentTextFormatHandle, &tf);
	ENSURESUCCESS(fres);

	DefaultTextFieldVSizeByLines = TRUE;
	CurrentTextFieldVSizeByLines = DefaultTextFieldVSizeByLines;

	DefaultTextLines = 1;
	CurrentTextLines = DefaultTextLines;

	Size defaultSz;
	defaultSz.Width = 5*tf->Font.GlyphSize.Width;
	defaultSz.Height = DefaultTextLines*tf->Font.GlyphSize.Height;

	DefaultTextFieldSize = defaultSz;
	CurrentTextFieldSize = DefaultTextFieldSize;

	DefaultTextFieldVAutoSize = TRUE;
	CurrentTextFieldVAutoSize = DefaultTextFieldVAutoSize;

	DefaultTextFieldHAutoSize = TRUE;
	CurrentTextFieldHAutoSize = DefaultTextFieldHAutoSize;

	DefaultTextToImageOffset = 1;
	CurrentTextToImageOffset = DefaultTextToImageOffset;

	return SUCCESS;
}


fresult MenuItemFactory::GetMenuItem(Position origin,    Alignment align, MenuItemStyles style, ImageHandle himg, char* text, ButtonState shrtcut, IMenuHandler* handler, char* name, IMenuItem** o_mi)
{
	fresult fres;

	BitmapImage* bmp;
	TextFormatHandle tfmtHandle;
	Size tfSize;
	Size pbxSize;

	tfmtHandle = CurrentTextFormatHandle;
	TextFormat *tfmt;
	fres = _repositories->TextFormats->GetTextFormat(tfmtHandle, &tfmt);
	ENSURESUCCESS(fres);

	bool_t hasText = false;
	//calculate tf size
	if (text!=NULL)
	{
		hasText = TRUE;

		//check style
		FAILIF(style == ImageOnly);

		//AutoSizing
		ubyte_t textLinesCount=1;
		sword_t crIndex =0;
		sword_t lastCrIndex=0;
		sword_t maxLineLen =0;

		//predict text size without WRAP
		if (CurrentTextFieldHAutoSize || CurrentTextFieldVAutoSize)
		{
			sword_t len = Length(text);
			//get lines count
			crIndex = InStr(text, "\n", lastCrIndex);
			if (crIndex == -1)
			{
				maxLineLen = len;
			}
			else
			{
				lastCrIndex = crIndex;
				maxLineLen = crIndex;
				textLinesCount++;
				do
				{
					crIndex = InStr(text, "\n", lastCrIndex);
					if (crIndex != -1)
					{
						textLinesCount++;
						if (maxLineLen < crIndex-lastCrIndex)
						{
							maxLineLen = lastCrIndex - crIndex;
						}
						lastCrIndex++;
					}
					else
					{
						//loop end;
						break;
					}
				} while(TRUE);

			}
		}

		//Get Height
		if (CurrentTextFieldVAutoSize || CurrentTextFieldVSizeByLines)
		{
			//Height
			//If something is set, use it
			if (CurrentTextFieldVSizeByLines)
			{
				textLinesCount = CurrentTextLines;
			}
			tfSize.Height = tfmt->Font.GlyphSize.Height*textLinesCount;
		}
		else
		{
			tfSize.Height = CurrentTextFieldSize.Height;
		}

		//Get Width
		if (CurrentTextFieldHAutoSize)
		{
			tfSize.Width = tfmt->Font.GlyphSize.Width*maxLineLen;
		}
		else
		{
			tfSize.Width = CurrentTextFieldSize.Width;
		}
	}

	bool_t hasImage = false;
	//calculate pbx size
	if (himg!=INVALID_HANDLE)
	{
		hasImage = TRUE;
		FAILIF(style == TextOnly);

		//find image
		bmp = _repositories->Images->GetImageById(himg);
		FAILIF(bmp==NULL);

		pbxSize.data = bmp->ImageSize.data;
	}

	//calculate mi Size
	Size sz;
	if (style==TextOnly)
	{
		sz.data = tfSize.data;
	}
	else if (style == ImageOnly)
	{
		sz.data = pbxSize.data;
	}
	else
	{
		switch (style)
		{
		case ImageLeft:
		case ImageRight:
			//width = summ, height = greater
			sz.Width = tfSize.Width + pbxSize.Width + CurrentTextToImageOffset;
			sz.Height = tfSize.Height > pbxSize.Height?tfSize.Height:pbxSize.Height;
			break;
		case ImageTop:
		case ImageBottom:
		default:
			//height = summ, width = greater
			sz.Height = pbxSize.Height + tfSize.Height + CurrentTextToImageOffset;
			sz.Width = tfSize.Width > pbxSize.Width?tfSize.Width:pbxSize.Width;
			break;
		}
	}

	//calculate mi pos
	Position pos;
	Position tfPos;
	Position pbxPos;

	switch(align.HAlign)
	{
		case ToLeft:
			pos.Left = origin.Left - sz.Width;
		break;
		case CenterHorizontaly:
			pos.Left = origin.Left - sz.Width/2;
		break;
		case ToRight:
		default:
			//no change
			pos.Left = origin.Left;
	}

	//position controls's left
	if (style == TextOnly)
	{
		tfPos.Left = pos.Left;
	}
	if (style == ImageOnly)
	{
		pbxPos.Left = pos.Left;
	}
	else
	{
		if (style == ImageTop || style == ImageBottom)
		{
			if (tfSize.Width > pbxSize.Width)
			{
				tfPos.Left = pos.Left;
				//align at the mid
				pbxPos.Left = pos.Left + (sz.Width - pbxSize.Width)/2;
			}
			else
			{
				pbxPos.Left = pos.Left;
				//align at the mid
				tfPos.Left = pos.Left + (sz.Width - tfSize.Width)/2;
			}
		}
		else if (style == ImageLeft)
		{
			pbxPos.Left = pos.Left;
			tfPos.Left = pbxPos.Left + pbxSize.Width+CurrentTextToImageOffset;
		}
		else if (style == ImageRight)
		{
			tfPos.Left = pos.Left;
			pbxPos.Left = tfPos.Left + tfSize.Width+CurrentTextToImageOffset;
		}
	}

	switch(align.VAlign)
	{
	case ToTop:
		pos.Top = origin.Top - sz.Height;
		break;
	case CenterVertically:
		pos.Top = origin.Top- sz.Height/2;
		break;
	case ToBottom:
	default:
		//no change
		pos.Top = origin.Top;
	}

	//position control 's top
	if (style == TextOnly)
	{
		tfPos.Top = pos.Top;
	}
	if (style == ImageOnly)
	{
		pbxPos.Top = pos.Top;
	}
	else
	{
		if (style == ImageLeft || style == ImageRight)
		{
			if (tfSize.Height > pbxSize.Height)
			{
				tfPos.Top = pos.Top;
				//align at the mid
				pbxPos.Top = pos.Top +(sz.Height - pbxSize.Height)/2;
			}
			else
			{
				pbxPos.Top = pos.Top;
				//align at the mid
				tfPos.Top = pos.Top + (sz.Height - tfSize.Height)/2;
			}
		}
		else if (style == ImageTop)
		{
			pbxPos.Top = pos.Top;
			tfPos.Top = pbxPos.Top + pbxSize.Height + CurrentTextToImageOffset;
		}
		else if (style == ImageBottom)
		{
			tfPos.Top = pos.Top;
			pbxPos.Top = tfPos.Top + tfSize.Height + CurrentTextToImageOffset;
		}
	}

	//creating controls
	TextField* tf = NULL;
	PictureBox* pbx = NULL;
	Panel* pnl =NULL;
	IControl* drawingControl=NULL;

	if (hasText)
	{
		_tfFactory->CurrentTextFormatHandle = tfmtHandle;
		_tfFactory->CurrentWrap = FALSE;
		fres = _tfFactory->GetTextBox(tfPos, text, tfSize, true, &tf);
		//restore fmt
		_tfFactory->CurrentTextFormatHandle = _tfFactory->DefaultTextFormatHandle;
		_tfFactory->CurrentWrap = _tfFactory->DefaultWrap;
		ENSURESUCCESS(fres);
		drawingControl = tf;
	}

	if (hasImage)
	{
		fres = _pbxFactory->GetPictureBox(pbxSize, pbxPos, bmp, &pbx);
		ENSURESUCCESS(fres);
		drawingControl = pbx;
	}

	if (hasText && hasImage)
	{
		fres = _pnlFactory->GetPanel(sz, pos, 2, &pnl);
		ENSURESUCCESS(fres);
		fres = pnl->SetControl(pbx, 0);
		ENSURESUCCESS(fres);
		fres = pnl->SetControl(tf, 1);
		ENSURESUCCESS(fres);
		drawingControl = pnl;
	}

	//init menu Item
	MenuItem* mi;
	mi = allocMenuItem();
	FAILIF(mi==NULL);

	fres = mi->Init(name,tf, tfmt, pbx, drawingControl, handler, shrtcut);
	ENSURESUCCESS(fres);

	//Set result
	*o_mi = mi;
	return SUCCESS;
}

}

