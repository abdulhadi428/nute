#include "menu.h"
#include "main.h"
#include "lcd110x.h"
#include "beep.h"
#include "delay_util.h"
#include "sensors.h"
#include "time.h"
#include "messages.h"
#include <avr/pgmspace.h>
#include "battery.h"

// ============================= Strings =======================================
prog_char chrPumps[4][8] = {"Насос 1", "Насос 2", "Насос 3", "Насос 4"};
prog_char chrBack[] = "Назад";

// ============================ Prototypes =====================================
void EvtMainPump(void);

void EvtPumpOnOff(void);
void EvtPumpExit(void);

void EvtExit(void);

// ============================= Types =========================================
typedef struct {
    prog_char *Title;
    void *PrevMenu;
    uint8_t CurrentItem;
    uint8_t ItemCount;
    Item_t Items[];
} Menu_t;

Menu_t MainMenu = {
    Title: 0,
    PrevMenu: 0,
    CurrentItem: 0,
    ItemCount: 5,
    Items: {
        { x: 2, y: 1, tag: 0, Next: 1, Prev: 4, Text: (prog_char*)chrPumps[0], EventMenu: &EvtMainPump },
        { x: 2, y: 2, tag: 1, Next: 2, Prev: 0, Text: (prog_char*)chrPumps[1], EventMenu: &EvtMainPump },
        { x: 2, y: 3, tag: 2, Next: 3, Prev: 1, Text: (prog_char*)chrPumps[2], EventMenu: &EvtMainPump },
        { x: 2, y: 4, tag: 3, Next: 4, Prev: 2, Text: (prog_char*)chrPumps[3], EventMenu: &EvtMainPump },
        { x: 2, y: 7,         Next: 0, Prev: 3, Text: (prog_char*)chrBack }
    }
};

Menu_t PumpMenu = {
    Title: 0,
    PrevMenu: &MainMenu,
    CurrentItem: 0,
    ItemCount: 2,
    Items: {
        { x: 8, y: 0, Next: 1, Prev: 1,                            EventMenu: &EvtPumpOnOff },
        { x: 2, y: 7, Next: 0, Prev: 0, Text: (prog_char*)chrBack, EventMenu: &EvtExit }
    }
};

Menu_t OnOffMenu = {
    Title: 0,
    PrevMenu: &PumpMenu,
    CurrentItem: 0,
    ItemCount: 3,
    Items: {
        { x: 8, y: 0,  },
        { x: 2, y: 2, tag: 1 },
        { x: 2, y: 4, tag: 2 }
    }
};

Menu_t *CurrentMenu;
uint8_t CurrentPump;

void DrawMenu(void);


enum State_t EState;
struct {
    uint16_t ExitTimer;
    bool AquaWasPressed;
    uint8_t Item;
    struct pump_t *Pmp;
} EMenu;



// ========================== Implementation ===================================
void MenuInit(void) {
//
//    // ==== OnOff menu ====
//    OnOffMenu.ItemCount = 3;
//    OnOffMenu.Title = 0;
//    OnOffMenu.Items[0].x = 2;
//    OnOffMenu.Items[0].y = 3;
//    OnOffMenu.Items[0].tag = 0;
//    OnOffMenu.Items[0].Text = PSTR("Включить");
//    OnOffMenu.Items[0].Next = (void*)&OnOffMenu.Items[1];
//    OnOffMenu.Items[0].Prev = (void*)&OnOffMenu.Items[2];
//    //OnOffMenu.Items[0].EventMenu = &;
//
//    OnOffMenu.Items[1].x = 2;
//    OnOffMenu.Items[1].y = 4;
//    OnOffMenu.Items[1].tag = 1;
//    OnOffMenu.Items[1].Text = PSTR("Выключить");
//    OnOffMenu.Items[1].Next = (void*)&OnOffMenu.Items[2];
//    OnOffMenu.Items[1].Prev = (void*)&OnOffMenu.Items[0];
//    //OnOffMenu.Items[1].EventMenu = &EvtMainPump;
//
//    OnOffMenu.Items[2].x = 0;
//    OnOffMenu.Items[2].y = 7;
//    OnOffMenu.Items[2].tag = 2;
//    OnOffMenu.Items[2].Text = PSTR("Назад");
//    OnOffMenu.Items[2].Next = (void*)&OnOffMenu.Items[0];
//    OnOffMenu.Items[2].Prev = (void*)&OnOffMenu.Items[1];
//    //OnOffMenu.Items[2].EventMenu = &EvtMainPump;


}


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
            if(DelayElapsed(&EMenu.ExitTimer, MENU_EXIT_TIMEOUT)) SetState(StIdle);
            break;
    }
}

