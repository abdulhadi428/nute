#pragma once

#include <iostream>
#include <windows.h>
using namespace std;

class Console
  {
	  HANDLE hstdout;
	  int    initial_colors;
public:
  Console();

  ~Console();

  int getcolors() const;

  void setcolors( int color );

  void setfg( int color );

  void setbg( int color );

  char getcolor(int forground, int background);

  int getfg() const { return  getcolors()    & 0x0F; }
  int getbg() const { return (getcolors() >> 4) & 0x0F; }

  void putchar(int x, int y, char charater, char format);


  };

enum ConsoleColor{
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

//Example
/*
int main()
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
  }

  */