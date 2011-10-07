#include "menu.h"
#include "main.h"
#include "lcd110x.h"
#include "beep.h"
#include "delay_util.h"
#include "sensors.h"
#include "time.h"
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "battery.h"

// =============================== Types =======================================
#define FLICKER_DELAY   207 // ms
struct {
    bool IsHighlighted;
    char Text[17];
    uint8_t x, y;
    uint16_t Timer;
    bool Enabled;
} EFlicker;

typedef enum {KeyUp, KeyDown} key_t;
typedef void(*PFvoidvoid)(void);
typedef void(*PFvoidkey)(key_t);

typedef struct {
    uint8_t Next, Prev;
    PFvoidvoid EventMenu;
    PFvoidkey EventChange;
    uint8_t Tag;
} Item_t;

typedef struct {
    void *ParentMenu;
    uint8_t CurrentItem;    // Highlighted item
    uint8_t ItemCount;
    PFvoidvoid Setup;
    Item_t Items[];
} Menu_t;

// ============================ Prototypes =====================================
// Menu setup
void MainMenuSetup(void);
void MainMenuExit(void);
void SetTimeMenuSetup(void);
void PumpMenuSetup(void);
void DurationMenuSetup (void);

void ManualAquaSetup(void);

// ==== Parameters change ====
void OnOffMenu(void);
void OnOffChange(key_t AKey);
// Period
void PeriodValueMenu(void);
void PeriodValueChange(key_t AKey);
void PeriodModeMenu(void);
void PeriodModeChange(key_t AKey);
void PeriodLeftMenu(void);
void PeriodLeftChange(key_t AKey);
// Start time
void StartTimeMenu(void);
void StartTimeChange(key_t AKey);
// Duration
void DurationMenu(void);
void DurationChange(key_t AKey);

// ==== Time setup ====
void TimeHoursMenu(void);
void TimeHoursChange(key_t AKey);
void TimeMinTensMenu(void);
void TimeMinTensChange(key_t AKey);
void TimeMinUnitsMenu(void);
void TimeMinUnitsChange(key_t AKey);

// Common functions
void EvtExit(void);
prog_char* WordForm (uint8_t N, PumpPeriodMode_t MUnit);

// Flicker
void FlickerEnableValuePText(uint8_t x, uint8_t y, uint8_t AValue, prog_char* S);
void FlickerEnableText(uint8_t x, uint8_t y, prog_char* S);
void FlickerEnableValue(uint8_t x, uint8_t y, uint8_t AValue);
void FlickerDisable(void);

// ============================= Variables =====================================
Menu_t MainMenu = {
    ParentMenu: 0,
    Setup: &MainMenuSetup,
    ItemCount: 6,
    Items: {
        { Next: 1, Prev: 5, EventMenu: &PumpMenuSetup },
        { Next: 2, Prev: 0, EventMenu: &PumpMenuSetup },
        { Next: 3, Prev: 1, EventMenu: &PumpMenuSetup },
        { Next: 4, Prev: 2, EventMenu: &PumpMenuSetup },
        { Next: 5, Prev: 3, EventMenu: &SetTimeMenuSetup },
        { Next: 0, Prev: 4, EventMenu: &MainMenuExit },
    }
};

Menu_t ManualAqua; // dummy menu

Menu_t SetTimeMenu = {
    ParentMenu: &MainMenu,
    Setup: &SetTimeMenuSetup,
    ItemCount: 4,
    Items: {
        { Next: 1, Prev: 3, EventMenu: &TimeHoursMenu,    EventChange: &TimeHoursChange},    // hours
        { Next: 2, Prev: 0, EventMenu: &TimeMinTensMenu,  EventChange: &TimeMinTensChange},  // minutes tens
        { Next: 3, Prev: 1, EventMenu: &TimeMinUnitsMenu, EventChange: &TimeMinUnitsChange}, // minutes units
        { Next: 0, Prev: 2, EventMenu: &EvtExit },                                           // exit
    }
};

