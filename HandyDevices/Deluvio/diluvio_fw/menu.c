#include "menu.h"
#include "main.h"
#include "lcd110x.h"
#include "beep.h"
#include "delay_util.h"
#include "sensors.h"
#include "time.h"
#include "messages.h"
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "battery.h"

// ============================= Strings =======================================
prog_char chrPumps[4][8] = {"Насос 1", "Насос 2", "Насос 3", "Насос 4"};
prog_char chrSetTime[] = {MSG_MENU_SET_TIME};
prog_char chrMenuSetTime[] = {MSG_SET_CORRECT_TIME};
prog_char chrBack[] = "Назад";
prog_char chrOnOff[4][11] = {"  Включен", "  Выключен", "* Включен", "* Выключен"};
prog_char chrDaysHours[4][8] = {"  дней", "  часов", " [дней", " [часов"};

prog_char chrPeriod[] = "Период ";
prog_char chrSetPeriod[] = "Период полива";
prog_char chrDaysShort[] = " д.";
prog_char chrLeft[] = "Осталось:";
prog_char chrDaysLeft[] = "Дней до полива:";
prog_char chrStartTime[] = "Полить в ";
prog_char chrSetStartTime[] = "Время полива";
prog_char chrDuration[] = "Поливать ";
prog_char chrSetDuration[] = "Длительность";
prog_char chrTime[] = ":00 ";
prog_char chrTimeBracket[] = ":00]";
prog_char chrColon[] = ":";
prog_char chrSec[] = " с";
prog_char chrSecBracket[] = " с]";

prog_char chrSpace[] = " ";
prog_char chrBracketLeft[] = "[";
prog_char chrBracketRight[] = "]";

// =============================== Types =======================================
typedef struct {
    uint8_t x, y;
    uint8_t Next, Prev;
    void (*EventMenu)(void);
    prog_char *Text;
    uint8_t *Value;
    uint8_t MaxValue;
    uint8_t MinValue;
    prog_char *TextAfterValue;
    bool PrintValue;
} Item_t;

typedef struct {
    prog_char *Title;
    void *ParentMenu;
    uint8_t CurrentItem;    // Highlighted item
    uint8_t ItemCount;
    void (*Setup)(void);
    Item_t Items[];
} Menu_t;

// ============================ Prototypes =====================================
//Common
void DrawMenu(void);

void MainMenuExit(void);

// Menu setup
void MainMenuSetup(void);
void SetTimeMenuSetup(void);
void PumpMenuSetup(void);
void OnOffMenuSetup(void);
void PeriodMenuSetup(void);
void TimeLeftMenuSetup(void);
void StartTimeMenuSetup(void);
void DurationMenuSetup (void);

// Parameters change
void EvtOnOff(void);
void TogglePeriodMode(void);

void ToggleNumberEditEnable(void);

// Common
void EvtExit(void);
prog_char* WordForm (uint8_t N, PumpDelayMode_t MUnit);

// ============================= Variables =====================================
Menu_t MainMenu = {
    Title: 0,
    ParentMenu: 0,
    CurrentItem: 0,
    Setup: &MainMenuSetup,
    ItemCount: 6,
    Items: {
        { x: 2, y: 1, Next: 1, Prev: 5, Text: (prog_char*)chrPumps[0], EventMenu: &PumpMenuSetup },
        { x: 2, y: 2, Next: 2, Prev: 0, Text: (prog_char*)chrPumps[1], EventMenu: &PumpMenuSetup },
        { x: 2, y: 3, Next: 3, Prev: 1, Text: (prog_char*)chrPumps[2], EventMenu: &PumpMenuSetup },
        { x: 2, y: 4, Next: 4, Prev: 2, Text: (prog_char*)chrPumps[3], EventMenu: &PumpMenuSetup },
        { x: 2, y: 5, Next: 5, Prev: 3, Text: (prog_char*)chrSetTime,  EventMenu: &SetTimeMenuSetup },
        { x: 2, y: 7, Next: 0, Prev: 4, Text: (prog_char*)chrBack,     EventMenu: &MainMenuExit }
    }
};

Menu_t SetTimeMenu = {
    Title: 0,
    ParentMenu: &MainMenu,
    CurrentItem: 1,
    Setup: &SetTimeMenuSetup,
    ItemCount: 4,
    Items: {
        { x: 0, y: 1, Next: 1, Prev: 1, Text: (prog_char*)chrMenuSetTime, EventMenu:0 },
        //set hours
        { x: 4, y: 4, Next: 2, Prev: 3,                             EventMenu: &ToggleNumberEditEnable, MinValue: 0, MaxValue: 23, PrintValue: true },
        //set minutes
        { x: 9, y: 4, Next: 3, Prev: 1,                             EventMenu: &ToggleNumberEditEnable, MinValue: 0, MaxValue: 59,  PrintValue: true },
        //exit
        { x: 0, y: 7, Next: 1, Prev: 2, Text: (prog_char*)chrBack,  EventMenu: &EvtExit },
    }
};

Menu_t PumpMenu = {
    Title: 0,
    ParentMenu: &MainMenu,
    Setup: &PumpMenuSetup,
    CurrentItem: 0,
    ItemCount: 6,
    Items: {
        //0
        {                      Prev: 5,                            EventMenu: &OnOffMenuSetup },
        //1
        { x: 0, y: 2,          Prev: 0,                            EventMenu: &PeriodMenuSetup, PrintValue: true}, // FIXME
        //2
        { x: 0, y: 3, Next: 3, Prev: 1,                            EventMenu: &TimeLeftMenuSetup, PrintValue: true},
        //3
        { x: 0, y: 4, Next: 4, /*Prev in PumpMenuSetup*/           EventMenu: &StartTimeMenuSetup, PrintValue: true},
        //4
        { x: 0, y: 5, Next: 5, Prev: 3,                            EventMenu: &DurationMenuSetup, PrintValue: true},
        //5
        { x: 0, y: 7, Next: 0,              Text: (prog_char*)chrBack, EventMenu: &EvtExit },
    }
};

