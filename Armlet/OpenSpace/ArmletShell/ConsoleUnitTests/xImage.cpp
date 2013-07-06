#include "xImage.h"
#include "Image.h"
#include <iostream>
using namespace ThreeKShell;

void xImage::doTests()
{
	char *testName;
	try
	{
		testName = "xBasicTest";
		xBasicTest();

		std::cout << "xImage tests passed.\n";
	}
	catch (char* err)
	{
		std::cout <<"xImage FAILED: "<< testName <<": "<< err << "\n";
	}
}

void xImage::xBasicTest()
{
	Image img;
}