#ifndef _NAMESPACE_PROTECT_
	#error "Use ThreeKShell.h"
#endif

/* Text Field:
Setting Text
 1. Text is a padded with 0x0 rectangular text area
 2. Dimensions should be set by TextSize

Setting Format
 1. Text is a padded with 0x0 rectangular text area
 2. Dimensions considered to be TextSize
 
Setting Size:
 1. Size is viewable area of the text (i.e. "window" of the text currently shown on screen)
 2. Scroll position is a Top, Left origin in Text
 3. Size shouldn't exeed TextSize (Control is not able to expand the Text)

Setting ScrollPosition
 1. Set scroll position by SetScrollPosition
 2. Setting scroll Position is ubyte_telligent. So setting position so, that it (according to size) move out of Text - will set the Topmost (and Leftmost) possible coordinates

*/

#define ThreeK_DEFAULT_TEXTFIELD_BACKGROUND 5
#define ThreeK_DEFAULT_TEXTFIELD_FOREGROUND 10
#define ThreeK_DEFAULT_TEXTFIELD_FONT 1

class TextField : public ScrollableControlBase
{

private:
	
	Size _textBuffSizeTx;
	uword_t _buffLength;
	char* _textBuff;

	//for streamed textfield
	ITextStream* _textStream;
	bool_t _streamed;
	//buff offset in stream
	uword_t _streamPos;


	Position _textBuffCarretPositionTx;
	TextFormat _Format;
	
	bool_t _WordWrap;
	Position _ScrollPositionTx;
	ubyte_t _lastUsedLine;

	//Gets linear index in lines Buff by x,y
	//doesn't check bounds
	sword_t GetBuffIndex(Position pos);
	
	fresult PutCharToBuff( Position pos, char charToPut);

	//Gets Line staring at specified poubyte_t. 
	//it returns poubyte_ter to the original buff, so it MUST be treated as const
	//NOTE:
	//If required length exceeds textSize.Width - returns GENERAL_WARNING and length is set to actual length
	fresult GetLineAtXY( Position pos, ubyte_t* ioLength, char** oLine);

	fresult TextField::AppendText(const char* text, uword_t sz);

public:
	

	//getters
	TextFormat* GetTextFormat();

	bool_t GetWordWrap()
	{
		return _WordWrap;
	}

	//Setters

	void SetWordWrap(bool_t wordWrap)
	{
		_WordWrap = wordWrap;
	};

	void SetTextFormat(TextFormat* format);

	//clears the data
	fresult Clear();
	
	//Setting the text;
	fresult SetText(const char* text);

	//Setting text stream
	fresult SetTextStream(ITextStream* stream);

	//append the text to current caret pos
	fresult AppendText(const char* text);

	//Setting the scroll window position ubyte_telligently (try find best fit if it will go off TextSize limits)
	// returns:
	//  SUCCESS: 
	//       - if position fits well
	//  GENERAL_WARNING
	//       - if position was adjusted to fit in TextSize (*positon parameter will contain adjusted values)
	//
	virtual fresult SetScrollPosition (Position position);

	//Initialization;
	fresult Init(Size size, Position position, char* buff, Size buffSize, IRender* renderer);

	//Logic
	
	//renders control partially to screen
	virtual fresult DrawArea(Position pos, Size size);

	virtual Position GetScrollPosition();

	virtual fresult ScrollUp();

	virtual fresult ScrollDown();

};
