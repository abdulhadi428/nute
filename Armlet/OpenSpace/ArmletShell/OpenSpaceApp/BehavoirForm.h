#pragma once

class BehaviorForm : public ItemsListFormBase
{

	TextField* _txtNewPattern;
	PictureBox* _pbxMusic;

	virtual fresult UpdateMenuSettings( MenuItemSettings* ms );

	virtual fresult DoLayout();

	char _subtitleTextBuff[9];
	fresult UpdateFormSubtite();

	virtual fresult OnBeforeShown( IForm* prevFrom, bool_t reActivation, FormShowResults formShowResult );
	virtual sbyte_t GetNextItemIndex( sbyte_t index );
	virtual sbyte_t GetPrevItemIndex( sbyte_t index );
	virtual fresult SetActiveItem( sbyte_t index );
	virtual fresult OnMenuL( IMenuItem* mi );
	virtual fresult OnMenuR( IMenuItem* mi );

public:
	fresult Init(Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, OpenSpaceApp* app, OpenSpaceLogic* logic);

	

};