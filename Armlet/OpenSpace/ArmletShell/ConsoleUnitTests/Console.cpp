
#include "Console.h"

using namespace std;


  Console::Console()
    {
    hstdout        = GetStdHandle( STD_OUTPUT_HANDLE );
    initial_colors = getcolors();
    }
  
  Console::~Console()
    {
    setcolors( initial_colors );
    }
	
  int Console::getcolors() const
    {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo( hstdout, &csbi );
    return csbi.wAttributes;
    }

  void Console::setcolors( int color )
    {
    SetConsoleTextAttribute( hstdout, color );
    }

  void Console::setfg( int color )
    {
    int current_colors = getcolors();
    setcolors( (current_colors & 0xF0) | (color & 0x0F) );
    }

  void Console::setbg( int color )
    {
    int current_colors = getcolors();
    setcolors( ((color & 0x0F) << 4) | (current_colors & 0x0F) );
    }

  char Console::getcolor(int forground, int background)
  {
	  char color = ((background & 0x0F) << 4) | (forground & 0x0F);
	  return color;
  }
  void Console::putchar(int x, int y, char charater, char format)
  {
	  CONSOLE_SCREEN_BUFFER_INFO csbiInfo;

	  GetConsoleScreenBufferInfo(hstdout, &csbiInfo);
	  csbiInfo.dwCursorPosition.X=x;
	  csbiInfo.dwCursorPosition.Y=y;
	  SetConsoleCursorPosition(hstdout, csbiInfo.dwCursorPosition);
	  setcolors(format);
	  cout<< charater;

	  setcolors( initial_colors );
  }


