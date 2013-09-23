#include "OpenSpace.h"
#include "ArmletApi.h"

#define MNU_NEXT_TEXT "следующий"
#define MNU_PREV_TEXT "предыдущий"

#define MNU_NEXT_TEXT_CRIT "   да!"
#define MNU_PREV_TEXT_CRIT " отложить"

fresult BehaviorForm::Init( Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, OpenSpaceApp* app, OpenSpaceLogic* logic )
{
	fresult fres;
	_TitleText = "Поведение/";
	_subtitleTextBuff[8]=0;
	_SubtitleText = _subtitleTextBuff;

	_nextText = MNU_NEXT_TEXT;
	_prevText = MNU_PREV_TEXT;

	fres = BaseInit(sbdmTitles, reps,facts, name, frmmngr, app, logic);
	ENSURESUCCESS(fres);

	_currentItemIndex =-1;
	_itemsCount =0;

	return SUCCESS;
}

fresult BehaviorForm::DoLayout()
{
	fresult fres;

	fres = ItemsListFormBase::DoLayout();
	ENSURESUCCESS(fres);

	Size caSz = _App->GetClientAreaSize();
	Position caPos = _App->GetClientAreaPos();

	//get message title
	TextFieldFactory* tff = _Factories->GetTextFieldFactory();
	FAILIF(tff==NULL);

	Position posTitle;
	posTitle.Left = 20;
	posTitle.Top = caPos.Top+4;

	tff->CurrentTextFormatHandle = TF_REDTEXT;
	fres = tff->GetTextBox(posTitle, "Новый!", &_txtNewPattern);
	ENSURESUCCESS(fres);
	fres = _FormPanel->AppendControl(_txtNewPattern);
	ENSURESUCCESS(fres);

	PictureBoxFactory* pbxf = _Factories->GetPictureBoxFactory();
	FAILIF(pbxf==NULL);
	
	Position posMusic;
	posMusic.Left = 6;
	posMusic.Top = caPos.Top+2;

	fres = pbxf->GetPictureBox(posMusic, music_button, &_pbxMusic);
	ENSURESUCCESS(fres);
	fres = _FormPanel->AppendControl(_pbxMusic);
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult BehaviorForm::UpdateMenuSettings( MenuItemSettings* ms )
{
	fresult fres;
	MenuItemSettings* mis;

	//ItemOriginA
	mis = &(ms[ItemOriginA]);
	mis->ImgHandle = small_arrow_left;
	fres = _FormManager->GetOpenFormHandler(_App->Forms->PlayMusicFormName, &mis->Handler);
	ENSURESUCCESS(fres);
	mis->Empty = FALSE;

	return SUCCESS;
}

fresult BehaviorForm::OnBeforeShown( IForm* prevFrom, bool_t reActivation, FormShowResults formShowResult )
{
	fresult fres;
	if (reActivation == FALSE)
	{
		//count active patterns
		_itemsCount =0;
		for (ubyte_t i =0; i< bhpLast;i++)
		{
			if (_App->Logic->BehaviorLogic->Patterns[i].IsActive)
			{
				_itemsCount++;
			}
		}
		
		_currentItemIndex = GetNextItemIndex(-1);
		
		fres = SetActiveItem(_currentItemIndex);
		ENSURESUCCESS (fres);

	}

	fres = ItemsListFormBase::OnBeforeShown(prevFrom, reActivation, formShowResult);
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult BehaviorForm::UpdateFormSubtite()
{
	fresult fres;

	ubyte_t displayedIndex = _currentItemIndex;
	//Count displayed index
	for (sbyte_t i=_currentItemIndex; i >=0 ; i--)
	{
		//Skipping non-active items
		if (_App->Logic->BehaviorLogic->Patterns[i].IsActive == FALSE)
		{
			displayedIndex--;
		}
	}


	ArmletApi::snprintf(_subtitleTextBuff, 8, "%d из %d", displayedIndex+1, _itemsCount);

	fres = _App->AppStatusBar->SetTitle("Поведение/", _subtitleTextBuff);
	ENSURESUCCESS(fres);

	fres = _App->AppStatusBar->Draw();
	ENSURESUCCESS(fres);

	return SUCCESS;
}

sbyte_t BehaviorForm::GetNextItemIndex( sbyte_t index )
{
	sbyte_t resIndex =-1;
	
	for (sbyte_t i=index+1;i < bhpLast; i++)
	{
		if (_App->Logic->BehaviorLogic->Patterns[i].IsActive == TRUE)
		{
			resIndex = i;
			break;
		}
	}

	return resIndex;
}

sbyte_t BehaviorForm::GetPrevItemIndex( sbyte_t index )
{
	sbyte_t resIndex =-1;

	for (sbyte_t i=index-1;i >= 0; i--)
	{
		if (_App->Logic->BehaviorLogic->Patterns[i].IsActive == TRUE)
		{
			resIndex = i;
			break;
		}
	}

	return resIndex;
}

fresult BehaviorForm::SetActiveItem( sbyte_t index )
{
	fresult fres;
	if (index!=-1)
	{
		BehaviourPatternDescription* pattern = &(_App->Logic->BehaviorLogic->Patterns[index]);

		fres = SetItemText(pattern->Text);
		ENSURESUCCESS(fres);

		fres = SetItemHeader(pattern->Title, ToRight, 2);
		ENSURESUCCESS(fres);

		TextField* txtBack = _mnuBack->GetTextField();
		FAILIF(txtBack==NULL);
		TextField* txtNext = _mnuNext->GetTextField();
		FAILIF(txtNext==NULL);

		char* nextText = NULL;
		char* prevText = NULL;
		TextFormat* tfmtNext = NULL;
		bool_t newVisible = FALSE;

		if (pattern->IsCritical && pattern->IsNew)
		{

			nextText = MNU_NEXT_TEXT_CRIT;
			prevText = MNU_PREV_TEXT_CRIT;
			_Repositories->TextFormats->GetTextFormat(TF_MENU_RED, &tfmtNext);
			ENSURESUCCESS(fres);

			fres = _txtItemContent->AppendText("\n");
			ENSURESUCCESS(fres);

			fres = _txtItemContent->AppendText(pattern->CritText);
			ENSURESUCCESS(fres);

			newVisible = TRUE;

			_currentItemIndex = index;
		}
		else
		{
			nextText = MNU_NEXT_TEXT;
			prevText = MNU_PREV_TEXT;
			_Repositories->TextFormats->GetTextFormat(TF_MENU, &tfmtNext);
			ENSURESUCCESS(fres);

			fres = ItemsListFormBase::SetActiveItem(index);
			ENSURESUCCESS(fres);
			
			newVisible = FALSE;
		}

		fres = txtBack->SetText(prevText);
		ENSURESUCCESS(fres);

		fres = txtNext->SetText(nextText);
		ENSURESUCCESS(fres);

		txtNext->SetTextFormat(tfmtNext);
		_currentItemIndex = index;

		_txtNewPattern->SetVisible(newVisible);
	}
	else
	{
		_currentItemIndex = -1;
	}
	
	fres = UpdateFormSubtite();
	ENSURESUCCESS(fres);

	fres = _App->RedrawIfActive(this);
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult BehaviorForm::OnMenuL( IMenuItem* mi )
{
	fresult fres;
	
	if (_App->Logic->BehaviorLogic->Patterns[_currentItemIndex].IsNew && _App->Logic->BehaviorLogic->Patterns[_currentItemIndex].IsCritical)
	{
		//Should act as Next
		fres = ItemsListFormBase::OnMenuR(mi);
	}
	else
	{
		fres = ItemsListFormBase::OnMenuL(mi);
		ENSURESUCCESS(fres);
	}

	return SUCCESS;
}

fresult BehaviorForm::OnMenuR( IMenuItem* mi )
{
	fresult fres;

	//now no pattern won't be new 
	_App->Logic->BehaviorLogic->Patterns[_currentItemIndex].IsNew = FALSE;
	fres = ItemsListFormBase::OnMenuR(mi);
	ENSURESUCCESS(fres);

	return SUCCESS;
}





