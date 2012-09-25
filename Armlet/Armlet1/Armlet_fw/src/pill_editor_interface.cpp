/*
 * pill_editor_interface.cpp
 *
 *  Created on: 25.09.2012
 *      Author: kreyl
 */

#include "pill_editor_interface.h"
#include "lcd1200.h"
#include "keys.h"

PEIntf_t PEInterface;

void PEIntf_t::Init() {
    Lcd.Printf(0, 0, "Pill Editor");
    IState = pstNoPill;
    Clear();
    SelId = selType;
}

void PEIntf_t::Task() {
    switch(IState) {
        case pstNoPill:
            if(Pill.State == esNew) {
                IState = pstReading;
                Pill.Read(0, (uint8_t*)&IPillData, 2);
            }
            break;

        case pstReading:
            if(Pill.State == esReady) {
                IState = pstIdle;
                // Prepare display
                Lcd.Printf(0, 4, "Type:");
                Lcd.Printf(0, 5, "Charge:");
                PrintType(true);
                PrintCharge(true);
                Select(selType);
            }
            break;

        case pstWriting:
            if(Pill.State == esReady) {
                IState = pstIdle;
            }
            break;

        case pstIdle:
            if(Pill.State == esFailure) {
                IState = pstNoPill;
                Clear();
            }
            else MenuHandler();
            break;
    } // switch
}

void PEIntf_t::MenuHandler() {
    switch(MState) {
        case msIdle:
            if(Keys.Down.IsPressed()) Select(selChrg);
            if(Keys.Up.IsPressed()) Select(selType);
            if(Keys.Enter.IsPressed()) {
                MState = (SelId == selType)? msTypeEdit : msChargeEdit;
                IsDisplayed = true;
                Delay.Reset(&Tmr);
            }
            break;

        case msTypeEdit:
            // Blink the value
            if(Delay.Elapsed(&Tmr, BLINK_DELAY)) {
                IsDisplayed = !IsDisplayed;
                PrintType(IsDisplayed);
            }
            // Handle keys
            if(Keys.Down.IsPressed() and (IPillData.Type != 0)) IPillData.Type--;
            if(Keys.Up.IsPressed() and (IPillData.Type < 2)) IPillData.Type++;
//            if(Keys.Enter.IsPressed()) {
//                PrintType(true);
//                IState = pstWriting;
//                Pill.Write(0, (uint8_t*)&IPillData, 2);
//            }
            break;

        case msChargeEdit:
            // Blink the value
            if(Delay.Elapsed(&Tmr, BLINK_DELAY)) {
                IsDisplayed = !IsDisplayed;
                PrintCharge(IsDisplayed);
            }
            // Handle keys
            if(Keys.Down.IsPressed()) IPillData.ChargeCount--;
            if(Keys.Up.IsPressed()) IPillData.ChargeCount++;
//            if(Keys.Enter.IsPressed()) {
//                PrintCharge(true);
//                IState = pstWriting;
//                Pill.Write(0, (uint8_t*)&IPillData, 2);
//            }
            break;
    }
}

void PEIntf_t::Select(PESelection_t NewSelId) {
    SelId = NewSelId;
    uint8_t ySel   = (NewSelId == selType)? 4 : 5;
    uint8_t yDesel = (NewSelId == selType)? 5 : 4;
    // Select new
    Lcd.Printf(7,  ySel, "%c", 0x10);
    Lcd.Printf(15, ySel, "%c", 0x11);
    // Deselect previous
    Lcd.Printf(7,  yDesel, " ");
    Lcd.Printf(15, yDesel, " ");
}

void PEIntf_t::PrintType(bool ShowNotClear) {
    if(ShowNotClear) {
        switch(IPillData.Type) {
            case PILL_TYPE_BACTA:   Lcd.Printf(8, 4, " Bacta "); break;
            case PILL_TYPE_ANTIBIO: Lcd.Printf(8, 4, "Antibio"); break;
            case PILL_TYPE_BANDAGE: Lcd.Printf(8, 4, "Bandage"); break;
            default:                Lcd.Printf(8, 4, "Not set"); break;
        }
    }
    else Lcd.Printf(8, 4, "       ");
}

void PEIntf_t::PrintCharge(bool ShowNotClear) {
    if(ShowNotClear) Lcd.Printf(10, 5, "%03u", IPillData.ChargeCount);
    else Lcd.Printf(10, 5, "   ", IPillData.ChargeCount);
}

void PEIntf_t::Clear() {
    Lcd.Printf(0, 4, " No pill        ");
    Lcd.Printf(0, 5, "                ");
}
