/*
 * menu.cpp
 *
 *  Created on: 26.08.2012
 *      Author: kreyl
 */

#include "interface.h"
#include "beep.h"

Interface_t Interface;

// =================================== Keys ====================================
void Interface_t::KeyDown(bool IsLongPressed) {
    Beep.Squeak(1, 7);
    switch(State) {
        case stIdle:
            if(SelectionIndx < T_TOSET_COUNT-1) Select(SelectionIndx+1);
            break;

        case stChanging:
            Delay.Reset(&IStateTmr);
            if(IsLongPressed and (tToSet[SelectionIndx] %10 == 0)) tToSet[SelectionIndx] -=10;
            else tToSet[SelectionIndx]--;
            if(tToSet[SelectionIndx] < 0) tToSet[SelectionIndx] = 0;
            DisplayTToset(SelectionIndx);
            break;
    }

}

void Interface_t::KeyUp(bool IsLongPressed) {
    Beep.Squeak(1, 7);
    switch(State) {
        case stIdle:
            if(SelectionIndx != 0) Select(SelectionIndx-1);
            break;

        case stChanging:
            Delay.Reset(&IStateTmr);
            if(IsLongPressed and (tToSet[SelectionIndx] %10 == 0)) tToSet[SelectionIndx] +=10;
            else tToSet[SelectionIndx]++;
            if(tToSet[SelectionIndx] > 500) tToSet[SelectionIndx] = 500;
            DisplayTToset(SelectionIndx);
            break;
    }
}

void Interface_t::KeyEnter() {
    Beep.Squeak(1, 7);
    switch(State) {
        case stIdle:
            State = stChanging;
            Delay.Reset(&IStateTmr);
            break;

        case stChanging:
            EnterIdle();
            break;
    }
}


// ==== Task ====
void Interface_t::Task() {
    switch(State) {
        case stIdle:

            break;

            case stChanging:
                if(Delay.Elapsed(&IBlinkTmr, BLINK_DELAY)) {
                    if(IsDisplayed) Lcd.Printf(10, 3+SelectionIndx*2, "    ");
                    else            Lcd.Printf(10, 3+SelectionIndx*2, "%i", tToSet[SelectionIndx]);
                    IsDisplayed = !IsDisplayed;
                }
                // Check if time go back to Idle
                if(Delay.Elapsed(&IStateTmr, STATE_DELAY)) EnterIdle();
                break;
    }
}

// ================================= General ===================================
void Interface_t::EnterIdle() {
    State = stIdle;
    Lcd.Printf(10, 3+SelectionIndx*2, "%i", tToSet[SelectionIndx]); // selection can be erased during blink
}

void Interface_t::Select(uint8_t AIndx) {
    // Clear current selection
    Lcd.GotoCharXY(8, 3+SelectionIndx*2);
    Lcd.DrawChar(' ', NotInverted);
    Lcd.GotoCharXY(14, 3+SelectionIndx*2);
    Lcd.DrawChar(' ', NotInverted);
    // Select now
    SelectionIndx = AIndx;
    Lcd.GotoCharXY(8, 3+SelectionIndx*2);
    Lcd.DrawChar(0x10, NotInverted);
    Lcd.GotoCharXY(14, 3+SelectionIndx*2);
    Lcd.DrawChar(0x11, NotInverted);
}

void Interface_t::Init() {
    // Draw background
    Lcd.Symbols(0, 0,
            CornerTopLeftDouble, 1,
            LineHorizDouble, 1,
            LineHorizDoubleDown, 1,
            LineHorizDouble, 4,
            LineHorizDoubleDown, 1,
            LineHorizDouble, 7,
            CornerTopRightDouble, 1,
            0);
    Lcd.Printf(0, 1, "%c#%ct in%c t set %c", LineVertDouble, LineVertDouble, LineVertDouble, LineVertDouble);
    for(uint8_t i=0; i<ADS_CH_COUNT; i++) {
        Lcd.Symbols(0, 2+i*2,
                LineVertDoubleRight, 1,
                LineHorizDouble, 1,
                LineCrossDouble, 1,
                LineHorizDouble, 4,
                LineCrossDouble, 1,
                LineHorizDouble, 7,
                LineVertDoubleLeft, 1,
                0);
        Lcd.Printf(0, 3+i*2, "%c%u%c    %c       %c", LineVertDouble, i+1, LineVertDouble, LineVertDouble, LineVertDouble);
    }
    // Last string
    Lcd.Symbols(0, 2+ADS_CH_COUNT*2,
            CornerBottomLeftDouble, 1,
            LineHorizDouble, 1,
            LineHorizDoubleUp, 1,
            LineHorizDouble, 4,
            LineHorizDoubleUp, 1,
            LineHorizDouble, 7,
            CornerBottomRightDouble, 1,
            0);
    // Initial setup values
    for(uint8_t i=0; i<T_TOSET_COUNT; i++) {
        tToSet[i] = 180;
        DisplayTToset(i);
    }
    // Initial selection
    SelectionIndx = 0;
    Select(0);
}
