#include "ThreeKShell.h"
#include "OpenSpace.h"

#define MAX_HEADER_TOTAL_LEN 20
#define TITLE_MAX_OFFSET

fresult OpenSpaceStatusBar::Init(Size sz, Repositories* reps, Factories* facts)
{
	fresult fres;
	fres = BaseInit(sz, reps, facts);
	ENSURESUCCESS(fres);

	_batteryImagesCount =5;
	_batteryImages[0] = battery_0;
	_batteryImages[1] = battery_1;
	_batteryImages[2] = battery_2;
	_batteryImages[3] = battery_3;
	_batteryImages[4] = battery_4;

	_networkImagesCount =5;
	_NetworkImages[0] = signal_0;
	_NetworkImages[1] = signal_1;
	_NetworkImages[2] = signal_2;
	_NetworkImages[3] = signal_3;
	_NetworkImages[4] = signal_4;

	fres = CreateStatusBar();
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult OpenSpaceStatusBar::CreateStatusBar()
{
	fresult fres;

	fres = StatusBarBase::CreateStatusBar();
	ENSURESUCCESS(fres);

	Position pbxBatteryPos;
	pbxBatteryPos.Left = 2;
	pbxBatteryPos.Top = 2;

	fres = AddBatteryLevel(pbxBatteryPos);
	ENSURESUCCESS(fres);
	
	Position pbxNetworkPos;
	pbxNetworkPos.Left = pbxBatteryPos.Left + _imglBatteryLevel->GetSize().Width + 2;
	pbxNetworkPos.Top = pbxBatteryPos.Top;

	fres = AddNetworkLevel(pbxNetworkPos);
	ENSURESUCCESS(fres);

	Size sbSz = GetSize();

	Position clockPos;
	clockPos.Left = sbSz.Width - 5*6-1;
	clockPos.Top = (sbSz.Height - 8)/2;
	fres = AddClock(clockPos);
	ENSURESUCCESS(fres);

	_headerMinLeft = pbxNetworkPos.Left + _imglNetworkLevel->GetSize().Width + 2;

	Size szHeader;
	szHeader.Height = sbSz.Height;
	szHeader.Width = sbSz.Width/2;

	Position posHeader;
	posHeader.Top = pbxBatteryPos.Top;
	posHeader.Left = _headerMinLeft;

	TextFieldFactory *tff = _Factories->GetTextFieldFactory();
	tff->CurrentTextFormatHandle = TF_HEADER;
	tff->CurrentFrames = 1;

	//Title
	fres = tff->GetTextBox(posHeader, MAX_HEADER_TOTAL_LEN, &_txtHeader);
	ENSURESUCCESS(fres);
	fres = _pnlBasePanel->AppendControl(_txtHeader);
	ENSURESUCCESS(fres);

	//Subtitle
	tff->CurrentLines = 2;
	Size pxSize;
	pxSize.Width = MAX_HEADER_TOTAL_LEN;
	pxSize.Height = 2;
	fres = tff->GetTextBox(posHeader, pxSize, 1, &_txtSubtitle);
	ENSURESUCCESS(fres);
	fres = _pnlBasePanel->AppendControl(_txtSubtitle);
	ENSURESUCCESS(fres);

	tff->CurrentLines = tff->DefaultLines;
	tff->CurrentFrames = tff->DefaultFrames;
	tff->CurrentTextFormatHandle = tff->DefaultTextFormatHandle;

	return SUCCESS;
}

fresult OpenSpaceStatusBar::SetTitle( char* szTitle, char* szSubtitle )
{
	fresult fres;
	//get max line length
	sword_t titleLen = Length(szTitle);

	sword_t subtitleCrPos = -1;
	subtitleCrPos = InStr(szSubtitle, "\n", 0);
	sword_t subtitleLen = 0;
	ubyte_t subtitleLines = 0;

	if (subtitleCrPos == -1)
	{
		subtitleLen = Length(szSubtitle);
		subtitleLines = 1;
	}
	else
	{
		sword_t line1Len = subtitleCrPos;
		sword_t line2Len = Length(szSubtitle) - subtitleCrPos;

		subtitleLen = line1Len > line2Len?line1Len:line2Len;
		subtitleLines = 2;
	}

	FAILIF(titleLen+subtitleLen > MAX_HEADER_TOTAL_LEN);

	//get tf
	TextFormat* tf = NULL;
	fres =_Repositories->TextFormats->GetTextFormat(TF_HEADER, &tf);
	ENSURESUCCESS(fres);

	Size statusSz = _pnlBasePanel->GetSize();


	//Positioning subtitle
	ubyte_t subtitleWidth = subtitleLen*tf->Font.GlyphSize.Width;
	Size sz ;
	sz.Width = subtitleWidth;


	ubyte_t titleOriginTop = statusSz.Height/2;
	ubyte_t titleOriginOffset = 0;
	if (subtitleLines ==1)
	{
		titleOriginOffset = (tf->Font.GlyphSize.Height)/2;
		sz.Height = tf->Font.GlyphSize.Height;
	}
	else
	{
		titleOriginOffset = tf->Font.GlyphSize.Height;
		sz.Height = tf->Font.GlyphSize.Height*2;
	}

	fres = _txtSubtitle->SetSize(sz);
	ENSURESUCCESS(fres);

	Position pos;
	pos.Left = _Size.Width - sz.Width-1;

	pos.Top = titleOriginTop - titleOriginOffset;

	fres = _txtSubtitle->SetPosition(pos);
	ENSURESUCCESS(fres);

	fres = _txtSubtitle->SetText(szSubtitle);
	ENSURESUCCESS(fres);
	
	//Positioning title
	subtitleWidth = subtitleLen*tf->Font.GlyphSize.Width;
	sz.Width = titleLen*tf->Font.GlyphSize.Width;
	sz.Height = tf->Font.GlyphSize.Height;
	fres = _txtHeader->SetSize(sz);
	ENSURESUCCESS(fres);

	pos = _txtHeader->GetPosition();
	pos.Left = SCREENX - sz.Width - subtitleWidth-2;
	pos.Top = titleOriginTop - tf->Font.GlyphSize.Height/2;

	fres = _txtHeader->SetPosition(pos);
	
	fres = _txtHeader->SetText(szTitle);
	ENSURESUCCESS(fres);

	fres = Draw();
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult OpenSpaceStatusBar::GetBatteryImages( ImageHandle** o_batteryImages, ubyte_t* o_count )
{
	*o_batteryImages = _batteryImages;
	*o_count = _batteryImagesCount;
	return SUCCESS;
}

fresult OpenSpaceStatusBar::GetNetworkImages( ImageHandle** o_networkImages, ubyte_t* o_count )
{
	*o_networkImages = _NetworkImages;
	*o_count = _batteryImagesCount;
	return SUCCESS;
}

ubyte_t OpenSpaceStatusBar::GetControlsCount()
{
	return 6;
}

fresult OpenSpaceStatusBar::DisplayMode( OpenSpaceStatusBarDisplayModes displayMode )
{
	if (displayMode == sbdmClock)
	{
		_txtClock->SetVisible(TRUE);
		_txtHeader->SetVisible(FALSE);
		_txtSubtitle->SetVisible(FALSE);
	}
	else
	{
		_txtClock->SetVisible(FALSE);
		_txtHeader->SetVisible(TRUE);
		_txtSubtitle->SetVisible(TRUE);
	}

	return SUCCESS;
}

