#include "DownInterface.h"
#include "ArmletApi.h"
#include "Helper.h"

ArmletApi::FILE LogFile;

namespace LowLevel {

	void LowLevelLibMain(void)
	{		
		if (!ArmletApi::OpenFile(&::LogFile, "log.txt", true)) {
			ArmletApi::CriticalError("OpenLog failed");
		}
		if (!ArmletApi::InitializeShell()) {
			ArmletApi::CriticalError("InitializeShell failed");
		}
	}

} //namespace

namespace ArmletApi {

	void Log(char* log)
	{		
		WriteFile(&LogFile, log, strlen(log));
#ifdef _MBCS
		LowLog("ARMLET: %s", log);
#endif
	}

	//SPECIAL PLATFORM DEPENDENT
#ifdef _MBCS
	//visualizer only syscall
	void SetCureName(int cure_id, char* name)
	{
		LowLevel::SetCureName(cure_id, name);
	}
#endif

}
