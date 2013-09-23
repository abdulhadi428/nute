#pragma once

typedef enum _STRESS_TYPES {
	StressHelplessness,
	StressEgo,
	StressCruelty,
	StressNone
} StressType;

class StressChangeHandler;

class OpenSpaceLogic
{

private:
	ubyte_t _stressHelplessness;
	ubyte_t _stressEgo;
	ubyte_t _stressCruelty;

	OSMedLogic _medLogicInstance;
	OSBehaviourLogic _behLogicInstance;
public:

#pragma region //		Stress
	bool_t SetStressHelplessness(ubyte_t level);
	ubyte_t GetStressHelplessness();

	bool_t SetStressEgo(ubyte_t level);
	ubyte_t GetStressEgo();

	bool_t SetStressCruelty(ubyte_t level);
	ubyte_t GetStressCruelty();

	ColorHandle GetStressColor(StressType stress, ubyte_t level);

	char* getStressMP3FileName(StressType stress);

#pragma endregion

	OSMedLogic* MedLogic;
	OSBehaviourLogic* BehaviorLogic;

	fresult Init();
};

