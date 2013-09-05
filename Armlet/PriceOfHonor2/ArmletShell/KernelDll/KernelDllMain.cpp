#define TEST_ARMLET_SYSCALLS
#ifdef TEST_ARMLET_SYSCALLS
#include "ArmletApi.h"
#include "KernelApi.h"
#include "ServerProtocol.h"
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

static int lh = 23;
static int lm = 59;
static unsigned short NoncheId = 2000;

bool __CALLBACK UpdateKernelProc(int elapsed)
{
	int gate_id, signal_level;
	ArmletApi::GetRadioStatus(&gate_id,&signal_level);
	KernelApi::UpdateKernel(
		ArmletApi::GetArmletId(),
		ArmletApi::GetLustraId(),
		ArmletApi::GetBatteryLevel(),
		gate_id,
		signal_level,
		lh,
		lm,
		ArmletApi::GetUpTime());
	return true;
}

bool __CALLBACK UpdateTimeProc(int elapsed)
{
	lm++; 
	if (lm==60) {
		lm=0;lh++;
		if (lh==24)
			lh=0;
	}
	return true;
}

extern "C" __declspec(dllexport) 
int __stdcall DllMain(void* hinstDLL, unsigned long fdwReason, void* lpvReserved)
{
	UNREFERENCED_PARAMETER(hinstDLL);
	UNREFERENCED_PARAMETER(lpvReserved);

	switch (fdwReason)
	{
	case 1://DLL_PROCESS_ATTACH
		TestArmletSyscalls();
		ArmletApi::RequestTimer(UpdateKernelProc,1000);
		ArmletApi::RequestTimer(UpdateTimeProc,60*1000);
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