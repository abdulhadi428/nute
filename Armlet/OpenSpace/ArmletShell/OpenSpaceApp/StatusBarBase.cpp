#include "ThreeKShell.h"
#include "StatusBarBase.h"

fresult StatusBarBase::BaseInit( Repositories* reps, Factories* facts)
{
	_Repositories = reps;
	_Factories = facts;
	return SUCCESS;
}

Size StatusBarBase::GetSize()
{
	Size sz;
	sz.data = 0;

	if (_pnlBasePanel != NULL)
		sz = _pnlBasePanel->GetSize();

	return sz;
}

fresult StatusBarBase::SetHeaderText( char* headerText )
{
	fresult fres;
	
	if (_txtHeader != NULL)
	{
		fres = _txtHeader->SetText(headerText);
		ENSURESUCCESS(fres);
		fres = _txtHeader->Draw();
		ENSURESUCCESS(fres);
	}

	return SUCCESS;
}

fresult StatusBarBase::Draw()
{
	fresult fres;
	if (_pnlBasePanel !=NULL)
	{
		fres = _pnlBasePanel->Draw();
	}

	return SUCCESS;
}
