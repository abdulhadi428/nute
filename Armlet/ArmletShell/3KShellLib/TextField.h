#pragma once
/* Text Field:
Setting Text
 1. Text is a padded with 0x0 rectangular text area
 2. Dimestions should be set by TextSize

Setting Format
 1. Text is a padded with 0x0 rectangular text area
 2. Dimestions cosidered to be TextSize
 
Setting Size:
 1. Size is viewable area of the text (i.e. "window" of the text currently shown on screen)
 2. Scroll position is a Top, Left origin in Text
 3. Size shouldn't exeed TextSize (Control is not able to expand the Text)

Setting ScrollPosition
 1. Set scroll position by SetScrollPosition
 2. Setting scroll Position is intelligent. So setting position so, that it (according to size) move out of Text - will set the Topmost (and Leftmost) possible coordinates

*/
#include "project.h"
#include "IRender.h"
#include "TextFormat.h"
#include "ScrollableControlBase.h"


#define DEFAULT_BACKGROUND 5
#define DEFAULT_FOREGROUND 10
#define DEFAULT_FONT 0


class TextField : public ScrollableControlBase
{

private:
	
	Size _BuffSizeTx;

	bool _readOnly;
	const char* _readOnlyText;
	char* _peadOnlyText;
	TextFormat _Format;

	Position _ScrollPositionTx;


	//Gets linear index in lines Buff by x,y
	//doesn't check bounds
	int GetBuffIndex(Position pos);

	//Gets Line staring at specified point. 
	//it returns pointer to the original buff, so it MUST be treated as const
	//NOTE:
	//If required length exceeds textSize.Width - returns GENERAL_WARNING and length is set to actual length
	fresult TextField::GetLineAtXY( Position pos, int* ioLength, char** oLine);

public:
	

	//getters
	const char* GetText();
	bool GetReadOnly();

	Size GetTextSize();
	TextFormat* GetTextFormat();

	//Setters

	void SetTextFormat(TextFormat* format);

	//Setting the text;
	fresult SetText(const char* text, Size textSize, bool readOnly,TextFormat* pFormat);

	//Setting the scroll window position intelligently (try find best fit if it will go off TextSize limits)
	// returns:
	//  SUCCESS: 
	//       - if position fits well
	//  GENERAL_WARNING
	//       - if position was adjusted to fit in TextSize (*positon parameter will contain adjusted values)
	//
	fresult virtual SetScrollPosition (Position position);

	//Initialization;
	fresult Init(Size size, Position position, IRender* renderer);

	//Logic
	
	//renders control partially to screen
	fresult virtual DrawArea(Position pos, Size size);

	Position virtual GetScrollPosition();

};
