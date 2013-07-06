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
	fresult virtual	SetScrollPosition (Position position) =0;
	fresult virtual	ScrollUp () =0;
	fresult virtual	ScrollDown () =0;

	Position virtual GetScrollPosition() =0;
};

}