Menu_t OnOffMenu = {
    Title: 0,
    ParentMenu: &PumpMenu,
    Setup: &OnOffMenuSetup,
    CurrentItem: 0,
    ItemCount: 3,
    Items: {
        { x: 0, y: 3, Next: 1, Prev: 2,                            EventMenu: &EvtOnOff},
        { x: 0, y: 4, Next: 2, Prev: 0,                            EventMenu: &EvtOnOff},
        { x: 2, y: 7, Next: 0, Prev: 1, Text: (prog_char*)chrBack, EventMenu: &EvtExit }
    }
};

Menu_t PeriodMenu = {
     Title: 0,
     ParentMenu: &PumpMenu,
     Setup: &PeriodMenuSetup,
     CurrentItem: 1,
     ItemCount: 4,
     Items: {
         { x: 0, y: 2, Next: 1, Prev: 1, Text: (prog_char*)chrSetPeriod },
         { x: 2, y: 4, Next: 2, Prev: 3,                              EventMenu: &ToggleNumberEditEnable, MinValue: 1, MaxValue: 11, PrintValue: true},
         { x: 5, y: 4, Next: 3, Prev: 1, /*Text in PeriodMenuSetup*/  EventMenu: &TogglePeriodMode},
//         { x: 2, y: 5, Next: 4, Prev: 2, /*Text in PeriodMenuSetup*/  EventMenu: &TogglePeriodMode},
         { x: 2, y: 7, Next: 1, Prev: 2, Text: (prog_char*)chrBack,   EventMenu: &EvtExit }
     }

};

Menu_t TimeLeftMenu = {
    Title: 0,
    ParentMenu: &PumpMenu,
    Setup: &TimeLeftMenuSetup,
    CurrentItem: 1,
    ItemCount: 3,
    Items: {
        { x: 0, y: 2, Next: 1, Prev: 2, Text: (prog_char*)chrDaysLeft                                          },
        { x: 3, y: 4, Next: 2, Prev: 2,                            EventMenu: &ToggleNumberEditEnable, MinValue: 1, MaxValue: 10, PrintValue: true },
        { x: 2, y: 7, Next: 1, Prev: 1, Text: (prog_char*)chrBack, EventMenu: &EvtExit }
    }
};

Menu_t StartTimeMenu = {
    Title: 0,
    ParentMenu: &PumpMenu,
    Setup: &StartTimeMenuSetup,
    CurrentItem: 1,
    ItemCount: 3,
    Items: {
        { x: 0, y: 2, Next: 1, Prev: 2, Text: (prog_char*)chrSetStartTime                                          },
        { x: 3, y: 4, Next: 2, Prev: 2,                            EventMenu: &ToggleNumberEditEnable, MinValue: 1, MaxValue: 23, PrintValue: true },
        { x: 2, y: 7, Next: 1, Prev: 1, Text: (prog_char*)chrBack, EventMenu: &EvtExit }
    }
};

Menu_t DurationMenu = {
    Title: 0,
    ParentMenu: &PumpMenu,
    Setup: &DurationMenuSetup,
    CurrentItem: 1,
    ItemCount: 3,
    Items: {
        { x: 0, y: 2, Next: 1, Prev: 2, Text: (prog_char*)chrSetDuration                                          },
        { x: 3, y: 4, Next: 2, Prev: 2,                            EventMenu: &ToggleNumberEditEnable, MinValue: 1, MaxValue: 250, PrintValue: true },
        { x: 2, y: 7, Next: 1, Prev: 1, Text: (prog_char*)chrBack, EventMenu: &EvtExit }
    }
};

Menu_t *CurrentMenu;
uint8_t CurrentPump;
bool NeedToExit;
bool EditEnabled = false;

enum State_t EState;
struct {
    uint16_t ExitTimer;
    bool AquaWasPressed;
} EMenu;

// ========================== Implementation ===================================
FORCE_INLINE void Task_Menu(void) {
    switch(EState) {
        case StIdle:
            return;
            break;

        case StManualAqua:
            if(!EKeys.KeyAquaPressed) { // Check if key depressed
            }
            break;

//        case StSetDuration:
//            if(EMenu.AquaWasPressed && EKeys.KeyAquaPressed) {  // Check if Aqua was pressed once and is still pressed
//                // Add second if passed
//                if(Time.SecondPassed) {
//                    Time.SecondPassed = false;
//                    if(Pumps[EMenu.Pump-1].Duration < PUMP_MAX_DURATION) Pumps[EMenu.Pump-1].Duration++;
//                    SetState(StSetDuration);
//                }
//            }
//            break;

        default:    // check if time to get out of menu
            if (NeedToExit) {
                if(DelayElapsed(&EMenu.ExitTimer, 261)) {
                    NeedToExit = false;
                    EnterIdle();
                }
            }
            else if(DelayElapsed(&EMenu.ExitTimer, MENU_EXIT_TIMEOUT)) EnterIdle();
            break;
    }
}

void EnterIdle(void) {
    LCD_Clear();
    EState = StIdle;
    CurrentMenu = &MainMenu;
    MainMenu.CurrentItem = 0;
    EditEnabled = false;
    LCD_BCKLT_OFF();
    EVENT_NewMinute();
    if(!Time.IsSetCorrectly) LCD_PrintString_P(0, PRINT_TIME_Y+2, PSTR(MSG_SET_CORRECT_TIME), false);
    CheckBattery();
    // Save settings if needed
    if(PumpsSettingsChanged) PumpsSave();
}

