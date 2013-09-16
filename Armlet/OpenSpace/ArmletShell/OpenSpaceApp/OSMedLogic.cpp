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
	return SUCCESS;
}

fresult OSMedLogic::Init()
{
	return SUCCESS;
}
