/*
 * interface.cpp of Armlet1 project
 *
 *  Created on: 15.09.2012
 *      Author: kreyl
 */

#include "interface.h"
#include "keys.h"

Interface_t Interface;

// =================================== Keys ====================================
void Interface_t::IInc(bool AFast, int32_t *PValue, int32_t AMaxValue, int32_t AMinValue) {
    if(AFast and ((*PValue) %10 == 0)) (*PValue) +=10;
    else (*PValue)++;
    if((*PValue) > AMaxValue) (*PValue) = AMinValue;
}
void Interface_t::IDec(bool AFast, int32_t *PValue, int32_t AMaxValue, int32_t AMinValue) {
    if(AFast and ((*PValue) %10 == 0)) (*PValue) -=10;
    else (*PValue)--;
    if((*PValue) < AMinValue) (*PValue) = AMaxValue;
}


// ==== Task ====
void Interface_t::Task() {
    static uint32_t STmr=0, IBlinkTmr=0, IStateTmr=0;
    static bool IsDisplayed;
    switch(State) {
        case msIdle:
            // Display time if needed
            if(Time.SecElapsed(&STmr, 1)) {
                Time.GetDateTime(&IDateTime);
                DisplayHour(true);
                DisplayMinute(true);
                DisplaySecond(true);
                // Display New Date
                if(IDateTime.H == 0 and IDateTime.M == 0 and IDateTime.S == 0) {
                    DisplayYear(true);
                    DisplayMonth(true);
                    DisplayDay(true);
                }
            }
            // Handle keys
            if(Keys.Enter.WasJustPressed()) {
                State = msHour;
                IsDisplayed = true;
                Delay.Reset(&IBlinkTmr);
                Delay.Reset(&IStateTmr);    // to return to idle
            }
            break;

        case msHour:
            // Handle keys
            if(Keys.Down.WasJustPressed()) {
                IDec(false, &IDateTime.H, 23, 0);
                Delay.Reset(&IStateTmr);
                DisplayHour(IsDisplayed = true);
            }
            else if(Keys.Up.WasJustPressed()) {
                IInc(false, &IDateTime.H, 23, 0);
                Delay.Reset(&IStateTmr);
                DisplayHour(IsDisplayed = true);
            }
            else if(Keys.Enter.WasJustPressed()) {
                IDateTime.S = 0;
                DisplaySecond(true);
                Delay.Reset(&IStateTmr);
                DisplayHour(IsDisplayed = true);
                State = msMinute;
                Time.SetTime(IDateTime.H, IDateTime.M, 0);
            }
            // Handle autoexit
            else if(Delay.Elapsed(&IStateTmr, STATE_DELAY)) {
                DisplayHour(true);
                State = msIdle;
                Time.SetTime(IDateTime.H, IDateTime.M, 0);
            }
            // Handle blink
            else if(!Keys.Down.IsPressed() and !Keys.Up.IsPressed() and Delay.Elapsed(&IBlinkTmr, BLINK_DELAY))
                DisplayHour(IsDisplayed = !IsDisplayed);   // Blink the value
            break;

        case msMinute:
            // Handle keys
            if(Keys.Down.WasJustPressed()) {
                IDec(false, &IDateTime.M, 59, 0);
                Delay.Reset(&IStateTmr);
                DisplayMinute(IsDisplayed = true);
            }
            else if(Keys.Up.WasJustPressed()) {
                IInc(false, &IDateTime.M, 59, 0);
                Delay.Reset(&IStateTmr);
                DisplayMinute(IsDisplayed = true);
            }
            else if(Keys.Enter.WasJustPressed()) {
                Delay.Reset(&IStateTmr);
                DisplayMinute(true);
                State = msYear;
                Time.SetTime(IDateTime.H, IDateTime.M, 0);
            }
            // Handle autoexit
            else if(Delay.Elapsed(&IStateTmr, STATE_DELAY)) {
                DisplayMinute(true);
                State = msIdle;
                Time.SetTime(IDateTime.H, IDateTime.M, 0);
            }
            // Handle blink
            else if(!Keys.Down.IsPressed() and !Keys.Up.IsPressed() and Delay.Elapsed(&IBlinkTmr, BLINK_DELAY))
                DisplayMinute(IsDisplayed = !IsDisplayed);   // Blink the value
            break;

        case msYear:
            // Handle keys
            if(Keys.Down.WasJustPressed()) {
                IDec(false, &IDateTime.Year, 2700, 0);
                Delay.Reset(&IStateTmr);
                DisplayYear(IsDisplayed = true);
            }
            else if(Keys.Up.WasJustPressed()) {
                IInc(false, &IDateTime.Year, 2700, 0);
                Delay.Reset(&IStateTmr);
                DisplayYear(IsDisplayed = true);
            }
            else if(Keys.Enter.WasJustPressed()) {
                DisplayYear(true);
                State = msMonth;
                Time.SetDate(IDateTime.Year, IDateTime.Month, IDateTime.Day);
            }
            // Handle autoexit
            else if(Delay.Elapsed(&IStateTmr, STATE_DELAY)) {
                DisplayYear(true);
                State = msIdle;
                Time.SetDate(IDateTime.Year, IDateTime.Month, IDateTime.Day);
            }
            // Handle blink
            else if(!Keys.Down.IsPressed() and !Keys.Up.IsPressed() and Delay.Elapsed(&IBlinkTmr, BLINK_DELAY))
                DisplayYear(IsDisplayed = !IsDisplayed);   // Blink the value
            break;

        case msMonth:
            // Handle keys
            if(Keys.Down.WasJustPressed()) {
                IDec(false, &IDateTime.Month, 12, 1);
                Delay.Reset(&IStateTmr);
                DisplayMonth(IsDisplayed = true);
            }
            else if(Keys.Up.WasJustPressed()) {
                IInc(false, &IDateTime.Month, 12, 1);
                Delay.Reset(&IStateTmr);
                DisplayMonth(IsDisplayed = true);
            }
            else if(Keys.Enter.WasJustPressed()) {
                DisplayMonth(true);
                State = msDay;
                Time.SetDate(IDateTime.Year, IDateTime.Month, IDateTime.Day);
            }
            // Handle autoexit
            else if(Delay.Elapsed(&IStateTmr, STATE_DELAY)) {
                DisplayMonth(true);
                State = msIdle;
                Time.SetDate(IDateTime.Year, IDateTime.Month, IDateTime.Day);
            }
            // Handle blink
            else if(!Keys.Down.IsPressed() and !Keys.Up.IsPressed() and Delay.Elapsed(&IBlinkTmr, BLINK_DELAY))
                DisplayMonth(IsDisplayed = !IsDisplayed);   // Blink the value
            break;

        case msDay:
            // Handle keys
            if(Keys.Down.WasJustPressed()) {
                IDec(false, &IDateTime.Day, 31, 1);
                Delay.Reset(&IStateTmr);
                DisplayDay(IsDisplayed = true);
            }
            else if(Keys.Up.WasJustPressed()) {
                IInc(false, &IDateTime.Day, 31, 1);
                Delay.Reset(&IStateTmr);
                DisplayDay(IsDisplayed = true);
            }
            else if(Keys.Enter.WasJustPressed()) {
                DisplayDay(true);
                State = msIdle;
                Time.SetDate(IDateTime.Year, IDateTime.Month, IDateTime.Day);
            }
            // Handle autoexit
            else if(Delay.Elapsed(&IStateTmr, STATE_DELAY)) {
                DisplayDay(true);
                State = msIdle;
                Time.SetDate(IDateTime.Year, IDateTime.Month, IDateTime.Day);
            }
            // Handle blink
            else if(!Keys.Down.IsPressed() and !Keys.Up.IsPressed() and Delay.Elapsed(&IBlinkTmr, BLINK_DELAY))
                DisplayDay(IsDisplayed = !IsDisplayed);   // Blink the value
            break;

        default:
            break;
    }
}

// ================================= Common ====================================
void Interface_t::Init() {
    // Draw background
    Lcd.Printf(0, 2, "Time:  ");
    Lcd.Printf(0, 4, "Year:  ");
    Lcd.Printf(0, 5, "Month: ");
    Lcd.Printf(0, 6, "Day:   ");

    Time.GetDateTime(&IDateTime);
    DisplayHour(true);
    DisplayMinute(true);
    DisplaySecond(true);
    DisplayYear(true);
    DisplayMonth(true);
    DisplayDay(true);

    State = msIdle;
}
