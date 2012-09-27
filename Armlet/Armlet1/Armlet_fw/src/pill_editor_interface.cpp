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
                Uart.Printf("R\r");
            }
            break;

        case pstReading:
            if(Pill.State == esReady) {
                IState = pstIdle;
                MState = msIdle;
                // Prepare display
                Lcd.Printf(0, 4, "Type:  ");
                Lcd.Printf(0, 5, "Charge:");
                PrintType(true);
                PrintCharge(true);
                Select(selType);
                //Uart.Printf("R Idle\r");
            }
            else if(Pill.State != esBusy) IState = pstNoPill;
            break;

        case pstWriting:
            if(Pill.State == esReady) {
                MState = msIdle;
                IState = pstIdle;
            }
            else if(Pill.State == esNew) {
                //Uart.Printf("W New\r");
                Lcd.Printf(0, 6, "Write failed");
                IState = pstReading;
                Pill.Read(0, (uint8_t*)&IPillData, 2);
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
            if(Keys.Down.WasJustPressed()) Select(selChrg);
            if(Keys.Up.WasJustPressed()) Select(selType);
            if(Keys.Enter.WasJustPressed()) {
                if(SelId == selType) {
                    MState = msTypeEdit;
                    if(IPillData.Type > 2) {    // Handle NotSet condition
                        IPillData.Type = 0;
                        PrintType(IsDisplayed);
                    }
                }
                else MState = msChargeEdit;
                IsDisplayed = true;
                Delay.Reset(&Tmr);
            }
            break;

        case msTypeEdit:
            // Blink the value
            if(Delay.Elapsed(&Tmr, PE_BLINK_DELAY)) {
                IsDisplayed = !IsDisplayed;
                PrintType(IsDisplayed);
            }
            // Handle keys
            if(Keys.AnyKeyWasJustPressed()) {
                // Handle individual keys
                if(Keys.Down.WasJustPressed() and (IPillData.Type != 0)) IPillData.Type--;
                if(Keys.Up.WasJustPressed() and (IPillData.Type < 2))    IPillData.Type++;
                PrintType(true);
                IsDisplayed = true;
                Delay.Reset(&Tmr);
                if(Keys.Enter.WasJustPressed()) {
                    IState = pstWriting;
                    Pill.Write(0, (uint8_t*)&IPillData, 2);
                    //Uart.Printf("WT\r");
                }
            }
            break;

        case msChargeEdit:
            // Blink the value
            if(Delay.Elapsed(&Tmr, PE_BLINK_DELAY)) {
                IsDisplayed = !IsDisplayed;
                PrintCharge(IsDisplayed);
            }
            // Handle keys
            if(Keys.AnyKeyWasJustPressed()) {
                // Handle individual keys
                if(Keys.Down.WasJustPressed()) IPillData.ChargeCount--;
                if(Keys.Up.WasJustPressed())   IPillData.ChargeCount++;
                PrintCharge(true);
                IsDisplayed = true;
                Delay.Reset(&Tmr);
                if(Keys.Enter.WasJustPressed()) {
                    IState = pstWriting;
                    Pill.Write(0, (uint8_t*)&IPillData, 2);
                    //Uart.Printf("WC\r");
                }
            }
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
    Lcd.Printf(0, 6, "                ");
}
