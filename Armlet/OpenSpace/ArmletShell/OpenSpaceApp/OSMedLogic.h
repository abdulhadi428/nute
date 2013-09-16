#pragma once


class OSMedLogic
{
public:

	fresult Init();

	char* GetWoundTargetText(MedWoundTarget target);
	fresult AddNewWound(MedWoundTarget target);
};