void DrawMenu(void) {
    LCD_Clear();
    // Print Title
    if (CurrentMenu->Title != 0) LCD_PrintString_P(0, 0, CurrentMenu->Title, false);
    // Print items
    uint8_t fcount = CurrentMenu->ItemCount;
    Item_t *itm;
    for (uint8_t i=0; i<fcount; i++) {
        itm = &CurrentMenu->Items[i];
        LCD_PrintString_P(itm->x, itm->y, itm->Text, (i == CurrentMenu->CurrentItem));
    }
}

// ============================ States =========================================
void SetState(enum State_t AState) {
    LCD_Clear();
    EState = AState;
    switch(AState) {
        case StIdle:
            LCD_BCKLT_OFF();
        case StBacklight:
            EVENT_NewMinute();  // print time
            if(!Time.IsSetCorrectly) LCD_PrintString_P(0, PRINT_TIME_Y+2, PSTR(MSG_SET_CORRECT_TIME), false);
            CheckBattery();
            // Save settings if needed
            if(PumpsSettingsChanged) PumpsSave();
            break;

        case StMainMenu:  //Show main menu
            EMenu.Item = EMenu.Pmp->ID-1;
            ShowMainMenu();
            break;

        case StOfferSetTime:
            LCD_PrintString_P(0, 0, PSTR(MSG_SETUP_TIME), false);
            LCD_PrintTime(PRINT_TIME_X, 4, false, false, false);
            break;
        case StShowBattery:
            LCD_PrintString_P(0, 0, PSTR(MSG_SHOW_BATTERY), false);
            BatteryMeasure();
            LCD_DrawGauge();
            LCD_GaugeValue(BatteryGaugeValue());
            break;
        case StManualAqua:
            LCD_PrintString_P(0, 0, PSTR(MSG_PUMP), false);
            //LCD_PrintUint(6, 0, EMenu.Pump, false);
            LCD_PrintString_P(0, 4, PSTR(MSG_MANUAL_ON), false);
            break;

        // Channel setup
        case StShowChannel: //Show all channel prefs
            EMenu.AquaWasPressed = false;       // Reset Aqua keypress
            EMenu.Pmp = &Pumps[(uint8_t)EMenu.Item];    // Store address of highlighted pump
            EMenu.Item = pmiOnOff;
            ShowChannelSummary(); // highlight enabled-disabled
            break;
        case StSetEnable:
            EMenu.Item = (EMenu.Pmp->Enabled)? pmiIsOn : pmiIsOff;
            ShowEnableScreen();
            break;

//        case StScreenSetEnable: // screen for enable-disable
//            if(Pmp->Enabled)
//                ShowEnableScreen(HIGHLIGHT_SCR_ENABLED);
//            else ShowEnableScreen(HIGHLIGHT_SCR_DISABLED);
//            break;
//        case StScreenSetEnableExit:
//            ShowEnableScreen(HIGHLIGHT_EXIT);
//            break;

//        case StSetPeriodType:
//            ShowChannelSummary(HIGHLIGHT_PERIOD_TYPE);
//            break;
//        case StSetPeriodValue:
//            ShowChannelSummary(HIGHLIGHT_PERIOD_VALUE);
//            break;
//        case StSetPeriodLeft:
//            ShowChannelSummary(HIGHLIGHT_PERIOD_LEFT);
//            break;
//        case StSetStartTime:
//            ShowChannelSummary(HIGHLIGHT_START_TIME);
//            break;
//        case StSetDuration:
//            ShowChannelSummary(HIGHLIGHT_DURATION);
//            break;
//        case StExit:
//            ShowChannelSummary(HIGHLIGHT_EXIT);
//            break;

        // Time setup
        case StSetTimeHours:
            LCD_PrintString_P(0, 0, PSTR(MSG_SET_TIME_HOURS), false);
            LCD_PrintTime(PRINT_TIME_X, 4, true, false, false);
            break;
        case StSetTimeMinTens:
            LCD_PrintString_P(0, 0, PSTR(MSG_SET_TIME_MINUTES), false);
            LCD_PrintTime(PRINT_TIME_X, 4, false, true, false);
            break;
        case StSetTimeMinUnits:
            LCD_PrintString_P(0, 0, PSTR(MSG_SET_TIME_MINUTES), false);
            LCD_PrintTime(PRINT_TIME_X, 4, false, false, true);
            break;

        default: break;
    } // switch
}

