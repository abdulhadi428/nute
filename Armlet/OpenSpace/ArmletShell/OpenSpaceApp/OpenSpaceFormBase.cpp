#include "ThreeKShell.h"
#include "To3KShell.h"
#include "OpenSpace.h"

fresult OpenSpaceFormBase::BaseInit(OpenSpaceStatusBarDisplayModes statusBarDisplayMode,Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, OpenSpaceApp* app, OpenSpaceLogic* logic)
{
	_App = app;
	_Logic = logic;
	_StatusBarDisplayMode = statusBarDisplayMode;
	return FormBase::BaseInit(reps, facts, name, frmmngr, app);
}

fresult OpenSpaceFormBase::GetStripesPanel(FormStripeStyles stripeStyle, Panel** o_pnl)
{
	fresult fres;

	Panel* pnl;

	if (stripeStyle==frmStripeNone)
	{
		*o_pnl = NULL;
		return SUCCESS;
	}
	
	Panel* stripes[7];
	for (ubyte_t i=0;i<7;i++)
	{
		stripes[i] = NULL;
	}
	
	Size caSz = _App->GetClientAreaSize();
	Position caPos = _App->GetClientAreaPos();

	Size pnlNarrowSz;
	pnlNarrowSz.Width = caSz.Width;
	pnlNarrowSz.Height = STRIPE_HEIGHT;

	Size pnlWideSz;
	pnlWideSz.Width = caSz.Width;
	pnlWideSz.Height = STRIPE_HEIGHT*2;

	Position pnlPos;

	PanelFactory* pf = _Factories->GetPanelFactory();

	//always have a bot ltGray stripe
	pnlPos.Left=0;
	pnlPos.Top = caPos.Top + 6*STRIPE_HEIGHT;
	
	fres = pf->GetPanel(pnlNarrowSz, pnlPos, 0, CL_MENU_BACKGROUND, &stripes[6]);
	ENSURESUCCESS(fres);

	ubyte_t stripesCount=0;
	switch (stripeStyle)
	{
	case frmStripeNone:
		stripesCount =0;
		break;
	case frmStripeOneBot:
		stripesCount =1;
		break;
	case frmStripeMessageYNDialog:
		stripesCount =4;
		break;
	case frmStripeThreeWide:
		stripesCount =4;
		break;
	case frmStripeSevenNarrow:
		stripesCount =7;
		break;
	default:
		*o_pnl = NULL;
		return GENERAL_ERROR;
	}

	if (stripeStyle != frmStripeOneBot)
	{
		if (stripeStyle == frmStripeThreeWide )
		{
			pnlPos.Left=0;
			pnlPos.Top = caPos.Top + 0*STRIPE_HEIGHT*2;

			fres = pf->GetPanel(pnlWideSz, pnlPos, 0, CL_MENU_EVEN_BACKGROUND, &stripes[0]);
			ENSURESUCCESS(fres);

			pnlPos.Top = caPos.Top + 1*STRIPE_HEIGHT*2;

			fres = pf->GetPanel(pnlWideSz, pnlPos, 0, CL_MENU_BACKGROUND, &stripes[1]);
			ENSURESUCCESS(fres);

			pnlPos.Top = caPos.Top + 2*STRIPE_HEIGHT*2;

			fres = pf->GetPanel(pnlWideSz, pnlPos, 0, CL_MENU_EVEN_BACKGROUND, &stripes[2]);
			ENSURESUCCESS(fres);
		}
		else
		{
			//frmStripeMessageYNDialog or frmStripeSevenNarrow
			
			//Both have one narrow ltGray top
			pnlPos.Left=0;
			pnlPos.Top = caPos.Top + 0*STRIPE_HEIGHT;

			fres = pf->GetPanel(pnlNarrowSz, pnlPos, 0, CL_MENU_BACKGROUND, &stripes[0]);
			ENSURESUCCESS(fres);

			if (stripeStyle == frmStripeSevenNarrow)
			{
				
				for (ubyte_t i=0; i< 5;i++)
				{
					//even?
					ColorHandle clr = CL_MENU_BACKGROUND;
					if (i%2 == 0)
					{
						clr = CL_MENU_EVEN_BACKGROUND;
					}

					pnlPos.Top = caPos.Top + (i+1)*STRIPE_HEIGHT;

					fres = pf->GetPanel(pnlNarrowSz, pnlPos, 0, clr, &stripes[i+1]);
					ENSURESUCCESS(fres);
				}
			}
			else
			{
				//frmStripeMessageYNDialog
				pnlPos.Top += STRIPE_HEIGHT;
				Size pnlExtraLargeSz;
				pnlExtraLargeSz.Width = pnlNarrowSz.Width;
				pnlExtraLargeSz.Height = pnlNarrowSz.Height*4;

				fres = pf->GetPanel(pnlExtraLargeSz, pnlPos, 0, CL_MENU_EVEN_BACKGROUND, &stripes[1]);

				//stripe for OK
				pnlPos.Top += 4*STRIPE_HEIGHT;
				
				fres = pf->GetPanel(pnlExtraLargeSz, pnlPos, 0, CL_MENU_BACKGROUND, &stripes[2]);
				
				ENSURESUCCESS(fres);
			}
		}
	}
	
	//Create container panel

	fres = pf->GetPanel(caSz, caPos, stripesCount, CL_TRANSPARENT, &pnl);
	ENSURESUCCESS(fres);

	ubyte_t addedPanels =0;
	for (ubyte_t i=0;i<7;i++)
	{
		
		if (stripes[i]!=NULL)
		{
			fres = pnl->SetControl(stripes[i], addedPanels);
			ENSURESUCCESS(fres);
			addedPanels++;
		}
	}

	*o_pnl = pnl;
	return SUCCESS;
}

fresult OpenSpaceFormBase::OnBeforeShown(IForm* prevFrom, bool_t reActivation, FormShowResults formShowResult)
{
	fresult fres;
	
	fres = FormBase::OnBeforeShown(prevFrom, reActivation, formShowResult);
	ENSURESUCCESS(fres);
	
	fres = _App->AppStatusBar->DisplayMode(_StatusBarDisplayMode);

	if (_StatusBarDisplayMode==sbdmTitles)
	{
		fres = _App->AppStatusBar->SetTitle(_TitleText, _SubtitleText);
		ENSURESUCCESS(fres);
	}
	
	fres = _App->AppStatusBar->Draw();
	ENSURESUCCESS(fres);

	return SUCCESS;
}