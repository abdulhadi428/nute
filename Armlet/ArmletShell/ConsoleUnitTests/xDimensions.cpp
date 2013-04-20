#include "xDimensions.h"
#include <iostream>
#include "Dimensions.h"


void xDimensions::doTests()
{
	char *testName;
	try
	{
		testName = "xGetRectClipping";
		xGetRectClipping();

		std::cout << "xDimensions tests passed.\n";
	}
	catch (char* err)
	{
		std::cout <<"xDimensions FAILED: "<< testName <<": "<< err;
	}
}

void xDimensions::xGetRectClipping()
{
	Position pos1;
	Size sz1;
	Position pos2;
	Size sz2;

	Position intPos;
	Size intSz;

	Position testPos;
	Size testSz;

	bool_t res;

	///////////////////////////////////////
	//equal
	pos1.Top = 20;
	pos1.Left = 10;
	sz1.Height = 15;
	sz1.Width = 25;

	pos2.Top = 20;
	pos2.Left = 10;
	sz2.Height = 15;
	sz2.Width = 25;

	testPos.Top = 20;
	testPos.Left = 10;
	testSz.Height = 15;
	testSz.Width = 25;

	intPos.data = 0;
	intSz.data = 0;

	res = GetRectClipping(pos1, sz1, pos2, sz2, &intPos, &intSz);
	if (res != TRUE)
		throw "Can't calc intersection of equal rects. Returned no intersection";
	if (intPos.data != testPos.data)
		throw "Can't calc intersection of equal rects. Invalid intersect position";
	if (intSz.data != testSz.data)
		throw "Can't calc intersection of equal rects. Invalid intersect size";

	//swap 
	intPos.data = 0;
	intSz.data = 0;
	res = GetRectClipping(pos2, sz2, pos1, sz1, &intPos, &intSz);
	if (res != TRUE)
		throw "Can't calc intersection of equal rects (swapped). Returned no intersection";
	if (intPos.data != testPos.data)
		throw "Can't calc intersection of equal rects (swapped). Invalid intersect position";
	if (intSz.data != testSz.data)
		throw "Can't calc intersection of equal rects (swapped). Invalid intersect size";

	////////////////////////////////////////////////////////////////
	//same x,y start
	pos1.Top = 20;
	pos1.Left = 10;
	sz1.Height = 13;
	sz1.Width = 23;

	pos2.Top = 20;
	pos2.Left = 10;
	sz2.Height = 15;
	sz2.Width = 25;

	testPos.Top = 20;
	testPos.Left = 10;
	testSz.Height = 13;
	testSz.Width = 23;

	intPos.data = 0;
	intSz.data = 0;

	res = GetRectClipping(pos1, sz1, pos2, sz2, &intPos, &intSz);
	if (res != TRUE)
		throw "Can't calc intersection of same origin rects. Returned no intersection";
	if (intPos.data != testPos.data)
		throw "Can't calc intersection of equal origin rects. Invalid intersect position";
	if (intSz.data != testSz.data)
		throw "Can't calc intersection of equal origin rects. Invalid intersect size";

	//swap 
	intPos.data = 0;
	intSz.data = 0;
	res = GetRectClipping(pos2, sz2, pos1, sz1, &intPos, &intSz);
	if (res != TRUE)
		throw "Can't calc intersection of equal origin rects (swapped). Returned no intersection";
	if (intPos.data != testPos.data)
		throw "Can't calc intersection of equal origin rects (swapped). Invalid intersect position";
	if (intSz.data != testSz.data)
		throw "Can't calc intersection of equal origin rects (swapped). Invalid intersect size";

	////////////////////////////////////////////////////////////////
	//same x,y end
	pos1.Top = 5;
	pos1.Left = 5;
	sz1.Height = 30;
	sz1.Width = 30;

	pos2.Top = 20;
	pos2.Left = 10;
	sz2.Height = 15;
	sz2.Width = 25;

	testPos.Top = 20;
	testPos.Left = 10;
	testSz.Height = 15;
	testSz.Width = 25;

	intPos.data = 0;
	intSz.data = 0;

	res = GetRectClipping(pos1, sz1, pos2, sz2, &intPos, &intSz);
	if (res != TRUE)
		throw "Can't calc intersection of same right, bot rects. Returned no intersection";
	if (intPos.data != testPos.data)
		throw "Can't calc intersection of equal right, bot rects. Invalid intersect position";
	if (intSz.data != testSz.data)
		throw "Can't calc intersection of equal right, bot rects. Invalid intersect size";

	//swap 
	intPos.data = 0;
	intSz.data = 0;
	res = GetRectClipping(pos2, sz2, pos1, sz1, &intPos, &intSz);
	if (res != TRUE)
		throw "Can't calc intersection of equal right, bot rects (swapped). Returned no intersection";
	if (intPos.data != testPos.data)
		throw "Can't calc intersection of equal right, bot rects (swapped). Invalid intersect position";
	if (intSz.data != testSz.data)
		throw "Can't calc intersection of equal right, bot rects (swapped). Invalid intersect size";


	////////////////////////////////////////////////////////////////
	//inside
	pos1.Top = 5;
	pos1.Left = 5;
	sz1.Height = 30;
	sz1.Width = 30;

	pos2.Top = 20;
	pos2.Left = 10;
	sz2.Height = 10;
	sz2.Width = 5;

	testPos.Top = 20;
	testPos.Left = 10;
	testSz.Height = 10;
	testSz.Width = 5;

	intPos.data = 0;
	intSz.data = 0;

	res = GetRectClipping(pos1, sz1, pos2, sz2, &intPos, &intSz);
	if (res != TRUE)
		throw "Can't calc intersection of inside rects. Returned no intersection";
	if (intPos.data != testPos.data)
		throw "Can't calc intersection of inside. Invalid intersect position";
	if (intSz.data != testSz.data)
		throw "Can't calc intersection of inside. Invalid intersect size";

	//swap 
	intPos.data = 0;
	intSz.data = 0;
	res = GetRectClipping(pos2, sz2, pos1, sz1, &intPos, &intSz);
	if (res != TRUE)
		throw "Can't calc intersection of inside rects (swapped). Returned no intersection";
	if (intPos.data != testPos.data)
		throw "Can't calc intersection of inside rects (swapped). Invalid intersect position";
	if (intSz.data != testSz.data)
		throw "Can't calc intersection of inside rects (swapped). Invalid intersect size";

	////////////////////////////////////////////////////////////////
	//no intersect all
	pos1.Top = 5;
	pos1.Left = 5;
	sz1.Height = 10;
	sz1.Width = 10;

	pos2.Top = 16;
	pos2.Left = 16;
	sz2.Height = 10;
	sz2.Width = 10;

	intPos.data = 0;
	intSz.data = 0;

	res = GetRectClipping(pos1, sz1, pos2, sz2, &intPos, &intSz);
	if (res == TRUE)
		throw "Can't calc intersection of all no intersect rects. Returned an intersection";

	//swap 
	intPos.data = 0;
	intSz.data = 0;
	res = GetRectClipping(pos2, sz2, pos1, sz1, &intPos, &intSz);
	if (res == TRUE)
		throw "Can't calc intersection of all no intersect rects (swapped). Returned an intersection";

	////////////////////////////////////////////////////////////////
	//no intersect x
	pos1.Top = 5;
	pos1.Left = 5;
	sz1.Height = 10;
	sz1.Width = 10;

	pos2.Top = 6;
	pos2.Left = 20;
	sz2.Height = 1;
	sz2.Width = 10;

	intPos.data = 0;
	intSz.data = 0;

	res = GetRectClipping(pos1, sz1, pos2, sz2, &intPos, &intSz);
	if (res == TRUE)
		throw "Can't calc intersection of all no intersect x rects. Returned an intersection";

	//swap 
	intPos.data = 0;
	intSz.data = 0;
	res = GetRectClipping(pos2, sz2, pos1, sz1, &intPos, &intSz);
	if (res == TRUE)
		throw "Can't calc intersection of all no intersect rects x (swapped). Returned an intersection";

	////////////////////////////////////////////////////////////////
	//no intersect y
	pos1.Top = 5;
	pos1.Left = 5;
	sz1.Height = 10;
	sz1.Width = 10;

	pos2.Top = 16;
	pos2.Left = 6;
	sz2.Height = 5;
	sz2.Width = 1;

	intPos.data = 0;
	intSz.data = 0;

	res = GetRectClipping(pos1, sz1, pos2, sz2, &intPos, &intSz);
	if (res == TRUE)
		throw "Can't calc intersection of all no intersect rects y. Returned an intersection";

	//swap 
	intPos.data = 0;
	intSz.data = 0;
	res = GetRectClipping(pos2, sz2, pos1, sz1, &intPos, &intSz);
	if (res == TRUE)
		throw "Can't calc intersection of all no intersect rects y(swapped). Returned an intersection";
}









