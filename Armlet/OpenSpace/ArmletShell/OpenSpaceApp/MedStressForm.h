#pragma once

class MedStressForm : public OpenSpaceFormBase
{
	fresult CreateMidContent(Panel** o_mainStatus);
	fresult CreateMenu(IMenu** o_mnu);

	fresult HandleIncreaseHelplessness(IMenuItem* mi);
	fresult HandleDecreaseHelplessness(IMenuItem* mi);

	fresult HandleIncreaseEgo(IMenuItem* mi);
	fresult HandleDecreaseEgo(IMenuItem* mi);

	fresult HandleIncreaseCruelty(IMenuItem* mi);
	fresult HandleDecreaseCruelty(IMenuItem* mi);


public:
	virtual fresult DoLayout();
	fresult Init(Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, OpenSpaceApp* app, OpenSpaceLogic* logic);

	virtual fresult OnMenuA( IMenuItem* mi );

};