void DrawMenu(void) {
    LCD_Clear();
    // Print Title
    if (CurrentMenu->Title != 0) LCD_PrintString_P(0, 0, CurrentMenu->Title, false);
    // Print items
    uint8_t fcount = CurrentMenu->ItemCount;
    Item_t *itm;
    uint8_t fx;
    bool fHighlight;
    for (uint8_t i=0; i<fcount; i++) {
        itm = &CurrentMenu->Items[i];
        if(itm->Text == 0) continue;
        fHighlight = (i == CurrentMenu->CurrentItem);
        fx = LCD_PrintString_P(itm->x, itm->y, itm->Text, fHighlight);
        if(itm->PrintValue)
            fx = LCD_PrintUint(fx, itm->y, *(itm->Value), fHighlight);
        if(itm->TextAfterValue != 0)
            LCD_PrintString_P(fx, itm->y, itm->TextAfterValue, fHighlight);
    }
}


// ============================ Key Events =====================================
void EVENT_AnyKey(void) {
    Beep(BEEP_SHORT);
    DelayReset(&EMenu.ExitTimer);
    LCD_BCKLT_ON();
    if (EState == StIdle) EState = StBacklight;
}

void EVENT_KeyUp(void) {
    if (EState != StMenu) return;
    // Increase value if needed
    if (EditEnabled) {
        if (*(CurrentMenu->Items[CurrentMenu->CurrentItem].Value) < CurrentMenu->Items[CurrentMenu->CurrentItem].MaxValue)
                (*(CurrentMenu->Items[CurrentMenu->CurrentItem].Value))++;
        else
                *(CurrentMenu->Items[CurrentMenu->CurrentItem].Value) = CurrentMenu->Items[CurrentMenu->CurrentItem].MinValue;
        PumpsSettingsChanged = true;
    }
    //Main and Channel menu
    else {
        Item_t *itm = &CurrentMenu->Items[CurrentMenu->CurrentItem];
        CurrentMenu->CurrentItem = itm->Prev;
    }
    DrawMenu();
}

void EVENT_KeyDown(void) {
    if (EState != StMenu) return;
    //Decrease value if needed
    if (EditEnabled) {
        if (*(CurrentMenu->Items[CurrentMenu->CurrentItem].Value) > CurrentMenu->Items[CurrentMenu->CurrentItem].MinValue)
                (*(CurrentMenu->Items[CurrentMenu->CurrentItem].Value))--;
        else
                *(CurrentMenu->Items[CurrentMenu->CurrentItem].Value) = CurrentMenu->Items[CurrentMenu->CurrentItem].MaxValue;
        PumpsSettingsChanged = true;
    }
    //Main and Channel menu
    else {
        Item_t *itm = &CurrentMenu->Items[CurrentMenu->CurrentItem];
        CurrentMenu->CurrentItem = itm->Next;
    }
    DrawMenu();
}

void EVENT_KeyMenu(void) {
    if (EState == StBacklight) {
        EState = StMenu;
        DrawMenu();
    }
    else {
        Item_t *itm = &CurrentMenu->Items[CurrentMenu->CurrentItem];
        if (itm->EventMenu != 0) itm->EventMenu();
    }
}

// ============================== Item handlers ================================
// Exits
void MainMenuExit(void) {
    NeedToExit = true;
}
void EvtExit(void) {
    Menu_t *ParentMenu = CurrentMenu->ParentMenu;
    EditEnabled = false;
    ParentMenu->Setup();
}

// Menu setups
void MainMenuSetup(void) {
    CurrentMenu = &MainMenu;
    DrawMenu();
}

void SetTimeMenuSetup(void) {
    CurrentMenu = &SetTimeMenu;
    CurrentMenu->Items[1].Value = &Time.Hour;
    CurrentMenu->Items[1].Text = (prog_char*)&chrSpace;
    CurrentMenu->Items[1].TextAfterValue = (prog_char*)&chrSpace;
    CurrentMenu->Items[2].Value = &Time.Minute;
    CurrentMenu->Items[2].Text = (prog_char*)&chrSpace;
    CurrentMenu->Items[2].TextAfterValue = (prog_char*)&chrSpace;
    if ((CurrentMenu->CurrentItem == 1) && (EditEnabled)) {
            CurrentMenu->Items[1].Text = (prog_char*)&chrBracketLeft;
            CurrentMenu->Items[1].TextAfterValue = (prog_char*)&chrBracketRight;
    }
    if ((CurrentMenu->CurrentItem == 2) && (EditEnabled)) {
            CurrentMenu->Items[2].Text = (prog_char*)&chrBracketLeft;
            CurrentMenu->Items[2].TextAfterValue = (prog_char*)&chrBracketRight;
    }

    DrawMenu();

}

//        case StSetTimeHours:
//            if(Time.Hour == 0) Time.Hour = 23;
//            else Time.Hour--;
//            LCD_PrintTime(PRINT_TIME_X, 4, true, false, false);
//            break;
//        case StSetTimeMinTens:
//            if(Time.Minute < 10) Time.Minute += 50;
//            else Time.Minute -= 10;
//            LCD_PrintTime(PRINT_TIME_X, 4, false, true, false);
//            break;
//        case StSetTimeMinUnits:
//           if(TimeGetMinuteUnits() == 0) Time.Minute += 9;
//            else Time.Minute--;
//            LCD_PrintTime(PRINT_TIME_X, 4, false, false, true);
//            break;

