#include "Common.h"

extern "C" __declspec(dllexport) 
int __stdcall DllMain(void* hinstDLL, unsigned long fdwReason, void* lpvReserved)
{
	UNREFERENCED_PARAMETER(hinstDLL);
	UNREFERENCED_PARAMETER(lpvReserved);

	switch (fdwReason)
	{
	case 1://DLL_PROCESS_ATTACH
        break;
	case 0://DLL_PROCESS_DETACH:
        break;
	case 2://DLL_THREAD_ATTACH:
        break;
	case 3://DLL_THREAD_DETACH:
        break;
	}
	return true;
}