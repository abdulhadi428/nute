/*
 * menu.cpp
 *
 *  Created on: 26.08.2012
 *      Author: kreyl
 */

#include "interface.h"
#include "lcd1200.h"

Interface_t Interface;

void Interface_t::Init() {
    // Draw background
    // Top line 0x2554 0x2550 0x2557
    Lcd.Printf(0, 0, "Qwe");
    //Lcd.PrintStringUtf8(0, 0, "╔ ═ ╗");
}