Menu_t PumpMenu = {
    ParentMenu: &MainMenu,
    Setup: &PumpMenuSetup,
    ItemCount: 7,
    Items: {
        { Next: 1, Prev: 6, EventMenu: &OnOffMenu,       EventChange: &OnOffChange },      // On/off
        { Next: 2, Prev: 0, EventMenu: &PeriodValueMenu, EventChange: &PeriodValueChange}, // Period value
        { Next: 3, Prev: 1, EventMenu: &PeriodModeMenu,  EventChange: &PeriodModeChange},  // Period Mode
        { Next: 4, Prev: 2, EventMenu: &PeriodLeftMenu,  EventChange: &PeriodLeftChange},  // Period left
        { Next: 5, Prev: 3, EventMenu: &StartTimeMenu,   EventChange: &StartTimeChange},   // Start time
        { Next: 6, Prev: 4, EventMenu: &DurationMenu,    EventChange: &DurationChange},    // Duration
        { Next: 0, Prev: 5, EventMenu: &EvtExit }                                          // Exit
    }
};

Menu_t *CurrentMenu;
pump_t *CurrentPump;
bool NeedToExit;
bool EditEnabled = false;

enum State_t EState;
struct {
    uint16_t ExitTimer;
    bool AquaWasPressed;
} EMenu;

// ========================== Implementation ===================================
FORCE_INLINE void Task_Menu(void) {
    // Flicker
    if(EFlicker.Enabled) {
        if (DelayElapsed(&EFlicker.Timer, FLICKER_DELAY)) {
            EFlicker.IsHighlighted = !EFlicker.IsHighlighted;
            LCD_PrintString(EFlicker.x, EFlicker.y, EFlicker.Text, EFlicker.IsHighlighted);
        }
    }
    // If setting up duration
    if((CurrentMenu == &PumpMenu) && (PumpMenu.CurrentItem == 5) && EditEnabled) {
        if(EMenu.AquaWasPressed && EKeys.KeyAquaPressed) {  // Check if Aqua was pressed once and is still pressed
            // Add second if passed
            if(Time.SecondPassed) {
                Time.SecondPassed = false;
                //if(CurrentPump->Duration < PUMP_MAX_DURATION)
                CurrentPump->Duration++;
                FlickerEnableValue(PumpMenu.Items[5].Tag, 5, CurrentPump->Duration);
                PumpMenuSetup();
            }
        }
    }

    if (EState != StIdle) {
        if (NeedToExit) {
            if(DelayElapsed(&EMenu.ExitTimer, 261)) {
                NeedToExit = false;
                EnterIdle();
            }
        }
        else if(DelayElapsed(&EMenu.ExitTimer, MENU_EXIT_TIMEOUT)) EnterIdle();
    }
}

void EnterIdle(void) {
    EState = StIdle;
    CurrentMenu = 0;
    MainMenu.CurrentItem = 0;
    EditEnabled = false;
    FlickerDisable();
    LCD_Clear();
    LCD_BCKLT_OFF();
    Time.FlowEnabled = true;    // Can be disabled during Time Setting
    EVENT_NewMinute();
    if(!Time.IsSetCorrectly) LCD_PrintString_P(0, PRINT_TIME_Y+2, PSTR("Установите время"), false);
    CheckBattery();
    // Save settings if needed
    if(PumpsSettingsChanged) PumpsSave();
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
    Item_t *itm = &CurrentMenu->Items[CurrentMenu->CurrentItem];
    if (EditEnabled) {
        if (itm->EventChange != 0) {
            itm->EventChange(KeyUp);
            PumpsSettingsChanged = true;
        }
    }
    else CurrentMenu->CurrentItem = itm->Prev;
    // Redraw menu
    CurrentMenu->Setup();
}
void EVENT_KeyDown(void) {
    if (EState != StMenu) return;
    Item_t *itm = &CurrentMenu->Items[CurrentMenu->CurrentItem];
    if (EditEnabled) {
        if (itm->EventChange != 0) {
            itm->EventChange(KeyDown);
            PumpsSettingsChanged = true;
        }
    }
    else CurrentMenu->CurrentItem = itm->Next;
    // Redraw menu
    CurrentMenu->Setup();
}
void EVENT_KeyMenu(void) {
    if (EState == StBacklight) {
        EState = StMenu;
        MainMenu.CurrentItem = 0;
        MainMenuSetup();
    }
    else {
        Item_t *itm = &CurrentMenu->Items[CurrentMenu->CurrentItem];
        if (itm->EventMenu != 0) itm->EventMenu();
    }
}

