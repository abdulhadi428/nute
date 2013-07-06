// Visualizer.cpp : main project file.

#include "stdafx.h"
#include "MainForm.h"

using namespace Visualizer;

[STAThreadAttribute]
int __clrcall main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	Application::Run(gcnew MainForm());
	return 0;
}

/*
Due to initialization of global object in libraries
entry pint is:
int __clrcall mainCRTStartupStrArray(cli::array<class System::String ^ >^)
?mainCRTStartupStrArray@@$$FYMHP$01AP$AAVString@System@@@Z
*/