void PumpMenuSetup(void) {
    if (CurrentMenu == &MainMenu) PumpMenu.CurrentItem = 0;
    CurrentMenu = &PumpMenu;
    CurrentPump = MainMenu.CurrentItem;

    CurrentMenu->Title = (prog_char*)&chrPumps[CurrentPump];
    // On/Off
    PumpMenu.Items[0].Text = (Pumps[CurrentPump].Enabled)? PSTR("включен") : PSTR("отключен");
    PumpMenu.Items[0].x    = (Pumps[CurrentPump].Enabled)? 8 : 3;
    PumpMenu.Items[0].y    = (Pumps[CurrentPump].Enabled)? 0 : 3;
    //If pump is on
    if (Pumps[CurrentPump].Enabled) {
        PumpMenu.Items[0].Next = 1;
        // Period setup
        PumpMenu.Items[1].Text = (prog_char*)&chrPeriod;
        PumpMenu.Items[1].Value = &Pumps[CurrentPump].Period;
        if (Pumps[CurrentPump].DelayMode == ModeDays) {
            PumpMenu.Items[1].TextAfterValue = WordForm(Pumps[CurrentPump].Period, ModeDays);
            PumpMenu.Items[1].Next = 2;
            PumpMenu.Items[3].Prev = 2;
            //Period Left
            PumpMenu.Items[2].Text = (prog_char*)&chrLeft;
            PumpMenu.Items[2].Value = &Pumps[CurrentPump].PeriodLeft;
            PumpMenu.Items[2].TextAfterValue = WordForm(Pumps[CurrentPump].PeriodLeft, ModeDays);
        }
        else {
            PumpMenu.Items[1].TextAfterValue = WordForm(Pumps[CurrentPump].Period, ModeHours);
            //Don't show PeriodLeft if PeriodMode is hours
            PumpMenu.Items[2].Text = 0;
            PumpMenu.Items[1].Next = 3;
            PumpMenu.Items[3].Prev = 1;
        }
        //Start time
        PumpMenu.Items[3].Text = (prog_char*)&chrStartTime;
        PumpMenu.Items[3].Value = &Pumps[CurrentPump].StartHour;
        PumpMenu.Items[3].TextAfterValue = (prog_char*)&chrTime;
        PumpMenu.Items[4].Text = (prog_char*)&chrDuration;
        PumpMenu.Items[4].Value = &Pumps[CurrentPump].Duration;
        PumpMenu.Items[4].TextAfterValue = (prog_char*)&chrSec;

        PumpMenu.Items[5].Prev = 4;
    }
    else {
        PumpMenu.Items[1].Text = 0;
        PumpMenu.Items[2].Text = 0;
        PumpMenu.Items[3].Text = 0;
        PumpMenu.Items[4].Text = 0;
        PumpMenu.Items[0].Next = 5;
        PumpMenu.Items[5].Prev = 0;
    }

    DrawMenu();
}

void OnOffMenuSetup(void) {
    CurrentMenu = &OnOffMenu;
    CurrentMenu->Title = (prog_char*)&chrPumps[CurrentPump];
    if (Pumps[CurrentPump].Enabled) {
        OnOffMenu.CurrentItem = 0;
        OnOffMenu.Items[0].Text = (prog_char*)chrOnOff[2];  // Marked ON
        OnOffMenu.Items[1].Text = (prog_char*)chrOnOff[1];  // Non-Marked OFF
    }
    else {
        OnOffMenu.CurrentItem = 1;
        OnOffMenu.Items[0].Text = (prog_char*)chrOnOff[0];  // Non-Marked ON
        OnOffMenu.Items[1].Text = (prog_char*)chrOnOff[3];  // Marked OFF
    }
    DrawMenu();
}

void PeriodMenuSetup(void) {   //FIXME
    CurrentMenu = &PeriodMenu;
    CurrentMenu->CurrentItem = 1;
    CurrentMenu->Title = (prog_char*)&chrPumps[CurrentPump];
    CurrentMenu->Items[1].Value = &Pumps[CurrentPump].Period;
    //Set a number
    if (CurrentMenu->CurrentItem == 1) {
        if (EditEnabled) {
            CurrentMenu->Items[1].Text = (prog_char*)&chrBracketLeft;
            CurrentMenu->Items[1].TextAfterValue = (prog_char*)&chrBracketRight;
        }
        else {
            CurrentMenu->Items[1].Text = (prog_char*)&chrSpace;
            CurrentMenu->Items[1].TextAfterValue = (prog_char*)&chrSpace;
        }
    }
    //Set days-hours
    if (CurrentMenu->CurrentItem == 2) {
        if (EditEnabled) {
            CurrentMenu->Items[2].Text = (prog_char*)&chrDaysHours[3];
            CurrentMenu->Items[2].TextAfterValue = (prog_char*)&chrBracketRight;
        }
        else {
            CurrentMenu->Items[2].Text = (prog_char*)&chrDaysHours[1];
            CurrentMenu->Items[2].TextAfterValue = (prog_char*)&chrSpace;
        }
    }
    else CurrentMenu->Items[2].Text = (Pumps[CurrentPump].DelayMode == ModeDays)? (prog_char*)&chrDaysHours[0] : (prog_char*)&chrDaysHours[1];
    DrawMenu();
}
//    if (Pumps[CurrentPump].DelayMode == ModeHours) {
//        CurrentMenu->Items[2].Text = (prog_char*)chrDaysHours[2]; // marked, ON
//        CurrentMenu->Items[3].Text = (prog_char*)chrDaysHours[1]; // not marked, OFF
//    }
//    else {
//        CurrentMenu->Items[2].Text = (prog_char*)chrDaysHours[0]; // not marked, OFF
//        CurrentMenu->Items[3].Text = (prog_char*)chrDaysHours[3]; // marked, ON
//    }