void EVENT_KeyAquaPressed(void) {
    if(IsPumping) return;   // Not allowed if pumping yet
    // If pressed in main menu
    if((CurrentMenu == &MainMenu) && (MainMenu.CurrentItem >= 0) && (MainMenu.CurrentItem <= 3)) {
        PumpOn(MainMenu.CurrentItem);
        ManualAquaSetup();
    }
    // If pressed when duration setup
    if((CurrentMenu == &PumpMenu) && (PumpMenu.CurrentItem == 5) && EditEnabled) {
        if(!EMenu.AquaWasPressed) { // For the first time, reset duration value
            EMenu.AquaWasPressed = true;
            CurrentPump->Duration = 0;
            FlickerEnableValue(PumpMenu.Items[5].Tag, 5, CurrentPump->Duration);
        }
        Time.SecondPassed = false;  // Reset timer
        PumpsSettingsChanged = true;
        PumpOn(MainMenu.CurrentItem);
        PumpMenuSetup();
    }
}
void EVENT_KeyAquaDepressed(void) {
    PumpOffAll();                   // Switch water off in any mode
    DelayReset(&EMenu.ExitTimer);   // Othewise we will exit in IDLE

    // Return to main menu if in manual mode
    if(CurrentMenu == &ManualAqua) MainMenuSetup();
    // Redraw pump menu if in pump menu
    if((CurrentMenu == &PumpMenu) && (PumpMenu.CurrentItem == 5) && EditEnabled) PumpMenuSetup();
}

// ============================== Item handlers ================================
// Exits
void MainMenuExit(void) {
    NeedToExit = true;
}
void EvtExit(void) {
    EditEnabled = false;
    CurrentMenu->CurrentItem = 0;
    ((Menu_t *)(CurrentMenu->ParentMenu))->Setup();
}

// ============================ Menu setups ====================================
void MainMenuSetup(void) {
    CurrentMenu = &MainMenu;
    EMenu.AquaWasPressed = false;
    // = Draw menu =
    LCD_Clear();
    // Items
    LCD_PrintString_P(2, 1, PSTR("Насос 1"), (CurrentMenu->CurrentItem == 0));
    LCD_PrintString_P(2, 2, PSTR("Насос 2"), (CurrentMenu->CurrentItem == 1));
    LCD_PrintString_P(2, 3, PSTR("Насос 3"), (CurrentMenu->CurrentItem == 2));
    LCD_PrintString_P(2, 4, PSTR("Насос 4"), (CurrentMenu->CurrentItem == 3));
    LCD_PrintString_P(2, 5, PSTR("Время"  ), (CurrentMenu->CurrentItem == 4));
    // Back
    LCD_PrintString_P(2, 7, PSTR("Назад"  ), (CurrentMenu->CurrentItem == 5));
}

