#pragma once

class ItemsListFormBase : public OpenSpaceFormBase
{
protected:
	TextField* _txtItemHeader;
	TextField* _txtItemContent;

	ubyte_t _currentItemIndex;
	ubyte_t _itemsCount;

	fresult CreateMenu(IMenu** o_mnu);

	virtual fresult UpdateMenuSettings(MenuItemSettings* ms);

	virtual sbyte_t GetNextItemIndex(sbyte_t index);
	virtual sbyte_t GetPrevItemIndex(sbyte_t index);

	virtual fresult SetActiveItem(sbyte_t index);

	virtual fresult SetItemText(char* itemText);
	virtual fresult SetItemHeader(char* headerText, HorizontalAlignmentStyles alignment, ubyte_t leftOffset);

	virtual fresult OnMenuL( IMenuItem* mi );
	virtual fresult OnMenuR( IMenuItem* mi );

	MenuItem* _mnuBack;
	MenuItem* _mnuNext;

	char* _nextText;
	char* _prevText;

public:
	virtual fresult DoLayout();

	virtual fresult OnMenuX( IMenuItem* mi );

	virtual fresult OnMenuZ( IMenuItem* mi );


};