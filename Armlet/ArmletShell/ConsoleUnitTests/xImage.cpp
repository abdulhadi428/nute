#include "xImage.h"
#include "Image.h"
#include <iostream>

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