void SetTimeMenuSetup(void) {
    uint8_t fx;
    CurrentMenu = &SetTimeMenu;
    // = Draw menu =
    LCD_Clear();
    // Title
    LCD_PrintString_P(0, 0, PSTR("Установите время"), false);
    // Items
    SetTimeMenu.Items[0].Tag = PRINT_TIME_X;
    fx = LCD_PrintUint(PRINT_TIME_X, PRINT_TIME_Y, Time.Hour, (CurrentMenu->CurrentItem == 0));
    fx = LCD_PrintString_P(fx, PRINT_TIME_Y, PSTR(":"), false);
    SetTimeMenu.Items[1].Tag = fx;
    fx = LCD_PrintUint(fx, PRINT_TIME_Y, Time.MinTens, (CurrentMenu->CurrentItem == 1));
    SetTimeMenu.Items[2].Tag = fx;
    LCD_PrintUint(fx, PRINT_TIME_Y, Time.MinUnits, (CurrentMenu->CurrentItem == 2));
    // Back
    LCD_PrintString_P(2, 7, PSTR("Назад"), (CurrentMenu->CurrentItem == 3));
}

void PumpMenuSetup(void) {
    uint8_t fx;
    if (CurrentMenu == &MainMenu) PumpMenu.CurrentItem = 0;
    CurrentMenu = &PumpMenu;
    CurrentPump = &Pumps[MainMenu.CurrentItem];
    // = Draw menu =
    LCD_Clear();
    // Title
    LCD_PrintString_P(0, 0, PSTR("Насос "), false);
    LCD_PrintUint(6, 0, (CurrentPump->ID), false);
    //If pump is on
    if (CurrentPump->Enabled) {
        // Setup next/prev item in accordance with enabled
        PumpMenu.Items[0].Next = 1;
        PumpMenu.Items[6].Prev = 5;
        // On/Off
        LCD_PrintString_P(8, 0, PSTR("включен"), (CurrentMenu->CurrentItem == 0));          // On/off, item 0
        // Period
        fx = LCD_PrintString_P(0,  2, PSTR("Период: "), false);
        PumpMenu.Items[1].Tag = fx; // store x of period text
        fx = 1+LCD_PrintUint  (fx, 2, CurrentPump->Period, (CurrentMenu->CurrentItem == 1));    // Period value, item 1
        LCD_PrintString_P     (fx, 2, WordForm(CurrentPump->Period, CurrentPump->PeriodMode), (CurrentMenu->CurrentItem == 2)); // Period mode, item 2
        PumpMenu.Items[2].Tag = fx; // store x of days/hours text
        //Period Left
        fx = LCD_PrintString_P(0,  3, PSTR("Осталось: "), false);
        PumpMenu.Items[3].Tag = fx; // store x of period left text
        fx = 1+LCD_PrintUint  (fx, 3, CurrentPump->PeriodLeft, (CurrentMenu->CurrentItem == 3));    // Period left
        LCD_PrintString_P     (fx, 3, (CurrentPump->PeriodMode == ModeDays)? PSTR("д.") : PSTR("ч."), false);

        //Don't show start time if PeriodMode is hours
        if (CurrentPump->PeriodMode == ModeDays) {
            PumpMenu.Items[3].Next = 4;
            PumpMenu.Items[5].Prev = 4;
            // Start time
            fx = LCD_PrintString_P(0,  4, PSTR("Полить в "), false);
            PumpMenu.Items[4].Tag = fx; // store x of start time text
            fx = LCD_PrintUint    (fx, 4, CurrentPump->StartHour, (CurrentMenu->CurrentItem == 4));
            LCD_PrintString_P     (fx, 4, PSTR(":00"), false);
        }
        else {  // mode Hours
            PumpMenu.Items[3].Next = 5;
            PumpMenu.Items[5].Prev = 3;
        }
        // Duration
        fx = LCD_PrintString_P(0,  5, PSTR("Поливать "), false);
        PumpMenu.Items[5].Tag = fx; // store x of duration time text
        fx = LCD_PrintUint    (fx, 5, CurrentPump->Duration, (CurrentMenu->CurrentItem == 5));
        LCD_PrintString_P     (fx, 5, PSTR(" с"), false);
        // Print "Aqua" if setting up duration with "Aqua" key
        if ((PumpMenu.CurrentItem == 5) && EditEnabled && EKeys.KeyAquaPressed) LCD_PrintString_P (12, 7, PSTR("Aqua"), false);
    }
    // If disabled
    else {
        // Setup next/prev item in accordance with enabled
        PumpMenu.Items[0].Next = 6;
        PumpMenu.Items[6].Prev = 0;
        LCD_PrintString_P(8, 0, PSTR("отключен"), (CurrentMenu->CurrentItem == 0));
    }
    // Back
    LCD_PrintString_P(2, 7, PSTR("Назад"), (CurrentMenu->CurrentItem == 6));
}