void TimeLeftMenuSetup(void) {
    CurrentMenu = &TimeLeftMenu;
    CurrentMenu->Title = (prog_char*)&chrPumps[CurrentPump];
    CurrentMenu->Items[1].Value = &Pumps[CurrentPump].PeriodLeft;
    if (EditEnabled == false) {
        CurrentMenu->Items[1].Text = (prog_char*)&chrSpace;
        CurrentMenu->Items[1].TextAfterValue = (prog_char*)&chrSpace;
    }
    else {
        CurrentMenu->Items[1].Text = (prog_char*)&chrBracketLeft;
        CurrentMenu->Items[1].TextAfterValue = (prog_char*)&chrBracketRight;
    }
    DrawMenu();
}
void StartTimeMenuSetup(void) {
    CurrentMenu = &StartTimeMenu;
    CurrentMenu->Title = (prog_char*)&chrPumps[CurrentPump];
    CurrentMenu->Items[1].Value = &Pumps[CurrentPump].StartHour;
    if (EditEnabled == false) {
        CurrentMenu->Items[1].Text = (prog_char*)&chrSpace;
        CurrentMenu->Items[1].TextAfterValue = (prog_char*)&chrTime;
    }
    else {
        CurrentMenu->Items[1].Text = (prog_char*)&chrBracketLeft;
        CurrentMenu->Items[1].TextAfterValue = (prog_char*)&chrTimeBracket;
    }
    DrawMenu();
}
void DurationMenuSetup (void) {
    CurrentMenu = &DurationMenu;
    CurrentMenu->Title = (prog_char*)&chrPumps[CurrentPump];
    CurrentMenu->Items[1].Value = &Pumps[CurrentPump].Duration;
    if (EditEnabled == false) {
        CurrentMenu->Items[1].Text = (prog_char*)&chrSpace;
        CurrentMenu->Items[1].TextAfterValue = (prog_char*)&chrSec;
    }
    else {
        CurrentMenu->Items[1].Text = (prog_char*)&chrBracketLeft;
        CurrentMenu->Items[1].TextAfterValue = (prog_char*)&chrSecBracket;
    }
    DrawMenu();
}


// Parameters change events
void EvtOnOff(void) {
    Pumps[CurrentPump].Enabled = !Pumps[CurrentPump].Enabled;
    PumpsSettingsChanged = true;
    CurrentMenu->Setup();
}

void TogglePeriodMode (void) {
    if (Pumps[CurrentPump].DelayMode == ModeDays) Pumps[CurrentPump].DelayMode = ModeHours;
    else Pumps[CurrentPump].DelayMode = ModeDays;
    CurrentMenu->Setup();
}

void ToggleNumberEditEnable(void) {
    EditEnabled = !EditEnabled;
    CurrentMenu->Setup();
}


// ============================= Inner use =====================================
prog_char* WordForm (uint8_t N, PumpDelayMode_t MUnit) {
    if ((N >= 10) && (N <= 20))
        return ((MUnit == ModeDays)? PSTR(" дней") : PSTR(" часов"));

    while (N >= 10) N-=10;

    if ((N == 0) || ((N > 4) && (N < 10)))
        return ((MUnit == ModeDays)? PSTR(" дней") : PSTR(" часов"));
    if (N == 1)
        return ((MUnit == ModeDays)? PSTR(" день") : PSTR(" час"));
    else
        return ((MUnit == ModeDays)? PSTR(" дня") : PSTR(" часа"));
}

//Old life

// Highlight needed option
void ShowChannelSummary() {
//    // Pump number
//    LCD_PrintString_P(0, 0, PSTR(MSG_PUMP), false);
//    LCD_PrintUint(6, 0, EMenu.Pmp->ID, false);
//    // Enabled/disabled
//    if(EMenu.Pmp->Enabled) {
//        // Enabled
//        LCD_PrintString_P(MSG_ENABLED_X, MSG_ENABLED_Y, PSTR(MSG_ENABLED), EMenu.Item == pmiOnOff);
//
//        // Delay period mode
//        LCD_PrintString_P(0, MSG_PERIOD_Y, PSTR(MSG_PERIOD), false);
//        if(EMenu.Pmp->DelayMode == ModeDays)
//            LCD_PrintString_P(MSG_PERIOD_X, MSG_PERIOD_Y, PSTR(MSG_PERIOD_DAYS), EMenu.Item == pmiPeriodTypeValue);
//        else
//            LCD_PrintString_P(MSG_PERIOD_X, MSG_PERIOD_Y, PSTR(MSG_PERIOD_HOURS), EMenu.Item == pmiPeriodTypeValue);
//
//        // Delay period value
//        LCD_PrintUint(13, MSG_PERIOD_Y, EMenu.Pmp->Period, EMenu.Item == pmiPeriodTypeValue);
//
//        // Period left
//        LCD_PrintString_P(0, MSG_PERIOD_Y+1, PSTR(MSG_PERIOD_LEFT), false);
//        LCD_PrintUint(13, MSG_PERIOD_Y+1, EMenu.Pmp->PeriodLeft, EMenu.Item == pmiPeriodLeft);
//
//        // In case of DAYS mode, display start hour
//        if(EMenu.Pmp->DelayMode == ModeDays) {
//            LCD_PrintString_P(0, MSG_START_TIME_Y, PSTR(MSG_START_TIME), false);
//            LCD_PrintUint0_99(11, MSG_START_TIME_Y, EMenu.Pmp->StartHour, EMenu.Item == pmiStartTime);
//            LCD_PrintString_P(13, MSG_START_TIME_Y, PSTR(":00"), false);
//        }
//
//        // Duration
//        LCD_PrintString_P(0, MSG_DURATION_Y, PSTR(MSG_DURATION), false);
//        LCD_PrintUint(9, MSG_DURATION_Y, EMenu.Pmp->Duration, EMenu.Item == pmiDuration);
//        LCD_PrintString_P(11, MSG_DURATION_Y, PSTR("сек."), false);
//        // Display Aqua if pressed
//        if(EKeys.KeyAquaPressed) LCD_PrintString_P(MSG_AQUA_X, MSG_DURATION_Y+1, PSTR(MSG_AQUA), false);
//    }
//    // When channel is disabled
//    else LCD_PrintString_P(MSG_ENABLED_X, MSG_ENABLED_Y, PSTR(MSG_DISABLED), EMenu.Item == pmiOnOff);
//    LCD_PrintString_P(0, MSG_EXIT_Y, PSTR(MSG_EXIT), EMenu.Item == pmiExit);
}

