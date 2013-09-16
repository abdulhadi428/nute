#include "ThreeKShell.h"
#include "To3KShell.h"

fresult FormBase::BaseInit( Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, ApplicationBase* app )
{
	FAILIF(reps==NULL);
	FAILIF(facts==NULL);
	FAILIF(name==NULL);
	FAILIF(frmmngr==NULL);
	FAILIF(app==NULL);

	_Repositories = reps;
	_Factories = facts;
	_FormManager = frmmngr;
	_Name = name;
	_AppBase = app;
	
	_FormPanel = NULL;
	_Menu = &_mnuInstance;
	_IsDialog = FALSE;

	_AutoDispatchMenuHandler = &(_autoDispatchMenuHandlerInstance);
	_AutoDispatchMenuHandler->Init(this, &FormBase::DefaultMenuHandler);

	return SUCCESS;
}

char* FormBase::GetName()
{
	return _Name;
}

fresult FormBase::OnButtonEvent( ButtonState key, bool* found)
{
	return _Menu->ProcessButton(key, found);
}

fresult FormBase::OnBeforeShown( IForm* prevFrom, bool_t reActivation, FormShowResults formShowResult )
{
	return SUCCESS;
}

fresult FormBase::OnAfterShown( IForm* prevFrom, bool_t reActivation, FormShowResults formShowResult )
{
	return SUCCESS;
}

fresult FormBase::OnLoad()
{
	return SUCCESS;
}

fresult FormBase::Draw()
{
	fresult fres;

	if (_FormPanel!=NULL)
	{
		fres = _FormPanel->Draw();
		ENSURESUCCESS(fres);
	}

	fres = _Menu->Draw();
	ENSURESUCCESS(fres);

	return SUCCESS;
}

bool_t FormBase::IsDialog()
{
	return _IsDialog;
}

fresult FormBase::OnAfterDialogShown( IForm* prevFrom, char* dialogName, FormShowResults results, bool* needRedraw )
{
	*needRedraw = TRUE;
	return SUCCESS;
}

fresult FormBase::DefaultMenuHandler( IMenuItem* mi )
{
	fresult fres;

	//exec virtual OnAnyMenu
	bool_t found = FALSE;
	fres = OnMenu(mi, &found);
	if (found == TRUE)
	{
		return fres;
	}

	//search menu handler
	char* miName = mi->GetName();
	MenuFactory* mf = _Factories->GetMenuFactory();
	FAILIF(mf==NULL);
	MenuItemSettings* mis = mf->Settings;

	//function pointer
	fresult (FormBase::*handler)(IMenuItem* mi);

	for(ubyte_t i=0; i<MenuItemOriginsLast;i++)
	{
		if (StringEquals(miName, mis[i].Name) == TRUE)
		{
			switch (i)
			{
			case ItemOriginA:
				handler = &FormBase::OnMenuA;
				break;
			case ItemOriginB:
				handler = &FormBase::OnMenuB;
				break;
			case ItemOriginC:
				handler = &FormBase::OnMenuC;
				break;
			case ItemOriginX:
				handler = &FormBase::OnMenuX;
				break;
			case ItemOriginY:
				handler = &FormBase::OnMenuY;
				break;
			case ItemOriginZ:
				handler = &FormBase::OnMenuZ;
				break;
			case ItemOriginL:
				handler = &FormBase::OnMenuL;
				break;
			case ItemOriginE:
				handler = &FormBase::OnMenuE;
				break;
			case ItemOriginR:
				handler = &FormBase::OnMenuR;
				break;
			default:
				return GENERAL_ERROR;
			}
			
			found = TRUE;
			break;
		}
	}

	if (found==TRUE)
	{
		//Call found handler
		fres = (this->*handler)(mi);
		ENSURESUCCESS(fres);
	}
	else
	{
		//call default
		fres = OnMenuOther(mi);
		ENSURESUCCESS(fres);
	}

	return SUCCESS;
}

fresult FormBase::CloseForm( FormShowResults formShowResult )
{
	return _FormManager->CloseForm(this, formShowResult);
}

fresult FormBase::HCenterTextField( TextField* tf, HorizontalAlignmentStyles alignment, uword_t chars, ubyte_t areaWidth, ubyte_t offsetLeft)
{
	fresult fres;
	
	ubyte_t requiredWidth;
	TextFormat* tfmt = tf->GetTextFormat();
	requiredWidth = chars*tfmt->Font.GlyphSize.Width;
	
	Size sz;
	sz = tf->GetSize();
	sz.Width = requiredWidth;
	fres = tf->SetSize(sz);
	ENSURESUCCESS(fres);

	return HCenterConrol(tf, alignment,requiredWidth, areaWidth, offsetLeft);
}

fresult FormBase::HCenterConrol( IControl* ctrl, HorizontalAlignmentStyles alignment, ubyte_t requiredWidth, ubyte_t areaWidth, ubyte_t offsetLeft)
{
	fresult fres;
	Position pos;
	pos = ctrl->GetPosition();
	if (alignment == CenterHorizontaly)
	{
		pos.Left = (areaWidth - requiredWidth) /2 - offsetLeft;
	}
	else if (alignment == ToLeft)
	{
		pos.Left = offsetLeft;
	}	
	else if (alignment ==  ToRight)
	{
		pos.Left = areaWidth - offsetLeft - requiredWidth;
	}

	fres = ctrl->SetPosition(pos);
	ENSURESUCCESS(fres);

	return SUCCESS;
}




