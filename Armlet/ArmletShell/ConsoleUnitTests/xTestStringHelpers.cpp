
#include "xTestStringHelpers.h"
#include "TextHelper.h"
#include <iostream>
#include "PlatformAPI.h"
#include "ArmletShell.h"
using namespace ArmletShell;

void xTestStringHelpers::doTests()
{
	char *testName;
	try
	{
		testName = "xInStr";
		xInStr();
		testName = "xLength";
		xLength();
		testName = "xStringAdd";
		xStringAdd();
		testName = "xAllocStringBuffer";
		xAllocStringBuffer();
		testName = "xFillStringBuffer";
		xFillStringBuffer();
		testName = "xSetTextInBuff";
		xSetTextInBuff();
		testName = "xAllocAndConvertBuffAsPrintable";
		xAllocAndConvertBuffAsPrintable();
		testName = "xAllocSquareBuffFromString";
		xAllocSquareBuffFromString();
		testName = "xStrCopy";
		xStrCopy();
		testName = "xStrPad";
		xStrPad();
		
		std::cout << "xTestStringHelpers tests passed.\n";
	}
	catch (char* err)
	{
		std::cout <<"xTestStringHelpers FAILED: " << testName <<": "<< err;
	}
}

void xTestStringHelpers::xInStr()
{

	/////////////////////////////////
	char* string = "deeper in the valley shallow river goes!";
	char* pattern = "deeper";
	int result = InStr(string, pattern, 0);
	if (result != 0)
		throw "Cant find the pattern in the start";

	/////////////////////////////////
	pattern = "valley";
	result = InStr(string, pattern, 0);
	if (result != 14)
		throw "Cant find the pattern in the mid";

	/////////////////////////////////
	pattern = "valley";
	result = InStr(string, pattern, 0);
	if (result != 14)
		throw "Cant find the pattern after start index";
	/////////////////////////////////

	pattern = "valley";
	result = InStr(string, pattern, 10);
	if (result != 14)
		throw "Cant find the pattern after start index";
	/////////////////////////////////

	pattern = "vallew";
	result = InStr(string, pattern, 0);
	if (result != -1)
		throw "Wrongly found patern";
	/////////////////////////////////
	
	pattern = "valley";
	result = InStr(string, pattern, 15);
	if (result != -1)
		throw "Wrongly found after start index";
}

void xTestStringHelpers::xLength()
{
	int result;
	/////////////////////////////////
	result = Length("hate");
	if (result!=4)
		throw "Wrongly determined 'hate' length";
	/////////////////////////////////

	result = Length("");
	if (result!=0)
		throw "Can't handle empty string";
	/////////////////////////////////

	result = Length(NULL);
	if (result!=0)
		throw "Can't handle empty string";

}

void xTestStringHelpers::xStringAdd()
{
	char* str;
	char* add;
	char* result;
	char* test;

	/////////////////////////////////
	str = "river";
	add = "valley";
	test = "rivervalley";
	result = StringAdd(str, add);
	if (!StringEquals(result, test))
		throw "failed to concat strings.";

	/////////////////////////////////
	str = "";
	add = "valley";
	test = "valley";
	result = StringAdd(str, add);
	if (!StringEquals(result, test))
		throw "failed to concat empty string with string.";

	/////////////////////////////////
	str = "river";
	add = "";
	test = "river";
		result = StringAdd(str, add);
	if (!StringEquals(result, test))
		throw "failed to concat string with empty string.";

}

void xTestStringHelpers::xAllocStringBuffer()
{
	Size size;
	char* result;

	/////////////////////////////////
	size.Width =0;
	size.Height = 10;
	result = AllocStringBuffer(size, false);
	if (result != NULL)
		throw "Can't handle 0-width size";

	/////////////////////////////////
	size.Width =10;
	size.Height = 0;
	result = AllocStringBuffer(size, false);
	if (result != NULL)
		throw "Can't handle 0-height size";

	/////////////////////////////////
	size.Width =10;
	size.Height = 10;
	result = AllocStringBuffer(size, false);
	if (result == NULL)
		throw "Can't handle normal size";
	delete result;

	/////////////////////////////////
	size.Width =2;
	size.Height = 2;
	result = AllocStringBuffer(size, true);
	if (result == NULL)
		throw "Can't handle normal size";
	if (result[size.Width*size.Height]!=NULL)
		throw "Failed to zero-terminated buff";
	delete result;
}

