#include "ThreeKShell.h"
#include "OpenSpace.h"

#define MAX_HEADER_TOTAL_LEN 13
#define TITLE_MAX_OFFSET

fresult OpenSpaceStatusBar::Init( Repositories* reps, Factories* facts)
{
	fresult fres;
	fres = BaseInit(reps, facts);
	ENSURESUCCESS(fres);

	fres = CreateStatusBar();
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult OpenSpaceStatusBar::CreateStatusBar()
{
	fresult fres;
	Size sbSz;
	sbSz.Width = SCREENX;
	sbSz.Height = 16;

	Position sbPos;
	sbPos.data = 0;
	
	fres = _Factories->GetPanelFactory()->GetPanel(sbSz, sbPos, 6,CL_HEADER_BACKGROUND, &_pnlBasePanel);
	ENSURESUCCESS(fres);

	//TODO: img lists
	PictureBox* pbxBattery;
	Position pbxBatteryPos;
	pbxBatteryPos.Left = 2;
	pbxBatteryPos.Top = 2;

	fres = _Factories->GetPictureBoxFactory()->GetPictureBox(pbxBatteryPos, battery_stub, &pbxBattery);
	ENSURESUCCESS(fres);

	_pnlBasePanel->SetControl(pbxBattery, 0);
	ENSURESUCCESS(fres);

	PictureBox* pbxNetwork;
	Position pbxNetworkPos;
	pbxNetworkPos.Left = pbxBatteryPos.Left + pbxBattery->GetSize().Width + 2;
	pbxNetworkPos.Top = pbxBatteryPos.Top;

	fres = _Factories->GetPictureBoxFactory()->GetPictureBox(pbxNetworkPos, net_stub, &pbxNetwork);
	ENSURESUCCESS(fres);

	_pnlBasePanel->SetControl(pbxNetwork, 1);
	ENSURESUCCESS(fres);

	_headerMinLeft = pbxNetworkPos.Left + pbxNetwork->GetSize().Width + 2;

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
	fres = _pnlBasePanel->SetControl(txtHeader, 3);
	ENSURESUCCESS(fres);

	//Subtitle
	fres = tff->GetTextBox(posHeader, MAX_HEADER_TOTAL_LEN, &txtSubtitle);
	ENSURESUCCESS(fres);
	fres = _pnlBasePanel->SetControl(txtSubtitle, 4);
	ENSURESUCCESS(fres);

	fres = tff->GetTextBox(posHeader, MAX_HEADER_TOTAL_LEN, &txtSubtitle2Line);
	ENSURESUCCESS(fres);
	txtSubtitle2Line->SetVisible(FALSE);
	fres = _pnlBasePanel->SetControl(txtSubtitle2Line, 5);
	ENSURESUCCESS(fres);


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

	TextField* txtChosenSubtitle = NULL;
	if (subtitleCrPos == -1)
	{
		subtitleLen = Length(szSubtitle);
		txtChosenSubtitle = txtSubtitle;
		txtSubtitle2Line->SetVisible(FALSE);
		txtSubtitle->SetVisible(TRUE);
	}
	else
	{
		sword_t line1Len = subtitleCrPos;
		sword_t line2Len = Length(szSubtitle) - subtitleCrPos;

		subtitleLen = line1Len > line2Len?line1Len:line2Len;
		txtChosenSubtitle = txtSubtitle2Line;
		txtSubtitle2Line->SetVisible(TRUE);
		txtSubtitle->SetVisible(FALSE);
	}

	FAILIF(titleLen+subtitleLen > 13);

	//get tf
	TextFormat* tf = NULL;
	fres =_Repositories->TextFormats->GetTextFormat(TF_HEADER, &tf);
	ENSURESUCCESS(fres);

	//Positioning subtitle
	ubyte_t subtitleWidth = 0;
	Size sz = txtChosenSubtitle->GetSize();
	subtitleWidth = subtitleLen*tf->Font.GlyphSize.Width;
	sz.Width = subtitleWidth;

	fres = txtChosenSubtitle->SetSize(sz);
	ENSURESUCCESS(fres);

	Position pos = txtChosenSubtitle->GetPosition();
	pos.Left = SCREENX - sz.Width;

	fres = txtChosenSubtitle->SetPosition(pos);
	ENSURESUCCESS(fres);

	fres = txtChosenSubtitle->SetText(szSubtitle);
	ENSURESUCCESS(fres);
	
	//Positioning title
	sz = txtHeader->GetSize();
	subtitleWidth = titleLen*tf->Font.GlyphSize.Width;
	sz.Width = subtitleWidth;

	fres = txtHeader->SetSize(sz);
	ENSURESUCCESS(fres);

	pos = txtHeader->GetPosition();
	pos.Left = SCREENX - sz.Width - subtitleWidth;

	fres = txtHeader->SetPosition(pos);
	
	fres = txtHeader->SetText(szTitle);
	ENSURESUCCESS(fres);

	fres = Draw();
	ENSURESUCCESS(fres);

	return SUCCESS;
}

