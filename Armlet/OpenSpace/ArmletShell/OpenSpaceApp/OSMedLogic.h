#pragma once

struct Wound
{
	char* WoundText;
	bool_t Active;

};

class OSMedLogic
{
	Wound _wounds[MedWoundTargetLast];

public:

	fresult Init();

	char* GetWoundTargetText(MedWoundTarget target);
	fresult AddNewWound(MedWoundTarget target);

	fresult CreateMedStatus(TextField* tf);

	void InitBody();
};