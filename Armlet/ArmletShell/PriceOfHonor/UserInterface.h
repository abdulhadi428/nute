#pragma once
#include "UIDesigner.h"

class UserInterface : public UIDesigner
{
public: 
	fresult Init();

	fresult virtual OnMainMnuHelp(IMenuItem* sender);
	fresult virtual OnMainMnuScrollUp(IMenuItem* sender);
	fresult	virtual OnMainMnuScrollDown(IMenuItem* sender);
	fresult virtual OnMainMnuCycleLeft(IMenuItem* sender);
	fresult virtual OnMainMnuShot(IMenuItem* sender);
	fresult virtual OnMainMnuKnockout(IMenuItem* sender);
	fresult virtual OnMainMnuOpenLock(IMenuItem* sender);
	fresult virtual OnMainMnuCycleRight(IMenuItem* sender);

	fresult virtual OnButtonPressed(ButtonState button);

	fresult virtual OnWoundMnuHead( IMenuItem* sender );
	fresult virtual OnWoundMnuStomach( IMenuItem* sender );
	fresult virtual OnWoundMnuLeftArm( IMenuItem* sender );
	fresult virtual OnWoundMnuRightArm( IMenuItem* sender );
	fresult virtual OnWoundMnuChest( IMenuItem* sender );
	fresult virtual OnWoundMnuBack( IMenuItem* sender );
	fresult virtual OnWoundMnuLeftLeg( IMenuItem* sender );
	fresult virtual OnWoundMnuRightLeg( IMenuItem* sender );


};