#include "OpenSpace.h"

char* OSMedLogic::GetWoundTargetText( MedWoundTarget target )
{
	char* result;
	switch (target)
	{
		case mwtLimbTarget:
			result = "В руку (ногу) попала пуля! Это точно случилось?";
		break;
		case mwtUpTorso:
			result = "В грудь попала пуля! Это точно случилось?";
			break;
		case mwtDownTorso:
			result = "В живот попала пуля! Это точно случилось?";
			break;
		case mwtHeadTarget:
			result = "В голову попала пуля! Это точно случилось?";
			break;
		case mwtBeatenTarget:
			result = "Последствия рукопашной драки блабла...";
			break;
		case mwtStunnedTarget:
			result = "Вы оглушены! Это точно случилось?";
			break;
		case mwtDyingTarget:
			result = "Вы присмерти! Это точно случилось?";
			break;
		default:
			result = "какайта херь, хз чо";
	}

	return result;
}

fresult OSMedLogic::AddNewWound( MedWoundTarget target )
{
	_wounds[target].Active = TRUE;
	return SUCCESS;
}

fresult OSMedLogic::Init()
{
	
	_wounds[mwtDyingTarget].WoundText = "ПРИ СМЕРТИ";
	_wounds[mwtStunnedTarget].WoundText = "ОГЛУШЕН"; 
	_wounds[mwtBeatenTarget].WoundText = "Избит";
	_wounds[mwtLimbTarget].WoundText = "Повреждена конечность"; 
	_wounds[mwtUpTorso].WoundText = "Ранение корпуса(верх)";
	_wounds[mwtDownTorso].WoundText = "Ранение корпуса(низ)";
	_wounds[mwtHeadTarget].WoundText = "Травма головы";
	
	InitBody();

	return SUCCESS;
}

void OSMedLogic::InitBody()
{
	for (ubyte_t i=0; i< MedWoundTargetLast; i++)
	{
		_wounds[i].Active = FALSE;
	}
}

fresult OSMedLogic::CreateMedStatus( TextField* tf )
{
	fresult fres;

	fres = tf->Clear();
	ENSURESUCCESS(fres);

	for (ubyte_t i=0; i< MedWoundTargetLast; i++)
	{

		if (_wounds[i].Active == TRUE)
		{
			fres = tf->AppendText(_wounds[i].WoundText);
			ENSURESUCCESS(fres);
			fres = tf->AppendText("\n");
			ENSURESUCCESS(fres);

		}
	}

	return SUCCESS;
}
