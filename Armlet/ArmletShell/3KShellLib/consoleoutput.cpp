
#include <iostream>
#include <windows.h>
using namespace std;

struct colors_t
  {
  HANDLE hstdout;
  int    initial_colors;

  colors_t()
    {
    hstdout        = GetStdHandle( STD_OUTPUT_HANDLE );
    initial_colors = getcolors();
    }

  ~colors_t()
    {
    setcolors( initial_colors );
    }

  int getcolors() const
    {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo( hstdout, &csbi );
    return csbi.wAttributes;
    }

  void setcolors( int color )
    {
    SetConsoleTextAttribute( hstdout, color );
    }

  void setfg( int color )
    {
    int current_colors = getcolors();
    setcolors( (current_colors & 0xF0) | (color & 0x0F) );
    }

  void setbg( int color )
    {
    int current_colors = getcolors();
    setcolors( ((color & 0x0F) << 4) | (current_colors & 0x0F) );
    }

  int getfg() const { return  getcolors()    & 0x0F; }
  int getbg() const { return (getcolors() >> 4) & 0x0F; }
  };

enum {
  black,
  dark_blue,
  dark_green,
  dark_cyan,
  dark_red,
  dark_magenta,
  dark_yellow,
  light_gray,
  dark_gray,
  light_blue,
  light_green,
  light_cyan,
  light_red,
  light_magenta,
  light_yellow,
  white
  };

/*int main()
  {
  colors_t colors;

  int row = 1;
  int col = 3;

  int matrix[ 5 ][ 5 ] = {
    { 0, 1, 2, 3, 4 },
    { 1, 2, 3, 4, 0 },
    { 2, 3, 4, 0, 1 },
    { 3, 4, 0, 1, 2 },
    { 4, 0, 1, 2, 3 }
    };

  cout << "row = " << row << "; col = " << col << endl;

  for (int r = 0; r < 5; r++)
    {
    for (int c = 0; c < 5; c++)
      {
      if ((r == row) && (c == col))
        colors.setfg( light_yellow );
      cout << matrix[ r ][ c ];
      colors.setcolors( colors.initial_colors );
      cout << " ";
      }
    cout << endl;
    }

  return 0;
  }*/