#pragma once

class PlayMusicForm : public OpenSpaceFormBase
{

	MenuItem* _miHelplessness;
	MenuItem* _miEgo;
	MenuItem* _miCruelty;
	MenuItem* _miVolumeUp;
	MenuItem* _miVolumeDown;
	StressType _playingStress;
	StressType _triedPlayStress;

	fresult CreateMenu(IMenu** o_mnu);

	bool_t _audioReady;
	virtual fresult OnMenuA( IMenuItem* mi );
	virtual fresult OnMenuC( IMenuItem* mi );
	virtual fresult OnMenuY( IMenuItem* mi );
	virtual fresult OnMenuL( IMenuItem* mi );
	virtual fresult OnMenuR( IMenuItem* mi );
	virtual fresult OnMenuE( IMenuItem* mi );

	fresult RestoreCaptionAfterStop();
	fresult HandleMenuHit(StressType stress);

	fresult PlayMusic(StressType stress);
	fresult StopPlayingMusic();

	virtual fresult OnAfterDialogShown( IForm* prevFrom, char* dialogName, FormShowResults results, bool* needRedraw );
	virtual fresult OnBeforeShown( IForm* prevFrom, bool_t reActivation, FormShowResults formShowResult );

public:
	virtual fresult DoLayout();
	fresult Init(Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, OpenSpaceApp* app, OpenSpaceLogic* logic);
	fresult UpdateMenu();

};