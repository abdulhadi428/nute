#define TEST_ARMLET_SYSCALLS
#ifdef TEST_ARMLET_SYSCALLS
#include "ArmletApi.h"
#include "Windows.h"

void TestGetRandom()
{
	int a,a0=0,a1=0,a2=0;
	for (int i=0;i<10000;i++) {
		a = ArmletApi::GetRandom(2);
		if (a==0) {a0++;continue;}
		if (a==1) {a1++;continue;}
		if (a==2) {a2++;continue;}
	}
	a=0;
}

void TestGetUpTime()
{
	int a,a0=0,a1=0,a2=0;
	a0=ArmletApi::GetUpTime();
	Sleep(1000);
	a1=ArmletApi::GetUpTime();
	Sleep(2000);
	a2=ArmletApi::GetUpTime();
	a=0;
}

void TestFiles()
{
	int res;
	char filename[40];
	ArmletApi::snprintf(filename,40,"%s","test.tmp");
	ArmletApi::FILE TestFile;
	res = ArmletApi::OpenFile(&TestFile,filename,true);
	res = ArmletApi::WriteFile(&TestFile,filename,sizeof(filename));
	res = ArmletApi::AppendFile(&TestFile,filename,sizeof(filename));
	res = ArmletApi::WriteFile(&TestFile,filename,sizeof(filename));
	memset(filename,0,sizeof(filename));
	res = ArmletApi::ReadFile(&TestFile,filename,sizeof(filename));
}

void TestArmletSyscalls() {
	TestGetRandom();
	TestGetUpTime();
	TestFiles();
}

#else
	void TestArmletSyscalls() {}
#endif

extern "C" __declspec(dllexport) 
int __stdcall DllMain(void* hinstDLL, unsigned long fdwReason, void* lpvReserved)
{
	UNREFERENCED_PARAMETER(hinstDLL);
	UNREFERENCED_PARAMETER(lpvReserved);

	switch (fdwReason)
	{
	case 1://DLL_PROCESS_ATTACH
		TestArmletSyscalls();
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