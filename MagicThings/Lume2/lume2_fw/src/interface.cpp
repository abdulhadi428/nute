/*
 * interface.cpp of Armlet1 project
 *
 *  Created on: 15.09.2012
 *      Author: kreyl
 */

#include "interface.h"

Interface_t Interface;

// =================================== Keys ====================================
void Interface_t::IInc(bool AFast, int32_t *PValue) {
    if(AFast and ((*PValue) %10 == 0)) (*PValue) +=10;
    else (*PValue)++;
    if((*PValue) > 0) (*PValue) = 0;
}
void Interface_t::IDec(bool AFast, int32_t *PValue) {
    if(AFast and ((*PValue) %10 == 0)) (*PValue) -=10;
    else (*PValue)--;
    if((*PValue) < -111) (*PValue) = -111;
}

/*
// ==== Keys ====

void Interface_t::KeyEnter() {
    switch(State) {
        case stIdle:
            State = stSetTime;
            Delay.Reset(&IStateTmr);
            EnterSetTime();
            break;

        default:
            EnterIdle();
            break;
    }
}*/


// ==== Task ====
void Interface_t::Task() {
    static uint32_t STmr=0;
    //static uint8_t OldMinute;
    //uint8_t Minute = Time.GetMinute();
    switch(State) {
        case stIdle:
            if(Time.SecElapsed(&STmr, 1)) {
                Time.GetDateTime(&IDateTime);
                DisplayTime();
                // Display Date
                if(IDateTime.H == 0 and IDateTime.M == 0 and IDateTime.S == 0) {
                    DisplayYear();
                    DisplayMonth();
                    DisplayDay();
                }
            }
            break;

        case stSetTime:
         //   if(Delay.Elapsed(&IStateTmr, STATE_DELAY)) EnterIdle();
            break;

        default:
            break;
    }
}

// ================================= Menus ===================================
void Interface_t::EnterIdle() {
    State = stIdle;
}

// ==== Set Time menu ====
void Interface_t::EnterSetTime() {
    Lcd.Cls();
    Lcd.Printf(0, 1, "Установите время");
    //Lcd.Printf(4, 4, " %u2 : %u2 ", Time.GetHour(), Time.GetMinute());
}

// ================================= Common ====================================
void Interface_t::DisplayTime() {
    Lcd.Printf(7, 2, "%02u:%02u:%02u", IDateTime.H, IDateTime.M, IDateTime.S);
}



void Interface_t::Init() {
    // Draw background
    Lcd.Printf(0, 2, "Time:  ");
    Lcd.Printf(0, 4, "Year:  ");
    Lcd.Printf(0, 5, "Month: ");
    Lcd.Printf(0, 6, "Day:   ");

    DisplayTime();
    DisplayYear();
    DisplayMonth();
    DisplayDay();

//    Lcd.Symbols(0, 0,   // Top line
//            CornerTopLeftDouble, 1,
//            LineHorizDouble, 7,
//            LineHorizDoubleDown, 1,
//            LineHorizDouble, 6,
//            CornerTopRightDouble, 1,
//            0);
//    Lcd.Printf(0, 1, "%cMIN Lvl%c      %c", LineVertDouble, LineVertDouble, LineVertDouble);
//    Lcd.Symbols(0, 2,   // Middle line
//            LineVertDoubleRight, 1,
//            LineHorizDouble, 7,
//            LineCrossDouble, 1,
//            LineHorizDouble, 6,
//            LineVertDoubleLeft, 1,
//            0);
//    Lcd.Printf(0, 3, "%cMAX Lvl%c      %c", LineVertDouble, LineVertDouble, LineVertDouble);
//    Lcd.Symbols(0, 4,   // Middle line
//            LineVertDoubleRight, 1,
//            LineHorizDouble, 7,
//            LineCrossDouble, 1,
//            LineHorizDouble, 6,
//            LineVertDoubleLeft, 1,
//            0);
//
//    // Current value
//    Lcd.Printf(0, 5, "%c ID    %c      %c", LineVertDouble, LineVertDouble, LineVertDouble);
//    Lcd.Printf(0, 6, "%c RX Lvl%c      %c", LineVertDouble, LineVertDouble, LineVertDouble);
//    Lcd.Symbols(0, 7,   // Bottom line
//            CornerBottomLeftDouble, 1,
//            LineHorizDouble, 7,
//            LineHorizDoubleUp, 1,
//            LineHorizDouble, 6,
//            CornerBottomRightDouble, 1,
//            0);

    EnterIdle();
}
