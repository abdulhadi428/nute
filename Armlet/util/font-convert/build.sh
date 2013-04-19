#!/bin/sh
#export FT_DIR="../freetype-2.4.8"
g++ main.cpp -g -O0 -o font-convert -I/usr/include/freetype2 -lfreetype
#-L${FT_DIR}/objs/.libs 
#-lfreetype
