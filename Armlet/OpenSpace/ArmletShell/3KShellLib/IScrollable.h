#pragma once

namespace ThreeKShell {

class IScrollable
{
public:
	//Setting the scroll window position intelligently (try find best fit if it will go off TextSize limits)
	// returns:
	//  SUCCESS: 
	//       - if position fits well
	//  GENERAL_WARNING
	//       - if position was adjusted to fit in TextSize (*positon parameter will contain adjusted values)
	//
	virtual fresult SetScrollPosition (Position position) =0;
	virtual	fresult ScrollUp () =0;
	virtual	fresult ScrollDown () =0;

	virtual Position GetScrollPosition() =0;
};

}
