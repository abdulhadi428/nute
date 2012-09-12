/*
 * menu.cpp
 *
 *  Created on: 26.08.2012
 *      Author: kreyl
 */

#include "interface.h"

Interface_t Interface;

// =================================== Keys ====================================
void Interface_t::KeyDown(bool IsLongPressed) {
    switch(State) {
        case stIdle:
            if(ISelection == sMinLvl) Select(sMaxLvl);
            break;

        case stChanging:
            Delay.Reset(&IStateTmr);
            if(ISelection == sMinLvl) {
                MinLvl--;
                if(MinLvl < -111) MinLvl = -111;
                DisplayMinLvl();
            }
            else {
                MaxLvl--;
                if(MaxLvl < -111) MaxLvl = -111;
                DisplayMaxLvl();
            }
            break;
    } // switch
}

void Interface_t::KeyUp(bool IsLongPressed) {
    switch(State) {
        case stIdle:
            if(ISelection == sMaxLvl) Select(sMinLvl);
            break;

        case stChanging:
            Delay.Reset(&IStateTmr);
            if(ISelection == sMinLvl) {
                MinLvl++;
                if(MinLvl > 0) MinLvl = 0;
                DisplayMinLvl();
            }
            else {
                MaxLvl++;
                if(MaxLvl > 0) MaxLvl = 0;
                DisplayMaxLvl();
            }
            break;
    }
}

void Interface_t::KeyEnter() {
    switch(State) {
        case stIdle:
            State = stChanging;
            Delay.Reset(&IStateTmr);
            break;

        case stChanging:
            EnterIdle();
            SettingChanged();
            break;
    }
}


// ==== Task ====
void Interface_t::Task() {
    switch(State) {
        case stIdle:

            break;

            case stChanging:
                // Implement blinking
                if(Delay.Elapsed(&IBlinkTmr, BLINK_DELAY)) {
                    if(ISelection == sMinLvl) {
                        if(IsDisplayed) Lcd.Printf(DATA_X, 1, "    ");
                        else            Lcd.Printf(DATA_X, 1, "%i", MinLvl);
                    }
                    else {
                        if(IsDisplayed) Lcd.Printf(DATA_X, 3, "    ");
                        else            Lcd.Printf(DATA_X, 3, "%i", MaxLvl);
                    }
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
    DisplayMinLvl();
    DisplayMaxLvl();
}

void Interface_t::Select(Selection_t ASelection) {
    uint8_t y = (ISelection == sMinLvl)? 1 : 3;
    // Clear current selection
    Lcd.GotoCharXY(9, y);
    Lcd.DrawChar(' ', NotInverted);
    Lcd.GotoCharXY(14, y);
    Lcd.DrawChar(' ', NotInverted);
    // Select now
    ISelection = ASelection;
    y = (ISelection == sMinLvl)? 1 : 3;
    Lcd.GotoCharXY(9, y);
    Lcd.DrawChar(0x10, NotInverted);
    Lcd.GotoCharXY(14, y);
    Lcd.DrawChar(0x11, NotInverted);
}

void Interface_t::Init() {
    // Draw background
    Lcd.Symbols(0, 0,   // Top line
            CornerTopLeftDouble, 1,
            LineHorizDouble, 7,
            LineHorizDoubleDown, 1,
            LineHorizDouble, 6,
            CornerTopRightDouble, 1,
            0);
    Lcd.Printf(0, 1, "%cMIN Lvl%c      %c", LineVertDouble, LineVertDouble, LineVertDouble);
    Lcd.Symbols(0, 2,   // Middle line
            LineVertDoubleRight, 1,
            LineHorizDouble, 7,
            LineCrossDouble, 1,
            LineHorizDouble, 6,
            LineVertDoubleLeft, 1,
            0);
    Lcd.Printf(0, 3, "%cMAX Lvl%c      %c", LineVertDouble, LineVertDouble, LineVertDouble);
    Lcd.Symbols(0, 4,   // Middle line
            LineVertDoubleRight, 1,
            LineHorizDouble, 7,
            LineCrossDouble, 1,
            LineHorizDouble, 6,
            LineVertDoubleLeft, 1,
            0);

    // Current value
    Lcd.Printf(0, 5, "%c ID    %c      %c", LineVertDouble, LineVertDouble, LineVertDouble);
    Lcd.Printf(0, 6, "%c RX Lvl%c      %c", LineVertDouble, LineVertDouble, LineVertDouble);
    Lcd.Symbols(0, 7,   // Bottom line
            CornerBottomLeftDouble, 1,
            LineHorizDouble, 7,
            LineHorizDoubleUp, 1,
            LineHorizDouble, 6,
            CornerBottomRightDouble, 1,
            0);
    // Initial selection
    ISelection = sMinLvl;
    Select(sMinLvl);
}