void xTestStringHelpers::xFillStringBuffer()
{
	fresult result = 0;
	 
	Size size;
	
	size.Height =2;
	size.Width =3;
	char *buff = AllocStringBuffer(size, true);
	char* testbuff = "###"
					 "###";
	result = FillStringBuffer(buff, size, '#');
	if (result!=SUCCESS)
		throw "Buff 2x3 problem with fresult";
	if (!StringEquals(buff, testbuff))
		throw "Buff 2x3 isn't filled correctly";
	delete buff;
	/////////////////////////////////

	size.Height =2;
	size.Width =2;
	buff = AllocStringBuffer(size, true);
	testbuff = "##"
		   	   "##";
	result= FillStringBuffer(buff, size, '#');
	if (result!=SUCCESS)
		throw "Buff 2x2 problem with fresult";
	if (!StringEquals(buff, testbuff))
		throw "Buff 2x2 isn't filled correctly";
	delete buff;
	/////////////////////////////////

	size.Height =1;
	size.Width =3;
	buff = AllocStringBuffer(size, true);
	testbuff = "###";
	result = FillStringBuffer(buff, size, '#');
	if (result!=SUCCESS)
		throw "Buff 1x3 problem with fresult";
	if (!StringEquals(buff, testbuff))
		throw "Buff 1x3 isn't filled correctly";
	delete buff;

	/////////////////////////////////

	size.Height =0;
	size.Width =3;
	result = FillStringBuffer(buff, size, '#');
	if (result!=GENERAL_ERROR)
		throw "Failed to handle 0 height";
	/////////////////////////////////

	size.Height =3;
	size.Width =0;
	result = FillStringBuffer(buff, size, '#');
	if (result!=GENERAL_ERROR)
		throw "Failed to handle 0 width";
	/////////////////////////////////

}

