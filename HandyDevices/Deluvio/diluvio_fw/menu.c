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

// ============================= Global variables ==============================
enum State_t EState;
struct {
    uint8_t Pump;
    uint16_t ExitTimer;
    bool AquaWasPressed;
    uint8_t StringNumber;
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

        case StSetDuration:
            if(EMenu.AquaWasPressed && EKeys.KeyAquaPressed) {  // Check if Aqua was pressed once and is still pressed
                // Add second if passed
                if(Time.SecondPassed) {
                    Time.SecondPassed = false;
                    if(Pumps[EMenu.Pump-1].Duration < PUMP_MAX_DURATION) Pumps[EMenu.Pump-1].Duration++;
                    SetState(StSetDuration);
                }
            }
            break;

        default:    // check if time to get out of menu
            if(DelayElapsed(&EMenu.ExitTimer, MENU_EXIT_TIMEOUT)) SetState(StIdle);
            break;
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
            EMenu.Pump = 1;     // Reset current channel
            EVENT_NewMinute();  // print time
            if(!Time.IsSetCorrectly) LCD_PrintString_P(0, PRINT_TIME_Y+2, PSTR(MSG_SET_CORRECT_TIME), false);
            CheckBattery();
            // Save settings if needed
            if(PumpsSettingsChanged) PumpsSave();
            break;


        case StMainMenu:
            EMenu.StringNumber = 1;
            ShowMainMenu();
            break;

        case StShowChannel:
            EMenu.AquaWasPressed = false;       // Reset Aqua keypress
            ShowChannelSummary(HIGHLIGHT_NONE); // do not highlight anything
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
            LCD_PrintUint(6, 0, EMenu.Pump, false);
            LCD_PrintString_P(0, 4, PSTR(MSG_MANUAL_ON), false);
            break;

