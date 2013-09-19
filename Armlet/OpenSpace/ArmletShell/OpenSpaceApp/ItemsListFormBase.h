#pragma once

class ItemsListFormBase : public OpenSpaceFormBase
{
protected:
	TextField* _txtItemHeader;
	TextField* _txtItemContent;

	char** _itemsTexts;
	ubyte_t _itemsCount;
	ubyte_t _currentIndex;

	fresult CreateMenu(IMenu** o_mnu);
	fresult BaseInit(char** itemsTexts, ubyte_t startIndex, ubyte_t itemsCount,
		Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, OpenSpaceApp* app, OpenSpaceLogic* logic);

	virtual fresult SetItem(ubyte_t index);
	virtual fresult SetItemHeader(char* headerText, HorizontalAlignmentStyles alignment, ubyte_t leftOffset);
	virtual MenuItemSettings* GetMenuSettings();

	fresult MoveToNext();
	fresult MoveToPrev();
public:
	virtual fresult DoLayout();

	virtual fresult OnBeforeShown( IForm* prevFrom, bool_t reActivation, FormShowResults formShowResult );


};