void xTestStringHelpers::xSetTextInBuff()
{
	fresult result;
	char* buff;
	Size buffSize;
	Position strPos;
	char* string;
	char* test;

	/////////////////////////////////
	buffSize.Height = 2;
	buffSize.Width = 3;
	strPos.Left = 0;
	strPos.Top = 0;
	string = "qq";
	test = "qq####";

	buff = AllocStringBuffer(buffSize, true);
	if (buff==NULL)
		throw "Can't alloc buffer.";
	result = FillStringBuffer(buff, buffSize, '#');
	if (result != SUCCESS)
		throw "Can't fill buffer with #.";
	result = SetTextInBuff(string, strPos, buff, buffSize);
	if (result != SUCCESS)
		throw "Error setting qq to buff.";
	if (!StringEquals(buff, test))
		throw "Can't set qq to buff.";

	/////////////////////////////////
	buffSize.Height = 2;
	buffSize.Width = 3;
	strPos.Left = 0;
	strPos.Top = 0;
	string = "qqq";
	test = "qqq###";

	buff = AllocStringBuffer(buffSize, true);
	if (buff==NULL)
		throw "Can't alloc buffer.";
	result = FillStringBuffer(buff, buffSize, '#');
	if (result != SUCCESS)
		throw "Can't fill buffer with #.";
	result = SetTextInBuff(string, strPos, buff, buffSize);
	if (result != SUCCESS)
		throw "Error setting qqq to buff.";
	if (!StringEquals(buff, test))
		throw "Can't set qqq to buff.";
	delete buff;

	/////////////////////////////////
	buffSize.Height = 2;
	buffSize.Width = 3;
	strPos.Left = 0;
	strPos.Top = 0;
	string = "qqqq";
	test = "qqq###";

	buff = AllocStringBuffer(buffSize, true);
	if (buff==NULL)
		throw "Can't alloc buffer.";
	result = FillStringBuffer(buff, buffSize, '#');
	if (result != SUCCESS)
		throw "Can't fill buffer with #.";
	result = SetTextInBuff(string, strPos, buff, buffSize);
	if (result != GENERAL_ERROR)
		throw "Can't handle too long string that exceeds buff width.";
	delete buff;

	/////////////////////////////////
	buffSize.Height = 2;
	buffSize.Width = 3;
	strPos.Left = 3;
	strPos.Top = 0;
	string = "qq";
	test = "qqq###";

	buff = AllocStringBuffer(buffSize, true);
	if (buff==NULL)
		throw "Can't alloc buffer.";
	result = FillStringBuffer(buff, buffSize, '#');
	if (result != SUCCESS)
		throw "Can't fill buffer with #.";
	result = SetTextInBuff(string, strPos, buff, buffSize);
	if (result != GENERAL_ERROR)
		throw "Can't handle invalid position which has Left outside buff.";
	delete buff;

	/////////////////////////////////
	buffSize.Height = 2;
	buffSize.Width = 3;
	strPos.Left = 2;
	strPos.Top = 2;
	string = "qq";
	test = "qqq###";

	buff = AllocStringBuffer(buffSize, true);
	if (buff==NULL)
		throw "Can't alloc buffer.";
	result = FillStringBuffer(buff, buffSize, '#');
	if (result != SUCCESS)
		throw "Can't fill buffer with #.";
	result = SetTextInBuff(string, strPos, buff, buffSize);
	if (result != GENERAL_ERROR)
		throw "Can't handle invalid position which has top outside buff.";
	delete buff;

	/////////////////////////////////
	buffSize.Height = 2;
	buffSize.Width = 3;
	strPos.Left = 2;
	strPos.Top = 1;
	string = "qq";
	test = "qqq###";

	buff = AllocStringBuffer(buffSize, true);
	if (buff==NULL)
		throw "Can't alloc buffer.";
	result = FillStringBuffer(buff, buffSize, '#');
	if (result != SUCCESS)
		throw "Can't fill buffer with #.";
	result = SetTextInBuff(string, strPos, buff, buffSize);
	if (result != GENERAL_ERROR)
		throw "Can't handle long string at position to exceed width.";
	delete buff;

	//new lines
	/////////////////////////////////
	buffSize.Height = 2;
	buffSize.Width = 3;
	strPos.Left = 0;
	strPos.Top = 0;
	string = "qq\nqq";
	test = "qq#qq#";

	buff = AllocStringBuffer(buffSize, true);
	if (buff==NULL)
		throw "Can't alloc buffer.";
	result = FillStringBuffer(buff, buffSize, '#');
	if (result != SUCCESS)
		throw "Can't fill buffer with #.";
	result = SetTextInBuff(string, strPos, buff, buffSize);
	if (result != SUCCESS)
		throw "Error setting text with 1 newline.";
	if (!StringEquals(buff, test))
		throw "Invalid result of setting text with 1 new line.";
	delete buff;

	/////////////////////////////////
	buffSize.Height = 3;
	buffSize.Width = 4;
	strPos.Left = 1;
	strPos.Top = 1;
	string = "qq\nqq";
	test = "#####qq#qq##";

	buff = AllocStringBuffer(buffSize, true);
	if (buff==NULL)
		throw "Can't alloc buffer.";
	result = FillStringBuffer(buff, buffSize, '#');
	if (result != SUCCESS)
		throw "Can't fill buffer with #.";
	result = SetTextInBuff(string, strPos, buff, buffSize);
	if (result != SUCCESS)
		throw "Error setting text with 1 newline at position 1,1.";
	if (!StringEquals(buff, test))
		throw "Invalid result of setting text with 1 new line at 1,1.";
	delete buff;	

	/////////////////////////////////
	//position string with newline outside of buff
	buffSize.Height = 3;
	buffSize.Width = 3;
	strPos.Left = 2;
	strPos.Top = 1;
	string = "qq\nqq";
	test = "#####qq#qq##";

	buff = AllocStringBuffer(buffSize, true);
	if (buff==NULL)
		throw "Can't alloc buffer.";
	result = FillStringBuffer(buff, buffSize, '#');
	if (result != SUCCESS)
		throw "Can't fill buffer with #.";
	result = SetTextInBuff(string, strPos, buff, buffSize);
	if (result == SUCCESS)
		throw "Cant handle length+position outside width";
	delete buff;

	/////////////////////////////////
	//height outside buff
	buffSize.Height = 2;
	buffSize.Width = 3;
	strPos.Left = 1;
	strPos.Top = 1;
	string = "qq\nqq";
	test = "#####qq#qq##";

	buff = AllocStringBuffer(buffSize, true);
	if (buff==NULL)
		throw "Can't alloc buffer.";
	result = FillStringBuffer(buff, buffSize, '#');
	if (result != SUCCESS)
		throw "Can't fill buffer with #.";
	result = SetTextInBuff(string, strPos, buff, buffSize);
	if (result == SUCCESS)
		throw "Cant handle lines+position outside height";
	delete buff;
}

