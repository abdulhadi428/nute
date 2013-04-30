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
		LowLog("ARMLET: %s", log);
	}

} //namespace
