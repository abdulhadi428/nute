#pragma once

//Patterns
typedef enum _BEHAVIOUR_PATTERNS {
	bhpWoundHead,
	bhpWoundTorso,
	bhpEgoCritical,
	bhpFuckingIllness,

	bhpLast
} BehaviorPatterns;

struct BehaviourPatternDescription
{
	bool_t IsActive;
	char* Text;
	char* Title;
	bool IsNew;
	bool IsCritical;
	char* CritText;
};