// ============================== Events =======================================
void EVENT_AnyKey(void) {
    Beep(BEEP_SHORT);
    DelayReset(&EMenu.ExitTimer);
    LCD_BCKLT_ON();
    if (EState == StIdle) EState = StBacklight;
}

// New life
void EVENT_KeyUp(void) {
    Item_t *itm = &CurrentMenu->Items[CurrentMenu->CurrentItem];
    CurrentMenu->CurrentItem = itm->Prev;
    DrawMenu();
}
void EVENT_KeyDown(void) {
    Item_t *itm = &CurrentMenu->Items[CurrentMenu->CurrentItem];
    CurrentMenu->CurrentItem = itm->Next;
    DrawMenu();
}
void EVENT_KeyMenu(void) {
    if (EState == StBacklight) {
        EState = StMenu;
        MainMenu.CurrentItem = 0;
        CurrentMenu = &MainMenu;
        DrawMenu();
    }
    else {
        Item_t *itm = &CurrentMenu->Items[CurrentMenu->CurrentItem];
        if (itm->EventMenu != 0) itm->EventMenu();
    }
}

// ============================== Item handlers ================================
void EvtMainPump(void) {
    CurrentPump = MainMenu.CurrentItem;
    CurrentMenu = &PumpMenu;                // Move to Pump menu
    CurrentMenu.Title = (prog_char*)&chrPumps[CurrentPump];
    CurrentMenu.CurrentItem = 0;
    PumpMenu.Items[0].Text = (Pumps[CurrentPump].Enabled)? PSTR("включен") : PSTR("отключен");
    DrawMenu();
}

void EvtPumpOnOff(void) {
    CurrentMenu = &OnOffMenu;
    CurrentMenu.Title = (prog_char*)&chrPumps[CurrentPump];
    OnOffMenu.CurrentItem = (Pumps[CurrentPump].Enabled)? 0 : 1;
    DrawMenu();
}