void xTestStringHelpers::xAllocAndConvertBuffAsPrintable()
{
	char* result;
	Size size;
	char* buff=NULL;
	char* testbuff;
	fresult fresult;

	/////////////////////////////////
	size.Height =0;
	size.Width =10;
	result = AllocAndConvertBuffAsPrintable(buff, size, '%');
	if (result != NULL)
		throw "Can't handle 0-height size";

	/////////////////////////////////
	size.Height =10;
	size.Width =0;
	result = AllocAndConvertBuffAsPrintable(buff, size, '%');
	if (result != NULL)
		throw "Can't handle 0-width size";

	/////////////////////////////////
	size.Height =2;
	size.Width =2;
	result = AllocAndConvertBuffAsPrintable(buff, size, '%');
	if (result != NULL)
		throw "Can't handle NULL buff";

	/////////////////////////////////
	size.Height =1;
	size.Width =2;
	testbuff = "%%";

	buff = AllocStringBuffer(size, true);
	if (buff == NULL)
		throw "Can't handle 1x2 buff. Can't alloc";
	result = AllocAndConvertBuffAsPrintable(buff, size, '%');
	if (result == NULL)
		throw "Can't handle 1x2 buff. Can't get printable buff";
	if (!StringEquals(result, testbuff))
		throw "Can't handle 1x2 buff. Wrong result";
	delete buff;
	delete result;

	/////////////////////////////////
	size.Height =1;
	size.Width =2;
	testbuff = "##";
			   
	buff = AllocStringBuffer(size, true);
	if (buff == NULL)
		throw "Can't handle 1x2 filled buff. Can't alloc";
	fresult = FillStringBuffer(buff, size, '#');
	if (fresult != SUCCESS)
		throw "Can't handle 1x2 filled buff. Can't fill with #";
	result = AllocAndConvertBuffAsPrintable(buff, size, '%');
	if (result == NULL)
		throw "Can't handle 1x2 filled buff. Can't get printable (filled with # buff)";
	if (!StringEquals(result, testbuff))
		throw "Can't handle 1x2 filled buff. Wrong result";
	delete buff;
	delete result;

	/////////////////////////////////
	size.Height =3;
	size.Width =2;
	testbuff = "%%\n%%\n%%";

	buff = AllocStringBuffer(size, true);
	if (buff == NULL)
		throw "Can't handle 3x2 buff. Can't alloc";
	result = AllocAndConvertBuffAsPrintable(buff, size, '%');
	if (result == NULL)
		throw "Can't handle 3x2 buff. Can't get printable";
	if (!StringEquals(result, testbuff))
		throw "Can't get 3x2 printable. Wrong result";
	delete buff;
	delete result;

	/////////////////////////////////
	size.Height =3;
	size.Width =3;
	testbuff = "%%%\n%qq\n%%%";
	Position pos;
	pos.Left =1;
	pos.Top =1;
	char* string = "qq";

	buff = AllocStringBuffer(size, true);
	if (buff == NULL)
		throw "Can't handle 3x2 buff. Can't alloc";
	fresult = SetTextInBuff(string, pos, buff, size);
	if (fresult!=SUCCESS)
		throw "Error setting text to 3x3 buff.";
	result = AllocAndConvertBuffAsPrintable(buff, size, '%');
	if (result == NULL)
		throw "Error getting 3x3 printable buff.";
	if (!StringEquals(result, testbuff))
		throw "Can't get 3x3 printable buff with text at  1,1 . Wrong result";
	delete buff;
	delete result;
}

