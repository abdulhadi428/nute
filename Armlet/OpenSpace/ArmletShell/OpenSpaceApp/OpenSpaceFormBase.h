#pragma once

#define STRIPE_HEIGHT 16

typedef enum _FRM_STRIPE_STYLES {
	frmStripeNone,
	frmStripeOneBot,
	frmStripeOneTopOneBot,
	frmStripeMessageYNDialog,
	frmStripeThreeWide,
	frmStripeSevenNarrow,
} FormStripeStyles;

class OpenSpaceFormBase : public FormBase
{
protected:
	OpenSpaceApp* _App;
	OpenSpaceLogic* _Logic;
	OpenSpaceStatusBarDisplayModes _StatusBarDisplayMode;

	char* _TitleText;
	char* _SubtitleText;
	FormStripeStyles _StripeStyle;

	fresult GetStripesPanel(FormStripeStyles stripeStyle, Panel** o_pnl);
	fresult GetContentDimensions(Position* o_pos, Size* o_size);
	fresult BaseInit(OpenSpaceStatusBarDisplayModes statusBarDisplayMode, Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, OpenSpaceApp* app, OpenSpaceLogic* logic);
	virtual fresult OnBeforeShown(IForm* prevFrom, bool_t reActivation, FormShowResults formShowResult);

public:
	
};