void EvtExit(void) {
    CurrentMenu = CurrentMenu->PrevMenu;
    DrawMenu();
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

// ============================= Inner use =====================================
// Highlight needed option
void ShowChannelSummary() {
    // Pump number
    LCD_PrintString_P(0, 0, PSTR(MSG_PUMP), false);
    LCD_PrintUint(6, 0, EMenu.Pmp->ID, false);
    // Enabled/disabled
    if(EMenu.Pmp->Enabled) {
        // Enabled
        LCD_PrintString_P(MSG_ENABLED_X, MSG_ENABLED_Y, PSTR(MSG_ENABLED), EMenu.Item == pmiOnOff);

        // Delay period mode
        LCD_PrintString_P(0, MSG_PERIOD_Y, PSTR(MSG_PERIOD), false);
        if(EMenu.Pmp->DelayMode == ModeDays)
            LCD_PrintString_P(MSG_PERIOD_X, MSG_PERIOD_Y, PSTR(MSG_PERIOD_DAYS), EMenu.Item == pmiPeriodTypeValue);
        else
            LCD_PrintString_P(MSG_PERIOD_X, MSG_PERIOD_Y, PSTR(MSG_PERIOD_HOURS), EMenu.Item == pmiPeriodTypeValue);

        // Delay period value
        LCD_PrintUint(13, MSG_PERIOD_Y, EMenu.Pmp->Period, EMenu.Item == pmiPeriodTypeValue);

        // Period left
        LCD_PrintString_P(0, MSG_PERIOD_Y+1, PSTR(MSG_PERIOD_LEFT), false);
        LCD_PrintUint(13, MSG_PERIOD_Y+1, EMenu.Pmp->PeriodLeft, EMenu.Item == pmiPeriodLeft);

        // In case of DAYS mode, display start hour
        if(EMenu.Pmp->DelayMode == ModeDays) {
            LCD_PrintString_P(0, MSG_START_TIME_Y, PSTR(MSG_START_TIME), false);
            LCD_PrintUint0_99(11, MSG_START_TIME_Y, EMenu.Pmp->StartHour, EMenu.Item == pmiStartTime);
            LCD_PrintString_P(13, MSG_START_TIME_Y, PSTR(":00"), false);
        }

        // Duration
        LCD_PrintString_P(0, MSG_DURATION_Y, PSTR(MSG_DURATION), false);
        LCD_PrintUint(9, MSG_DURATION_Y, EMenu.Pmp->Duration, EMenu.Item == pmiDuration);
        LCD_PrintString_P(11, MSG_DURATION_Y, PSTR("сек."), false);
        // Display Aqua if pressed
        if(EKeys.KeyAquaPressed) LCD_PrintString_P(MSG_AQUA_X, MSG_DURATION_Y+1, PSTR(MSG_AQUA), false);
    }
    // When channel is disabled
    else LCD_PrintString_P(MSG_ENABLED_X, MSG_ENABLED_Y, PSTR(MSG_DISABLED), EMenu.Item == pmiOnOff);
    LCD_PrintString_P(0, MSG_EXIT_Y, PSTR(MSG_EXIT), EMenu.Item == pmiExit);
}
void ShowMainMenu() {
    LCD_PrintString_P(2, 1, PSTR(MSG_PUMP), (EMenu.Item == mmiPump1));
    LCD_PrintUint    (8, 1, 1,              (EMenu.Item == mmiPump1));
    LCD_PrintString_P(2, 2, PSTR(MSG_PUMP), (EMenu.Item == mmiPump2));
    LCD_PrintUint    (8, 2, 2,              (EMenu.Item == mmiPump2));
    LCD_PrintString_P(2, 3, PSTR(MSG_PUMP), (EMenu.Item == mmiPump3));
    LCD_PrintUint    (8, 3, 3,              (EMenu.Item == mmiPump3));
    LCD_PrintString_P(2, 4, PSTR(MSG_PUMP), (EMenu.Item == mmiPump4));
    LCD_PrintUint    (8, 4, 4,              (EMenu.Item == mmiPump4));

    LCD_PrintString_P(2, 5, PSTR(MSG_MENU_SET_TIME), EMenu.Item == mmiSetTime);
    LCD_PrintString_P(2, MSG_EXIT_Y, PSTR(MSG_EXIT), EMenu.Item == mmiExit);
}

void ShowEnableScreen() {
    LCD_PrintString_P(0, 0, PSTR(MSG_PUMP), false);
    LCD_PrintUint(6, 0, EMenu.Pmp->ID, false);
    if(EMenu.Pmp->Enabled) LCD_PrintString_P(MSG_ENABLED_X, MSG_ENABLED_Y, PSTR(MSG_ENABLED), false);
    else                   LCD_PrintString_P(MSG_ENABLED_X, MSG_ENABLED_Y, PSTR(MSG_DISABLED), false);

    LCD_PrintString_P(3, 3, PSTR(MSG_TO_ENABLE),  (EMenu.Item == pmiIsOn));
    LCD_PrintString_P(3, 4, PSTR(MSG_TO_DISABLE), (EMenu.Item == pmiIsOff));
    LCD_PrintString_P(0, MSG_EXIT_Y, PSTR("save&exit"), (EMenu.Item == pmiExit));
}