void xTestStringHelpers::xAllocSquareBuffFromString()
{
	fresult result;
	Size size;
	char* string=NULL;
	char* buff=NULL;
	char* test;
	char* printableView;

	/////////////////////////////////
	result = AllocSquareBuffFromString(NULL, &buff, &size, '#');
	if (result!=GENERAL_ERROR)
		throw "Failed to handle NULL string";

	/////////////////////////////////
	string="deeper";
	result = AllocSquareBuffFromString(string, &buff, NULL, '#');
	if (result!=GENERAL_ERROR)
		throw "Failed to handle NULL size";
	
	/////////////////////////////////
	size.Width =0;
	size.Height =0;
	buff = NULL;
	string="deeper";
	test = "deeper";
	result = AllocSquareBuffFromString(string, &buff, &size, NULL);
	if (result!=SUCCESS)
		throw "Error getting 1-line string";
	if (size.Height !=1)
		throw "Failed to calc height for 1-line string";
	if (size.Width != 6)
		throw "Failed to calc width for 1-line string";
	printableView = AllocAndConvertBuffAsPrintable(buff, size, '%');
	if(printableView == NULL)
		throw "Failed to create buff from 1-line string. Error getting printable.";
	if (!StringEquals(printableView, test))
		throw "Failed to create buff from 1-line string. Incorrect result ";
	delete buff;
	delete printableView;

	/////////////////////////////////
	size.Width =0;
	size.Height =0;
	buff = NULL;
	string="deeper\ndeeper";
	test = "deeper\ndeeper";
	result = AllocSquareBuffFromString(string, &buff, &size, '#');
	if (result!=SUCCESS)
		throw "Error getting 2-line string";
	if (size.Height !=2)
		throw "Failed to calc height for 2-line rect string";
	if (size.Width != 6)
		throw "Failed to calc width for 2-line rect string";
	printableView = AllocAndConvertBuffAsPrintable(buff, size, '%');
	if(printableView == NULL)
		throw "Failed to create buff from 2-line rect string. Error getting printable.";
	if (!StringEquals(printableView, test))
		throw "Failed to create buff from 2-line rect string. Incorrect result ";
	delete buff;
	delete printableView;

	/////////////////////////////////
	size.Width =0;
	size.Height =0;
	buff = NULL;
	string="deep\ndeeper";
	test = "deep%%\ndeeper";
	result = AllocSquareBuffFromString(string, &buff, &size, '\0');
	if (result!=SUCCESS)
		throw "Error getting 2-line diff-width strings";
	if (size.Height !=2)
		throw "Failed to calc height for 2-line diff-width strings";
	if (size.Width != 6)
		throw "Failed to calc width for 2-line diff-width strings";
	printableView = AllocAndConvertBuffAsPrintable(buff, size, '%');
	if(printableView == NULL)
		throw "Failed to create buff from 2-line diff-width strings. Error getting printable.";
	if (!StringEquals(printableView, test))
		throw "Failed to create buff from 2-line diff-width strings. Incorrect result ";
	delete buff;
	delete printableView;

	/////////////////////////////////
	size.Width =0;
	size.Height =0;
	buff = NULL;
	string="deeper\ndeep\ndee";
	test = "deeper\ndeep%%\ndee%%%";
	result = AllocSquareBuffFromString(string, &buff, &size, '\0');
	if (result!=SUCCESS)
		throw "Error getting 3-line diff-width strings";
	if (size.Height !=3)
		throw "Failed to calc height for 3-line diff-width strings";
	if (size.Width != 6)
		throw "Failed to calc width for 3-line diff-width strings";
	printableView = AllocAndConvertBuffAsPrintable(buff, size, '%');
	if(printableView == NULL)
		throw "Failed to create buff from 3-line diff-width strings. Error getting printable.";
	if (!StringEquals(printableView, test))
		throw "Failed to create buff from 3-line diff-width strings. Incorrect result ";
	delete buff;
	delete printableView;
}

void xTestStringHelpers::xStrCopy()
{
	char dest[50]; 
	char* test;
	fresult fres;

	strcpy_s(dest,"kkkkkk");
	test = "dfgkkk";
	
	fres = StrCopy(dest, 0, "dfgh", 3);
	if (fres!=SUCCESS)
		throw "Cant copy 3 chars dfgh to kkkkkk at 0.";
	if (!StringEquals(dest, test))
		throw  "Cant copy 3 chars dfgh to kkkkkk at 0. Wrong result";

	strcpy_s(dest,"kkkkkk");
	test = "kkkdfg";
	fres = StrCopy(dest, 3, "dfgh", 3);
	if (fres!=SUCCESS)
		throw "Cant copy 3 chars dfgh to kkkkkk at 3.";
	if (!StringEquals(dest, test))
		throw  "Cant copy 3 chars dfgh to kkkkkk at 3. Wrong result";

}

void xTestStringHelpers::xStrPad()
{
	char dest[50];
	char* test;
	fresult fres;

	test = "@@@kkk";

	strcpy_s(dest,"kkkkkk");
	fres = StrPad(dest, 0, '@', 3);
	if (fres!=SUCCESS)
		throw "Cant pad 3 chars @ to kkkkkk at 0.";
	if (!StringEquals(dest, test))
		throw  "Cant pad 3 chars @ to kkkkkk at 0.. Wrong result";

	strcpy_s(dest,"kkkkkk");
	test = "kkk@@@";
	fres = StrPad(dest, 3, '@', 3);
	if (fres!=SUCCESS)
		throw "Cant pad 3 chars @ to kkkkkk at 3.";
	if (!StringEquals(dest, test))
		throw  "Cant pad 3 chars @ to kkkkkk at 3. Wrong result";
}









