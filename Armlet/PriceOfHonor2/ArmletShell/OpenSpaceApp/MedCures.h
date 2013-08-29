#pragma once
#ifndef _MED_
	#error _MED_
#endif

typedef struct _CURE_DESC {					
	CURE_ID id;						//cure id
	const char* Name;				//cure name
//	const char* Purpose;			//cure purpose description
	const char* Effect;				//cure effect description
//	const char* Contraindications;	//cure contraindications description
//	const char* SideEffects;		//cure side effects description
} CURE_DESC;

extern const char* CureNames[];
extern CURE_DESC CureDescs[MaxCureId];

void InitCureDescs();
