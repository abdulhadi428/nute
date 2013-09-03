#include "ThreeKShell.h"
#include "Honor2.h"
#define MAX_HEADER_TOTAL_LEN 13
#define TITLE_MAX_OFFSET

fresult Honor2StatusBar::Init( Repositories* reps, Factories* facts)
{
	fresult fres;
	fres = BaseInit(reps, facts);
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

fresult Honor2StatusBar::CreateStatusBar()
{
	fresult fres;

	fres = StatusBarBase::CreateStatusBar();
	ENSURESUCCESS(fres);

	//TODO: img lists
	Position pbxBatteryPos;
	pbxBatteryPos.Left = 2;
	pbxBatteryPos.Top = 2;

	fres = AddBatteryLevel(pbxBatteryPos);
	ENSURESUCCESS(fres);
	
	fres = _BatteryLevel->SetValue(0);
	ENSURESUCCESS(fres);

	Position pbxNetworkPos;
	pbxNetworkPos.Left = pbxBatteryPos.Left + _BatteryLevel->GetSize().Width + 2;
	pbxNetworkPos.Top = pbxBatteryPos.Top;

	fres = AddNetworkLevel(pbxNetworkPos);
	ENSURESUCCESS(fres);

	fres = _NetworkLevel->SetValue(0);
	ENSURESUCCESS(fres);

	_headerMinLeft = pbxNetworkPos.Left + _NetworkLevel->GetSize().Width + 2;

	Size sbSz = GetSize();
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
	fres = tff->GetTextBox(posHeader, MAX_HEADER_TOTAL_LEN, &txtHeader);
	ENSURESUCCESS(fres);
	fres = _pnlBasePanel->AppendControl(txtHeader);
	ENSURESUCCESS(fres);

	//Subtitle
	tff->CurrentLines = 2;
	fres = tff->GetTextBox(posHeader, MAX_HEADER_TOTAL_LEN*2, &txtSubtitle);
	ENSURESUCCESS(fres);
	fres = _pnlBasePanel->AppendControl(txtSubtitle);
	ENSURESUCCESS(fres);

	tff->CurrentLines = tff->DefaultLines;
	tff->CurrentFrames = tff->DefaultFrames;
	tff->CurrentTextFormatHandle = tff->DefaultTextFormatHandle;

	return SUCCESS;
}

fresult Honor2StatusBar::SetTitle( char* szTitle, char* szSubtitle )
{
	fresult fres;
	//get max line length
	sword_t titleLen = Length(szTitle);

	sword_t subtitleCrPos = -1;
	subtitleCrPos = InStr(szSubtitle, "\n", 0);
	sword_t subtitleLen = 0;
	ubyte_t subtitleLines = 0;

	TextField* txtChosenSubtitle = NULL;
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

	FAILIF(titleLen+subtitleLen > 13);

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

	fres = txtSubtitle->SetSize(sz);
	ENSURESUCCESS(fres);

	Position pos;
	pos.Left = SCREENX - sz.Width;

	pos.Top = titleOriginTop - titleOriginOffset;

	fres = txtSubtitle->SetPosition(pos);
	ENSURESUCCESS(fres);

	fres = txtSubtitle->SetText(szSubtitle);
	ENSURESUCCESS(fres);
	
	//Positioning title
	subtitleWidth = subtitleLen*tf->Font.GlyphSize.Width;
	sz.Width = titleLen*tf->Font.GlyphSize.Width;
	sz.Height = tf->Font.GlyphSize.Height;
	fres = txtHeader->SetSize(sz);
	ENSURESUCCESS(fres);

	pos = txtHeader->GetPosition();
	pos.Left = SCREENX - sz.Width - subtitleWidth;
	pos.Top = titleOriginTop - tf->Font.GlyphSize.Height/2;

	fres = txtHeader->SetPosition(pos);
	
	fres = txtHeader->SetText(szTitle);
	ENSURESUCCESS(fres);

	fres = Draw();
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult Honor2StatusBar::GetBatteryImages( ImageHandle** o_batteryImages, ubyte_t* o_count )
{
	*o_batteryImages = _batteryImages;
	*o_count = _batteryImagesCount;
	return SUCCESS;
}

fresult Honor2StatusBar::GetNetworkImages( ImageHandle** o_networkImages, ubyte_t* o_count )
{
	*o_networkImages = _NetworkImages;
	*o_count = _batteryImagesCount;
	return SUCCESS;
}

ubyte_t Honor2StatusBar::GetControlsCount()
{
	return 5;
}

Size Honor2StatusBar::GetSize()
{
	Size sz;

	sz.Width = SCREENX;
	sz.Height = 25;

	return sz;
}

