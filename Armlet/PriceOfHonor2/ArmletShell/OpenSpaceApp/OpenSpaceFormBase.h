#pragma once

#define STRIPE_HEIGHT 16

typedef enum _FRM_STRIPE_STYLES {
	frmStripeNone,
	frmStripeOneBot,
	frmStripeOneTopOneBot,
	frmStripeThreeWide,
	frmStripeSevenNarrow,
} FormStripeStyles;

class OpenSpaceFormBase : public FormBase
{
protected:
	OpenSpaceApp* _App;
	OpenSpaceLogic* _Logic;

	char* _TitleText;
	char* _SubtitleText;

	fresult GetStripesPanel(FormStripeStyles stripeStyle, Panel** o_pnl);
	fresult BaseInit(Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, OpenSpaceApp* app, OpenSpaceLogic* logic);
	virtual fresult OnBeforeShow( IForm* prevFrom, bool_t reActivation );

public:
	
};