// ============================ States =========================================
void SetState(enum State_t AState) {
//    LCD_Clear();
//    EState = AState;
//    switch(AState) {
//        case StIdle:
//            LCD_BCKLT_OFF();
//        case StBacklight:
//            EVENT_NewMinute();  // print time
//            if(!Time.IsSetCorrectly) LCD_PrintString_P(0, PRINT_TIME_Y+2, PSTR(MSG_SET_CORRECT_TIME), false);
//            CheckBattery();
//            // Save settings if needed
//            if(PumpsSettingsChanged) PumpsSave();
//            break;
//
//        case StMainMenu:  //Show main menu
//            EMenu.Item = EMenu.Pmp->ID-1;
//            ShowMainMenu();
//            break;
//
//        case StOfferSetTime:
//            LCD_PrintString_P(0, 0, PSTR(MSG_SETUP_TIME), false);
//            LCD_PrintTime(PRINT_TIME_X, 4, false, false, false);
//            break;
//        case StShowBattery:
//            LCD_PrintString_P(0, 0, PSTR(MSG_SHOW_BATTERY), false);
//            BatteryMeasure();
//            LCD_DrawGauge();
//            LCD_GaugeValue(BatteryGaugeValue());
//            break;
//        case StManualAqua:
//            LCD_PrintString_P(0, 0, PSTR(MSG_PUMP), false);
//            //LCD_PrintUint(6, 0, EMenu.Pump, false);
//            LCD_PrintString_P(0, 4, PSTR(MSG_MANUAL_ON), false);
//            break;
//
//        // Channel setup
//        case StShowChannel: //Show all channel prefs
//            EMenu.AquaWasPressed = false;       // Reset Aqua keypress
//            EMenu.Pmp = &Pumps[(uint8_t)EMenu.Item];    // Store address of highlighted pump
//            EMenu.Item = pmiOnOff;
//            ShowChannelSummary(); // highlight enabled-disabled
//            break;
//        case StSetEnable:
//            EMenu.Item = (EMenu.Pmp->Enabled)? pmiIsOn : pmiIsOff;
//            ShowEnableScreen();
//            break;
//
////        case StScreenSetEnable: // screen for enable-disable
////            if(Pmp->Enabled)
////                ShowEnableScreen(HIGHLIGHT_SCR_ENABLED);
////            else ShowEnableScreen(HIGHLIGHT_SCR_DISABLED);
////            break;
////        case StScreenSetEnableExit:
////            ShowEnableScreen(HIGHLIGHT_EXIT);
////            break;
//
////        case StSetPeriodType:
////            ShowChannelSummary(HIGHLIGHT_PERIOD_TYPE);
////            break;
////        case StSetPeriodValue:
////            ShowChannelSummary(HIGHLIGHT_PERIOD_VALUE);
////            break;
////        case StSetPeriodLeft:
////            ShowChannelSummary(HIGHLIGHT_PERIOD_LEFT);
////            break;
////        case StSetStartTime:
////            ShowChannelSummary(HIGHLIGHT_START_TIME);
////            break;
////        case StSetDuration:
////            ShowChannelSummary(HIGHLIGHT_DURATION);
////            break;
////        case StExit:
////            ShowChannelSummary(HIGHLIGHT_EXIT);
////            break;
//
////        // Time setup
////        case StSetTimeHours:
////            LCD_PrintString_P(0, 0, PSTR(MSG_SET_TIME_HOURS), false);
////            LCD_PrintTime(PRINT_TIME_X, 4, true, false, false);
////            break;
////        case StSetTimeMinTens:
////            LCD_PrintString_P(0, 0, PSTR(MSG_SET_TIME_MINUTES), false);
////            LCD_PrintTime(PRINT_TIME_X, 4, false, true, false);
////            break;
////        case StSetTimeMinUnits:
////            LCD_PrintString_P(0, 0, PSTR(MSG_SET_TIME_MINUTES), false);
////            LCD_PrintTime(PRINT_TIME_X, 4, false, false, true);
////            break;
//
//        default: break;
//    } // switch
}