void ManualAquaSetup(void) {
    CurrentMenu = &ManualAqua;
    CurrentPump = &Pumps[MainMenu.CurrentItem];
    LCD_Clear();
    LCD_PrintString_P(2, 3, PSTR("Насос "), false);
    LCD_PrintUint(8, 3, (CurrentPump->ID), false);
    LCD_PrintString_P(0, 4, PSTR("Ручное включение"), false);
    _delay_ms(200);
    CheckBattery();
}

// ========== On/Off ==========
void OnOffMenu(void) {
    if(EditEnabled) {
        EditEnabled = false;
        FlickerDisable();
    }
    else {
        EditEnabled = true;
        if (CurrentPump->Enabled) FlickerEnableText(8, 0, PSTR("включен"));
        else                      FlickerEnableText(8, 0, PSTR("отключен"));
    }
}
void OnOffChange(key_t AKey) {
    CurrentPump->Enabled = !CurrentPump->Enabled;
    if (CurrentPump->Enabled) FlickerEnableText(8, 0, PSTR("включен"));
    else                      FlickerEnableText(8, 0, PSTR("отключен"));
    PumpMenuSetup();
}

// ========= Period ==========
void PeriodValueMenu(void) {
    if(EditEnabled) {
        EditEnabled = false;
        FlickerDisable();
    }
    else {
        EditEnabled = true;
        FlickerEnableValue(PumpMenu.Items[1].Tag, 2, CurrentPump->Period);
    }
}
void PeriodValueChange(key_t AKey) {
    if ((AKey == KeyUp)   && (CurrentPump->Period < 99)) CurrentPump->Period++;
    if ((AKey == KeyDown) && (CurrentPump->Period > 1 )) CurrentPump->Period--;
    FlickerEnableValue(PumpMenu.Items[1].Tag, 2, CurrentPump->Period);
    PumpMenuSetup();
}
void PeriodModeMenu(void) {
    if(EditEnabled) {
        EditEnabled = false;
        FlickerDisable();
    }
    else {
        EditEnabled = true;
        // Get x value from item Tag
        FlickerEnableText(PumpMenu.Items[2].Tag, 2, WordForm(CurrentPump->Period, CurrentPump->PeriodMode));
    }
}
void PeriodModeChange(key_t AKey) {
    if (CurrentPump->PeriodMode == ModeDays) CurrentPump->PeriodMode = ModeHours;
    else                                     CurrentPump->PeriodMode = ModeDays;
    FlickerEnableText(PumpMenu.Items[2].Tag, 2, WordForm(CurrentPump->Period, CurrentPump->PeriodMode));
    PumpMenuSetup();
}
void PeriodLeftMenu(void) {
    if(EditEnabled) {
        EditEnabled = false;
        FlickerDisable();
    }
    else {
        EditEnabled = true;
        FlickerEnableValue(PumpMenu.Items[3].Tag, 3, CurrentPump->PeriodLeft);
    }
}
void PeriodLeftChange(key_t AKey) {
    if ((AKey == KeyUp)   && (CurrentPump->PeriodLeft < 99)) CurrentPump->PeriodLeft++;
    if ((AKey == KeyDown) && (CurrentPump->PeriodLeft >= 1)) CurrentPump->PeriodLeft--;
    FlickerEnableValue(PumpMenu.Items[3].Tag, 3, CurrentPump->PeriodLeft);
    PumpMenuSetup();
}