        // Channel setup
        case StSetEnable:
            ShowChannelSummary(HIGHLIGHT_ENABLED);
            break;
        case StSetPeriodType:
            ShowChannelSummary(HIGHLIGHT_PERIOD_TYPE);
            break;
        case StSetPeriodValue:
            ShowChannelSummary(HIGHLIGHT_PERIOD_VALUE);
            break;
        case StSetPeriodLeft:
            ShowChannelSummary(HIGHLIGHT_PERIOD_LEFT);
            break;
        case StSetStartTime:
            ShowChannelSummary(HIGHLIGHT_START_TIME);
            break;
        case StSetDuration:
            ShowChannelSummary(HIGHLIGHT_DURATION);
            break;
        case StExit:
            ShowChannelSummary(HIGHLIGHT_EXIT);
            break;

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

void EVENT_KeyUp(void) {
    switch(EState) {
        case StMainMenu:
            if (EMenu.StringNumber > 1) {
                EMenu.StringNumber--;
                ShowMainMenu();
            }
            else {
                EMenu.StringNumber = 6;
                ShowMainMenu();
            }
            break;

        case StShowChannel:
            if(EMenu.Pump == PUMP_COUNT) SetState(StOfferSetTime);
            else {
                EMenu.Pump++;
                SetState(StShowChannel);
            }
            break;
        case StOfferSetTime:
            SetState(StShowBattery);
            break;
        case StShowBattery:
            EMenu.Pump = 1;
            SetState(StShowChannel);
            break;

        // Channel setup
        case StSetEnable:   // Toggle enabled/disabled
            Pumps[EMenu.Pump-1].Enabled = !Pumps[EMenu.Pump-1].Enabled;
            PumpsSettingsChanged = true;
            SetState(StSetEnable);
            break;
        case StSetPeriodType:   // Toggle period type
            if(Pumps[EMenu.Pump-1].DelayMode == ModeDays) Pumps[EMenu.Pump-1].DelayMode = ModeHours;
            else Pumps[EMenu.Pump-1].DelayMode = ModeDays;
            Pumps[EMenu.Pump-1].PeriodLeft = Pumps[EMenu.Pump-1].Period;    // Set left period to period value
            PumpsSettingsChanged = true;
            SetState(StSetPeriodType);
            break;
        case StSetPeriodValue:
            if(Pumps[EMenu.Pump-1].Period < 250) Pumps[EMenu.Pump-1].Period++;
            Pumps[EMenu.Pump-1].PeriodLeft = Pumps[EMenu.Pump-1].Period;    // Set left period to period value
            PumpsSettingsChanged = true;
            SetState(StSetPeriodValue);
            break;
        case StSetPeriodLeft:
            if(Pumps[EMenu.Pump-1].PeriodLeft < 250) Pumps[EMenu.Pump-1].PeriodLeft++;
            SetState(StSetPeriodLeft);
            break;
        case StSetStartTime:
            if(Pumps[EMenu.Pump-1].StartHour == 23) Pumps[EMenu.Pump-1].StartHour = 0;
            else Pumps[EMenu.Pump-1].StartHour++;
            PumpsSettingsChanged = true;
            SetState(StSetStartTime);
            break;
        case StSetDuration:
            if(Pumps[EMenu.Pump-1].Duration < PUMP_MAX_DURATION) Pumps[EMenu.Pump-1].Duration++;
            PumpsSettingsChanged = true;
            SetState(StSetDuration);
            break;

        // Time setup
        case StSetTimeHours:
            if(Time.Hour == 23) Time.Hour = 0;
            else Time.Hour++;
            LCD_PrintTime(PRINT_TIME_X, 4, true, false, false);
            break;
        case StSetTimeMinTens:
            if(Time.Minute >= 50) Time.Minute -= 50;
            else Time.Minute += 10;
            LCD_PrintTime(PRINT_TIME_X, 4, false, true, false);
            break;
        case StSetTimeMinUnits:
            if(TimeGetMinuteUnits() == 9) Time.Minute -= 9;
            else Time.Minute++;
            LCD_PrintTime(PRINT_TIME_X, 4, false, false, true);
            break;

        default: break;
    } // switch
}
void EVENT_KeyDown(void) {
    switch(EState) {
        case StMainMenu:
            if (EMenu.StringNumber < 6) {
                EMenu.StringNumber++;
                ShowMainMenu();
            }
            else {
                EMenu.StringNumber = 1;
                ShowMainMenu();
            }
            break;
        case StShowChannel:
            if(EMenu.Pump == 1) SetState(StShowBattery);
            else {
                EMenu.Pump--;
                SetState(StShowChannel);
            }
            break;
        case StShowBattery:
            SetState(StOfferSetTime);
            break;
        case StOfferSetTime:
            EMenu.Pump = PUMP_COUNT;
            SetState(StShowChannel);
            break;

        // Channel setup
//       case StSetEnable:   // Toggle enabled/disabled
//            Pumps[EMenu.Pump-1].Enabled = !Pumps[EMenu.Pump-1].Enabled;
//            PumpsSettingsChanged = true;
//            SetState(StSetEnable);
//            break;
//        case StSetPeriodType:   // Toggle period type
//            if(Pumps[EMenu.Pump-1].DelayMode == ModeDays) Pumps[EMenu.Pump-1].DelayMode = ModeHours;
//            else Pumps[EMenu.Pump-1].DelayMode = ModeDays;
//            Pumps[EMenu.Pump-1].PeriodLeft = Pumps[EMenu.Pump-1].Period;    // Set left period to period value
//            PumpsSettingsChanged = true;
//            SetState(StSetPeriodType);
//            break;
//        case StSetPeriodValue:
//            if(Pumps[EMenu.Pump-1].Period > 1) Pumps[EMenu.Pump-1].Period--;
//            Pumps[EMenu.Pump-1].PeriodLeft = Pumps[EMenu.Pump-1].Period;    // Set left period to period value
//            PumpsSettingsChanged = true;
//            SetState(StSetPeriodValue);
//            break;
//        case StSetPeriodLeft:
//            if(Pumps[EMenu.Pump-1].PeriodLeft > 1) Pumps[EMenu.Pump-1].PeriodLeft--;
//            SetState(StSetPeriodLeft);
//            break;
//        case StSetStartTime:
//            if(Pumps[EMenu.Pump-1].StartHour == 0) Pumps[EMenu.Pump-1].StartHour = 23;
//            else Pumps[EMenu.Pump-1].StartHour--;
//            PumpsSettingsChanged = true;
//            SetState(StSetStartTime);
//            break;
//        case StSetDuration:
//            if(Pumps[EMenu.Pump-1].Duration > 1) Pumps[EMenu.Pump-1].Duration--;
//            PumpsSettingsChanged = true;
//            SetState(StSetDuration);
//            break;

//        case StShowChannel:
//            SetState(StSetEnable);
//            break;
        case StSetEnable:
            if(Pumps[EMenu.Pump-1].Enabled) SetState(StSetPeriodType);
            else SetState(StShowChannel);
            break;
        case StSetPeriodType:
            SetState(StSetPeriodValue);
            break;
        case StSetPeriodValue:
            SetState(StSetPeriodLeft);
            break;
        case StSetPeriodLeft:
            if(Pumps[EMenu.Pump-1].DelayMode == ModeDays) SetState(StSetStartTime);
            else SetState(StSetDuration);
            break;
        case StSetStartTime:
            SetState(StSetDuration);
            break;
        case StSetDuration:
            SetState(StExit);
            break;
        case StExit:
            SetState(StShowChannel);
            break;


            // Time setup
        case StSetTimeHours:
            if(Time.Hour == 0) Time.Hour = 23;
            else Time.Hour--;
            LCD_PrintTime(PRINT_TIME_X, 4, true, false, false);
            break;
        case StSetTimeMinTens:
            if(Time.Minute < 10) Time.Minute += 50;
            else Time.Minute -= 10;
            LCD_PrintTime(PRINT_TIME_X, 4, false, true, false);
            break;
        case StSetTimeMinUnits:
            if(TimeGetMinuteUnits() == 0) Time.Minute += 9;
            else Time.Minute--;
            LCD_PrintTime(PRINT_TIME_X, 4, false, false, true);
            break;

        default: break;
    } // switch
}
void EVENT_KeyMenu(void) {
    switch(EState) {
        case StIdle:
        case StBacklight:
            // SetState(StShowChannel);
            SetState(StMainMenu);
            break;
        case StMainMenu:
            switch(EMenu.StringNumber) {
                case 5:  SetState(StOfferSetTime); break;
                case 6:  SetState(StBacklight);    break;
                default: SetState(StShowChannel);  break;
            }
            break;
        // Channel setup
        case StShowChannel:
            SetState(StSetEnable);
            break;
        case StSetEnable:
            if(Pumps[EMenu.Pump-1].Enabled) SetState(StSetPeriodType);
            else SetState(StShowChannel);
            break;
        case StSetPeriodType:
            SetState(StSetPeriodValue);
            break;
        case StSetPeriodValue:
            SetState(StSetPeriodLeft);
            break;
        case StSetPeriodLeft:
            if(Pumps[EMenu.Pump-1].DelayMode == ModeDays) SetState(StSetStartTime);
            else SetState(StSetDuration);
            break;
        case StSetStartTime:
            SetState(StSetDuration);
            break;
        case StSetDuration:
            SetState(StExit);
            break;
        case StExit:
            SetState(StShowChannel);
            break;

        // Time setup
        case StOfferSetTime:
            Time.IsSetCorrectly = true; // If we has changed time, think we know what is wrong or right.
            SetState(StSetTimeHours);
            break;
        case StSetTimeHours:
            SetState(StSetTimeMinTens);
            break;
        case StSetTimeMinTens:
            SetState(StSetTimeMinUnits);
            break;
        case StSetTimeMinUnits:
            SetState(StOfferSetTime);
            break;

        default: break;
    } // switch
}

void EVENT_KeyAquaPressed(void) {
    if(IsPumping) return;   // Not allowed if pumping yet
    switch(EState) {
        case StShowChannel:
            SetState(StManualAqua);
            PumpOn(EMenu.Pump);
            break;

        case StSetDuration:
            if(!EMenu.AquaWasPressed) { // For the first time, reset duration value
                Pumps[EMenu.Pump-1].Duration = 0;
                EMenu.AquaWasPressed = true;
            }
            Time.SecondPassed = false;  // Reset timer
            PumpsSettingsChanged = true;
            PumpOn(EMenu.Pump);
            break;

        default: break;
    } // switch
}
void EVENT_KeyAquaDepressed(void) {
    PumpOffAll();                   // Switch water off in any mode
    DelayReset(&EMenu.ExitTimer);   // Othewise we will exit in IDLE
    switch(EState) {
        case StSetDuration: SetState(StSetDuration); break;
        case StManualAqua:  SetState(StShowChannel); break;
        default: break;
    }
}

// ============================= Inner use =====================================
// Highlight needed option
void ShowChannelSummary(uint8_t Highlight) {
    EMenu.Pump = EMenu.StringNumber;
    struct pump_t *Pmp = &Pumps[EMenu.Pump-1];
    // Pump number
    LCD_PrintString_P(0, 0, PSTR(MSG_PUMP), (Highlight == HIGHLIGHT_NONE));
    LCD_PrintUint(6, 0, EMenu.Pump, (Highlight == HIGHLIGHT_NONE));
    // Enabled/disabled
    if(Pmp->Enabled) {
        // Enabled
        LCD_PrintString_P(MSG_ENABLED_X, MSG_ENABLED_Y, PSTR(MSG_ENABLED), (Highlight == HIGHLIGHT_ENABLED));

        // Delay period mode
        LCD_PrintString_P(0, MSG_PERIOD_Y, PSTR(MSG_PERIOD), false);
        if(Pmp->DelayMode == ModeDays)
            LCD_PrintString_P(MSG_PERIOD_X, MSG_PERIOD_Y, PSTR(MSG_PERIOD_DAYS), (Highlight == HIGHLIGHT_PERIOD_TYPE));
        else
            LCD_PrintString_P(MSG_PERIOD_X, MSG_PERIOD_Y, PSTR(MSG_PERIOD_HOURS), (Highlight == HIGHLIGHT_PERIOD_TYPE));

        // Delay period value
        LCD_PrintUint(13, MSG_PERIOD_Y, Pmp->Period, (Highlight == HIGHLIGHT_PERIOD_VALUE));

        // Period left
        LCD_PrintString_P(0, MSG_PERIOD_Y+1, PSTR(MSG_PERIOD_LEFT), false);
        LCD_PrintUint(13, MSG_PERIOD_Y+1, Pmp->PeriodLeft, (Highlight == HIGHLIGHT_PERIOD_LEFT));

        // In case of DAYS mode, display start hour
        if(Pmp->DelayMode == ModeDays) {
            LCD_PrintString_P(0, MSG_START_TIME_Y, PSTR(MSG_START_TIME), false);
            LCD_PrintUint0_99(11, MSG_START_TIME_Y, Pmp->StartHour, (Highlight == HIGHLIGHT_START_TIME));
            LCD_PrintString_P(13, MSG_START_TIME_Y, PSTR(":00"), false);
        }

        // Duration
        LCD_PrintString_P(0, MSG_DURATION_Y, PSTR(MSG_DURATION), false);
        LCD_PrintUint(9, MSG_DURATION_Y, Pmp->Duration, (Highlight == HIGHLIGHT_DURATION));
        LCD_PrintString_P(11, MSG_DURATION_Y, PSTR("сек."), false);
        // Display Aqua if pressed
        if(EKeys.KeyAquaPressed) LCD_PrintString_P(MSG_AQUA_X, MSG_DURATION_Y+1, PSTR(MSG_AQUA), false);
    }
    else LCD_PrintString_P(MSG_ENABLED_X, MSG_ENABLED_Y, PSTR(MSG_DISABLED), (Highlight == HIGHLIGHT_ENABLED));
    LCD_PrintString_P(0, MSG_EXIT_Y, PSTR(MSG_EXIT), (Highlight == HIGHLIGHT_EXIT));
}
void ShowMainMenu(){
    for (uint8_t i=0; i<4; i++) {
        LCD_PrintString_P(2, 1+i, PSTR(MSG_PUMP), ((1+i) == EMenu.StringNumber));
        LCD_PrintUint(8, 1+i, 1+i, ((1+i) == EMenu.StringNumber));
    }
    LCD_PrintString_P(2, 5, PSTR(MSG_MENU_SET_TIME), EMenu.StringNumber == 5);
    LCD_PrintString_P(2, MSG_EXIT_Y, PSTR(MSG_EXIT), EMenu.StringNumber == 6);

}