//void EVENT_KeyUp(void) {
//    switch(EState) {
//        case StMainMenu:
//            if (EMenu.Item != mmiPump1) EMenu.Item--;
//            else                        EMenu.Item = mmiExit;
//            ShowMainMenu();
//            break;
////        case StOfferSetTime:
////            SetState(StShowBattery);
////            break;
////        case StShowBattery:
////            EMenu.Pump = 1;
////            SetState(StShowChannel);
////            break;
//
//        // Channel setup
//        case StShowChannel:
//            if (EMenu.Item != pmiOnOff) EMenu.Item--;
//            else                        EMenu.Item = pmiExit;
//            ShowChannelSummary();
//            break;
////        case StSetEnable:   // Toggle enabled/disabled
////            Pumps[EMenu.Pump-1].Enabled = !Pumps[EMenu.Pump-1].Enabled;
////            PumpsSettingsChanged = true;
////            SetState(StSetEnable);
////            break;
////        case StSetPeriodType:   // Toggle period type
////            if(Pumps[EMenu.Pump-1].DelayMode == ModeDays) Pumps[EMenu.Pump-1].DelayMode = ModeHours;
////            else Pumps[EMenu.Pump-1].DelayMode = ModeDays;
////            Pumps[EMenu.Pump-1].PeriodLeft = Pumps[EMenu.Pump-1].Period;    // Set left period to period value
////            PumpsSettingsChanged = true;
////            SetState(StSetPeriodType);
////            break;
////        case StSetPeriodValue:
////            if(Pumps[EMenu.Pump-1].Period < 250) Pumps[EMenu.Pump-1].Period++;
////            Pumps[EMenu.Pump-1].PeriodLeft = Pumps[EMenu.Pump-1].Period;    // Set left period to period value
////            PumpsSettingsChanged = true;
////            SetState(StSetPeriodValue);
////            break;
////        case StSetPeriodLeft:
////            if(Pumps[EMenu.Pump-1].PeriodLeft < 250) Pumps[EMenu.Pump-1].PeriodLeft++;
////            SetState(StSetPeriodLeft);
////            break;
////        case StSetStartTime:
////            if(Pumps[EMenu.Pump-1].StartHour == 23) Pumps[EMenu.Pump-1].StartHour = 0;
////            else Pumps[EMenu.Pump-1].StartHour++;
////            PumpsSettingsChanged = true;
////            SetState(StSetStartTime);
////            break;
////        case StSetDuration:
////            if(Pumps[EMenu.Pump-1].Duration < PUMP_MAX_DURATION) Pumps[EMenu.Pump-1].Duration++;
////            PumpsSettingsChanged = true;
////            SetState(StSetDuration);
////            break;
//
//        // Time setup
//        case StSetTimeHours:
//            if(Time.Hour == 23) Time.Hour = 0;
//            else Time.Hour++;
//            LCD_PrintTime(PRINT_TIME_X, 4, true, false, false);
//            break;
//        case StSetTimeMinTens:
//            if(Time.Minute >= 50) Time.Minute -= 50;
//            else Time.Minute += 10;
//            LCD_PrintTime(PRINT_TIME_X, 4, false, true, false);
//            break;
//        case StSetTimeMinUnits:
//            if(TimeGetMinuteUnits() == 9) Time.Minute -= 9;
//            else Time.Minute++;
//            LCD_PrintTime(PRINT_TIME_X, 4, false, false, true);
//            break;
//
//        default: break;
//    } // switch
//}
//void EVENT_KeyDown(void) {
//    switch(EState) {
//        case StMainMenu:
//            if (EMenu.Item != mmiExit) EMenu.Item++;
//            else                       EMenu.Item = mmiPump1;
//            ShowMainMenu();
//            break;
//
//        case StShowBattery:
//            SetState(StOfferSetTime);
//            break;
////        case StOfferSetTime:
////            EMenu.Pump = PUMP_COUNT;
////            SetState(StShowChannel);
////            break;
//
//        // Channel setup
//        case StShowChannel:
//            if (EMenu.Pmp->Enabled){
//                if (EMenu.Item != pmiExit) EMenu.Item++;
//                else                       EMenu.Item = pmiOnOff;
//            }
//            else {
//                EMenu.Item = (EMenu.Item == pmiExit)? pmiOnOff : pmiExit;
//            }
//            ShowChannelSummary();
//            break;
//        case StSetEnable:   // Toggle enabled/disabled
//            switch (EMenu.Item) {
//                case pmiIsOn:  EMenu.Item = pmiIsOff; break;
//                case pmiIsOff: EMenu.Item = pmiExit;  break;
//                case pmiExit:  EMenu.Item = pmiIsOn;  break;
//            }
//            ShowEnableScreen();
//            break;
////            Pumps[EMenu.Pump-1].Enabled = !Pumps[EMenu.Pump-1].Enabled;
////            PumpsSettingsChanged = true;
////            SetState(StSetEnable);
////            break;
////        case StSetPeriodType:   // Toggle period type
////            if(Pumps[EMenu.Pump-1].DelayMode == ModeDays) Pumps[EMenu.Pump-1].DelayMode = ModeHours;
////            else Pumps[EMenu.Pump-1].DelayMode = ModeDays;
////            Pumps[EMenu.Pump-1].PeriodLeft = Pumps[EMenu.Pump-1].Period;    // Set left period to period value
////            PumpsSettingsChanged = true;
////            SetState(StSetPeriodType);
////            break;
////        case StSetPeriodValue:
////            if(Pumps[EMenu.Pump-1].Period > 1) Pumps[EMenu.Pump-1].Period--;
////            Pumps[EMenu.Pump-1].PeriodLeft = Pumps[EMenu.Pump-1].Period;    // Set left period to period value
////            PumpsSettingsChanged = true;
////            SetState(StSetPeriodValue);
////            break;
////        case StSetPeriodLeft:
////            if(Pumps[EMenu.Pump-1].PeriodLeft > 1) Pumps[EMenu.Pump-1].PeriodLeft--;
////            SetState(StSetPeriodLeft);
////            break;
////        case StSetStartTime:
////            if(Pumps[EMenu.Pump-1].StartHour == 0) Pumps[EMenu.Pump-1].StartHour = 23;
////            else Pumps[EMenu.Pump-1].StartHour--;
////            PumpsSettingsChanged = true;
////            SetState(StSetStartTime);
////            break;
////        case StSetDuration:
////            if(Pumps[EMenu.Pump-1].Duration > 1) Pumps[EMenu.Pump-1].Duration--;
////            PumpsSettingsChanged = true;
////            SetState(StSetDuration);
////            break;
//
////        case StShowChannel:
////            SetState(StSetEnable);
////            break;
////        case StSetEnable:
////            if(Pumps[EMenu.Pump-1].Enabled) SetState(StSetPeriodType);
////            else SetState(StShowChannel);
////            break;
//        case StSetPeriodType:
//            SetState(StSetPeriodValue);
//            break;
//        case StSetPeriodValue:
//            SetState(StSetPeriodLeft);
//            break;
////        case StSetPeriodLeft:
////            if(Pumps[EMenu.Pump-1].DelayMode == ModeDays) SetState(StSetStartTime);
////            else SetState(StSetDuration);
////            break;
//        case StSetStartTime:
//            SetState(StSetDuration);
//            break;
//        case StSetDuration:
//            SetState(StExit);
//            break;
//        case StExit:
//            SetState(StSetEnable);
//            break;
////        case StScreenSetEnable:
////            ShowEnableScreen(HIGHLIGHT_SCR_ENABLED);
//
//            // Time setup
//        case StSetTimeHours:
//            if(Time.Hour == 0) Time.Hour = 23;
//            else Time.Hour--;
//            LCD_PrintTime(PRINT_TIME_X, 4, true, false, false);
//            break;
//        case StSetTimeMinTens:
//            if(Time.Minute < 10) Time.Minute += 50;
//            else Time.Minute -= 10;
//            LCD_PrintTime(PRINT_TIME_X, 4, false, true, false);
//            break;
//        case StSetTimeMinUnits:
//            if(TimeGetMinuteUnits() == 0) Time.Minute += 9;
//            else Time.Minute--;
//            LCD_PrintTime(PRINT_TIME_X, 4, false, false, true);
//            break;
//
//        default: break;
//    } // switch
//}
//void EVENT_KeyMenu(void) {
//    switch(EState) {
//        case StIdle:
//        case StBacklight:
//            EMenu.Pmp = &Pumps[0];
//            SetState(StMainMenu);
//            break;
//        case StMainMenu:
//            switch(EMenu.Item) {
//                case mmiPump1:
//                case mmiPump2:
//                case mmiPump3:
//                case mmiPump4:
//                    SetState(StShowChannel);
//                    break;
//                case mmiSetTime:  SetState(StOfferSetTime); break;
//                case mmiExit:  SetState(StBacklight);    break;
//            }
//            break;
//        // Channel setup
//
//        case StShowChannel:
//            switch(EMenu.Item) {
//                case pmiOnOff: SetState(StSetEnable); break;
//                case pmiExit:  SetState(StMainMenu);  break;
//            }
//            break;
//        case StSetEnable:   // Go to enabled/disabled screen
//            switch (EMenu.Item) {
//                //case pmiIsOn:  EMenu.Item = pmiIsOff; break;
//                //case pmiIsOff: EMenu.Item = pmiExit;  break;
//                case pmiExit:
//                    SetState(StShowChannel);
//                    break;
//            }
//
//            break;
////        case StScreenSetEnable: //
////            Pumps[EMenu.Pump-1].Enabled = !Pumps[EMenu.Pump-1].Enabled;
////            PumpsSettingsChanged = true;
////            SetState(StScreenSetEnableExit);
////            break;
//        case StScreenSetEnableExit:
//            SetState(StShowChannel);
//            break;
//
//
//        case StSetPeriodType:
//            SetState(StSetPeriodValue);
//            break;
//        case StSetPeriodValue:
//            SetState(StSetPeriodLeft);
//            break;
////        case StSetPeriodLeft:
////            if(Pumps[EMenu.Pump-1].DelayMode == ModeDays) SetState(StSetStartTime);
////            else SetState(StSetDuration);
////            break;
//        case StSetStartTime:
//            SetState(StSetDuration);
//            break;
//        case StSetDuration:
//            SetState(StExit);
//            break;
//        case StExit:
//            SetState(StShowChannel);
//            break;
//
//        // Time setup
//        case StOfferSetTime:
//            Time.IsSetCorrectly = true; // If we has changed time, think we know what is wrong or right.
//            SetState(StSetTimeHours);
//            break;
//        case StSetTimeHours:
//            SetState(StSetTimeMinTens);
//            break;
//        case StSetTimeMinTens:
//            SetState(StSetTimeMinUnits);
//            break;
//        case StSetTimeMinUnits:
//            SetState(StOfferSetTime);
//            break;
//
//        default: break;
//    } // switch
//}
void EVENT_KeyAquaPressed(void) {
//    if(IsPumping) return;   // Not allowed if pumping yet
//    switch(EState) {
//        case StShowChannel:
//            SetState(StManualAqua);
//            PumpOn(EMenu.Pump);
//            break;
//
//        case StSetDuration:
//            if(!EMenu.AquaWasPressed) { // For the first time, reset duration value
//                Pumps[EMenu.Pump-1].Duration = 0;
//                EMenu.AquaWasPressed = true;
//            }
//            Time.SecondPassed = false;  // Reset timer
//            PumpsSettingsChanged = true;
//            PumpOn(EMenu.Pump);
//            break;
//
//        default: break;
//    } // switch
}
void EVENT_KeyAquaDepressed(void) {
//    PumpOffAll();                   // Switch water off in any mode
//    DelayReset(&EMenu.ExitTimer);   // Othewise we will exit in IDLE
//    switch(EState) {
//        case StSetDuration: SetState(StSetDuration); break;
//        case StManualAqua:  SetState(StShowChannel); break;
//        default: break;
//    }
}

