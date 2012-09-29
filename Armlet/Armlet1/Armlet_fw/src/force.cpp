/*
 * force.cpp
 *
 *  Created on: 29.09.2012
 *      Author: kreyl
 */

#include "force.h"
#include "lcd1200.h"
#include "aurebesh.h"
#include "kl_lib.h"

void Force_t::DrawForce() {
    if(!IsForceUser) return;
    static uint32_t Tmr, indx=0;
    static bool IsOn = true;
    // Iterate symbols
    if(Delay.Elapsed(&Tmr, 360)) {
        if(indx == 0) Lcd.DrawImage(0, 0,  IsOn? Aur1211j : Aur1211space);
        if(indx == 1) Lcd.DrawImage(0, 16, IsOn? Aur1211o : Aur1211space);
        if(indx == 2) Lcd.DrawImage(0, 32, IsOn? Aur1211t : Aur1211space);
        if(indx == 3) Lcd.DrawImage(0, 48, IsOn? Aur1211s : Aur1211space);
        indx++;
        if((IsOn and (indx > JediCount)) or (indx >= 4)) {
            indx = 0;
            IsOn = !IsOn;
        }
    }
}


