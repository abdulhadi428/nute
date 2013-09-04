#include "ThreeKShell.h"
#include "To3KShell.h"
#include "Honor2.h"

fresult Honor2FormBase::BaseInit(bool_t showHeader, Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, Honor2App* app, Honor2Logic* logic)
{
	_App = app;
	_Logic = logic;
	_ShowHeader = showHeader;
	
	
	if (_ShowHeader)
	{
		_FormClientSize.Width = _App->GetDisplaySize().Width;
		_FormClientSize.Height = _App->GetDisplaySize().Height - HEADER_HEIGHT;
	}
	else
	{
		_FormClientSize.data = _App->GetClientAreaSize().data;
	}
	return FormBase::BaseInit(reps, facts, name, frmmngr, app);
}

 fresult Honor2FormBase::OnBeforeShow( IForm* prevFrom, bool_t reActivation )
{
	fresult fres;
	if (_ShowHeader)
	{
		_pnlPanelHeader->SetVisible(TRUE);
		fres = _App->AppStatusBar->SetVisible(FALSE);
	}
	else
	{
		fres = _App->AppStatusBar->SetVisible(TRUE);
		if (_pnlPanelHeader != NULL)
		{
			_pnlPanelHeader->SetVisible(FALSE);
		}
	}
	return SUCCESS;
 }

 fresult Honor2FormBase::CreateHeader(ImageHandle headerIconHandle, char* headerTitleText, char* headerSubtitleText)
 {
	fresult fres;

	Size clSz;
	Size headerSz;
	Position headerPos;

	Size sz;
	Position pos;

	clSz = _App->GetClientAreaSize();
	headerSz.Width = clSz.Width;
	headerSz.Height = HEADER_HEIGHT;
	headerPos.data =0;
	fres = _Factories->GetPanelFactory()->GetPanel(headerSz, headerPos, HeaderControlsCount(), CL_HEADER_BACKGROUND, &_pnlPanelHeader);
	ENSURESUCCESS(fres);
	_FormPanel->AppendControl(_pnlPanelHeader);
	ENSURESUCCESS(fres);

	sz.Width = 24;
	sz.Height =HEADER_HEIGHT;
	pos.Left = headerPos.Left +1;
	pos.Top = 0;
	fres = _Factories->GetPictureBoxFactory()->GetPictureBox(sz, pos, headerIconHandle, &_pbxHeaderIcon);
	ENSURESUCCESS(fres);
	fres = _pnlPanelHeader->AppendControl(_pbxHeaderIcon);
	ENSURESUCCESS(fres);

	TextFieldFactory* tff = _Factories->GetTextFieldFactory();
	tff->CurrentTextFormatHandle = TF_HEADER;

	pos.Left = pos.Left + 24 +1;
	pos.Top =0;
	fres = _Factories->GetTextFieldFactory()->GetTextBox(pos, 22, &_txtHeaderTitle);
	ENSURESUCCESS(fres);
	fres = _pnlPanelHeader->AppendControl(_txtHeaderTitle);
	ENSURESUCCESS(fres);
	fres = _txtHeaderTitle->SetText(headerTitleText);
	ENSURESUCCESS(fres);

	pos.Left = pos.Left;
	pos.Top = 16;
	fres = _Factories->GetTextFieldFactory()->GetTextBox(pos, 22, &_txtHeaderSubTitle);
	ENSURESUCCESS(fres);
	fres = _pnlPanelHeader->AppendControl(_txtHeaderSubTitle);
	ENSURESUCCESS(fres);
	fres = _txtHeaderSubTitle->SetText(headerSubtitleText);
	ENSURESUCCESS(fres);

	tff->CurrentTextFormatHandle = tff->DefaultTextFormatHandle;

	return SUCCESS;
 }


 ubyte_t Honor2FormBase::HeaderControlsCount()
 {
	return 3;
 }
