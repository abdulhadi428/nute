#include "DownInterface.h"

namespace LowLevel {

	VOID_ROUTINE* Vibro = 0;
	VOID_ROUTINE* DecreasePillCharges = 0;
	INT_STR_ROUTINE* InvokeLog = 0;
	INT_STR_ROUTINE* SetCureName = 0;

	void Register__CALLBACKs(
		VOID_ROUTINE* pVibro, 
		VOID_ROUTINE* pDecreasePillCharges,
		INT_STR_ROUTINE* pInvokeLog,
		INT_STR_ROUTINE* pSetCureName)
	{
		Vibro = pVibro;
		DecreasePillCharges = pDecreasePillCharges;
		InvokeLog = pInvokeLog;
		SetCureName = pSetCureName;
	}

} //namespace