// ======= Start time =======
void StartTimeMenu(void) {
    if(EditEnabled) {
        EditEnabled = false;
        FlickerDisable();
    }
    else {
        EditEnabled = true;
        FlickerEnableValue(PumpMenu.Items[4].Tag, 4, CurrentPump->StartHour);
    }
}
void StartTimeChange(key_t AKey) {
    if (AKey == KeyUp) {
        if (CurrentPump->StartHour == 23) CurrentPump->StartHour = 0;
        else CurrentPump->StartHour++;
    }
    if (AKey == KeyDown) {
        if (CurrentPump->StartHour == 0) CurrentPump->StartHour = 23;
        else CurrentPump->StartHour--;
    }
    FlickerEnableValue(PumpMenu.Items[4].Tag, 4, CurrentPump->StartHour);
    PumpMenuSetup();
}

// ======= Duration =======
void DurationMenu(void) {
    if(EditEnabled) {
        EditEnabled = false;
        FlickerDisable();
    }
    else {
        EditEnabled = true;
        FlickerEnableValue(PumpMenu.Items[5].Tag, 5, CurrentPump->Duration);
    }
}
void DurationChange(key_t AKey) {
    if ((AKey == KeyUp)   && (CurrentPump->Duration < PUMP_MAX_DURATION)) CurrentPump->Duration++;
    if ((AKey == KeyDown) && (CurrentPump->Duration > 1)) CurrentPump->Duration--;
    FlickerEnableValue(PumpMenu.Items[5].Tag, 5, CurrentPump->Duration);
    PumpMenuSetup();
}

// ==== Time ====
void TimeHoursMenu(void) {
    if(EditEnabled) {
        EditEnabled = false;
        FlickerDisable();
        Time.FlowEnabled = true;
    }
    else {
        EditEnabled = true;
        Time.IsSetCorrectly = true;
        Time.FlowEnabled = false;
        FlickerEnableValue(SetTimeMenu.Items[0].Tag, PRINT_TIME_Y, Time.Hour);
    }
}
void TimeHoursChange(key_t AKey) {
    if (AKey == KeyUp) {
        if (Time.Hour == 23) Time.Hour = 0;
        else Time.Hour++;
    }
    if (AKey == KeyDown) {
        if (Time.Hour == 0) Time.Hour = 23;
        else Time.Hour--;
    }
    FlickerEnableValue(SetTimeMenu.Items[0].Tag, PRINT_TIME_Y, Time.Hour);
    SetTimeMenuSetup();
}
void TimeMinTensMenu(void) {
    if(EditEnabled) {
        EditEnabled = false;
        FlickerDisable();
        Time.FlowEnabled = true;
    }
    else {
        EditEnabled = true;
        Time.IsSetCorrectly = true;
        Time.FlowEnabled = false;
        FlickerEnableValue(SetTimeMenu.Items[1].Tag, PRINT_TIME_Y, Time.MinTens);
    }
}
void TimeMinTensChange(key_t AKey) {
    if (AKey == KeyUp) {
        if (Time.MinTens == 5) Time.MinTens = 0;
        else Time.MinTens++;
    }
    if (AKey == KeyDown) {
        if (Time.MinTens == 0) Time.MinTens = 5;
        else Time.MinTens--;
    }
    FlickerEnableValue(SetTimeMenu.Items[1].Tag, PRINT_TIME_Y, Time.MinTens);
    SetTimeMenuSetup();
}
void TimeMinUnitsMenu(void) {
    if(EditEnabled) {
        EditEnabled = false;
        FlickerDisable();
        Time.FlowEnabled = true;
    }
    else {
        EditEnabled = true;
        Time.IsSetCorrectly = true;
        Time.FlowEnabled = false;
        FlickerEnableValue(SetTimeMenu.Items[2].Tag, PRINT_TIME_Y, Time.MinUnits);
    }
}
void TimeMinUnitsChange(key_t AKey) {
    if (AKey == KeyUp) {
        if (Time.MinUnits == 9) Time.MinUnits = 0;
        else Time.MinUnits++;
    }
    if (AKey == KeyDown) {
        if (Time.MinUnits == 0) Time.MinUnits = 9;
        else Time.MinUnits--;
    }
    FlickerEnableValue(SetTimeMenu.Items[2].Tag, PRINT_TIME_Y, Time.MinUnits);
    SetTimeMenuSetup();
}


