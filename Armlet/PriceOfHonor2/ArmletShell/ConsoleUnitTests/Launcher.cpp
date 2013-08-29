
//#include "TextForm.h"
#include "xTestStringHelpers.h"
#include <tchar.h>
#include <stdio.h>
#include <Windows.h>
#include <conio.h>

#include "defs.h"
#include "xTestTextField.h"
#include "xDimensions.h"
#include "xScatteredMenu.h"
#include "xImage.h"

int __cdecl _tmain(int argc, _TCHAR* argv[])
{
/*
	//TextField fields[2];
	Size size;
	Position position;

	size.Height = 3;
	size.Width = 4;

	position


	fields[0].Init(&size, &position)

	*/

	char* a = "deeper in the valley the river goes!";
	//char* b =  

	xTestStringHelpers tests;
	tests.doTests();

	xTestTextField TextFieldTests;
	TextFieldTests.Init();
	TextFieldTests.doTests();

	xDimensions DimensionsTests;
	DimensionsTests.doTests();

	xScatteredMenu MenuTests;
	MenuTests.Init();
	MenuTests.doTests();

	xImage ImageTests;
	ImageTests.doTests();
  	_getch();

	PMY_CALLBACK pf = &CallbackImplementation;

	
}

char CallbackImplementation(char* str)
{
	return str[0];
}

