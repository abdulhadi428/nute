/*
 * interface.cpp of Armlet1 project
 *
 *  Created on: 15.09.2012
 *      Author: kreyl
 */

#include "interface.h"
#include "kl_time.h"

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
    static uint8_t OldMinute;
    uint8_t Minute = Time.GetMinute();
    switch(State) {
        case stIdle:
            // Display time
            if(Minute != OldMinute) {
                Lcd.Printf(0, 0, "%02u:%02u", Time.GetHour(), Minute);
                OldMinute = Minute;
            }
            break;

        case stSetTime:
            if(Delay.Elapsed(&IStateTmr, STATE_DELAY)) EnterIdle();
            break;

        default:
            break;
    }
}

// ================================= Menus ===================================
void Interface_t::EnterIdle() {
    State = stIdle;
    Lcd.Cls();
    // Display time
    Lcd.Printf(0, 0, "%02u:%02u", Time.GetHour(), Time.GetMinute());
}

// ==== Set Time menu ====
void Interface_t::EnterSetTime() {
    Lcd.Cls();
    Lcd.Printf(0, 1, "Установите время");
    //Lcd.Printf(4, 4, " %u2 : %u2 ", Time.GetHour(), Time.GetMinute());
}



void Interface_t::Init() {
    // Draw background
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