// ============================= Inner use =====================================
prog_char* WordForm (uint8_t N, PumpPeriodMode_t MUnit) {
    if ((N >= 10) && (N <= 20))            return ((MUnit == ModeDays)? PSTR("дней") : PSTR("часов"));
    while (N >= 10) N-=10;
    if ((N == 0) || ((N > 4) && (N < 10))) return ((MUnit == ModeDays)? PSTR("дней") : PSTR("часов"));
    if (N == 1)                            return ((MUnit == ModeDays)? PSTR("день") : PSTR("час"));
    else                                   return ((MUnit == ModeDays)? PSTR("дня")  : PSTR("часа"));
}

// ================================ Flicker ====================================
uint8_t UintToChr(uint16_t AValue, char *S) {
    uint8_t digit = '0', fx=0;
    bool ShouldPrint = false;
    const uint16_t FArr[4] = {10000, 1000, 100, 10};
    // Iterate until ANumber > 10
    for(uint8_t i=0; i<4; i++) {
        while (AValue >= FArr[i]) {
            digit++;
            AValue -= FArr[i];
        }
        if((digit != '0') || ShouldPrint) {
            S[fx++] = digit;
            ShouldPrint = true;
        }
        digit = '0';
    }
    // Print last digit
    S[fx++] = '0' + AValue;
    return fx;
}

void FlickerEnableValuePText(uint8_t x, uint8_t y, uint8_t AValue, prog_char* S) {
    // Print value
    uint8_t fx = UintToChr(AValue, EFlicker.Text);
    // Print text after value
    if (S != 0) {
        char FChar;
        while ((FChar = pgm_read_byte(S++)) != '\0') {
            EFlicker.Text[fx++] = FChar;
        }
    }
    EFlicker.Text[fx] = '\0';   // Terminating NULL
    // Copy other values
    EFlicker.x = x;
    EFlicker.y = y;
    EFlicker.IsHighlighted = true;
    LCD_PrintString(x, y, EFlicker.Text, true);
    DelayReset(&EFlicker.Timer);
    EFlicker.Enabled = true;
}
void FlickerEnableText(uint8_t x, uint8_t y, prog_char* S) {
    uint8_t fx=0;
    if (S != 0) {
        char FChar;
        while ((FChar = pgm_read_byte(S++)) != '\0') {
            EFlicker.Text[fx++] = FChar;
        }
    }
    EFlicker.Text[fx] = '\0';   // Terminating NULL
    EFlicker.x = x;
    EFlicker.y = y;
    EFlicker.IsHighlighted = true;
    LCD_PrintString(x, y, EFlicker.Text, true);
    DelayReset(&EFlicker.Timer);
    EFlicker.Enabled = true;
}
void FlickerEnableValue(uint8_t x, uint8_t y, uint8_t AValue) {
    // Print value
    uint8_t fx = UintToChr(AValue, EFlicker.Text);
    EFlicker.Text[fx] = '\0';   // Terminating NULL
    // Copy other values
    EFlicker.x = x;
    EFlicker.y = y;
    EFlicker.IsHighlighted = true;
    LCD_PrintString(x, y, EFlicker.Text, true);
    DelayReset(&EFlicker.Timer);
    EFlicker.Enabled = true;
}
void FlickerDisable(void) {
    EFlicker.Enabled = false;
    LCD_PrintString(EFlicker.x, EFlicker.y, EFlicker.Text, true);
}
