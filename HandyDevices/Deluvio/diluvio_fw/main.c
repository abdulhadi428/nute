/*
 * File:   main.c
 * Author: Kreyl Laurelindo
 *
 * Created on 29-11-2010 ã., 19:48
 */

#include "main.h"
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include "lcd110x.h"
#include "sensors.h"
#include "delay_util.h"
#include "beep.h"
#include "time.h"
#include "menu.h"
#include "battery.h"
#include "messages.h"
#include "images.h"

struct pump_t Pumps[PUMP_COUNT];
bool IsPumping, PumpsSettingsChanged;
bool MustSleep;
volatile bool WaterOk = true, BatteryOk = false;

// =============================== Implementation ==============================
int main(void) {
    GeneralInit();
    sei();
    EnterIdle();

    while(1) {
        if(POWER_OK()) {
            Task_Sensors();
            Task_Menu();
            Task_Pump();
            Task_Beep();
            Task_Sleep();
            wdt_reset();
        }
        else {  // power failure, enter sleep mode
            LCD_Shutdown();
            PumpOffAll();
            // Enter sleep
            set_sleep_mode(SLEEP_MODE_PWR_SAVE);    // Left timer2 enabled
            sleep_enable();
            // Circle here until power comes
            do {
                // Disable watchdog
                ATOMIC_BLOCK(ATOMIC_FORCEON) {
                    wdt_reset();
                    wdt_disable();
                }
                sleep_cpu();
            } while(!POWER_OK());
            // Power restored, reinit all needed
            wdt_enable(WDTO_1S);
            LCD_Init();
            EnterIdle();
        }
    } // while(1)
    return 0;
}

FORCE_INLINE void GeneralInit(void) {
    wdt_enable(WDTO_1S);
    // Disable all unneeded
    ACSR = (1<<ACD);
    DDRC  |=   (1<<PC0)|(1<<PC1);
    PORTC &= ~((1<<PC0)|(1<<PC1));
    DDRA  |=   (1<<PA2);
    PORTA &=  ~(1<<PA2);
    DDRD  |=   (1<<PD0)|(1<<PD1);
    PORTD &= ~((1<<PD0)|(1<<PD1));

    // Different modules init
    LCD_Init();
    // Show Logo
    LCD_DrawImage(0, 0, ImageLogo, false);
    for (uint8_t i=0; i<4; i++) { // Repeat to allow watchdog to reset
        _delay_ms(500);
        wdt_reset();
    }
    // Proceed with init
    QTouchInit();
    DelayInit();
    BeepInit();
    TimeInit();
    // Battery sensor init
    PWROK_DDR &= ~(1<<PWROK_P);
    BatteryMeasure();
    BatteryOk = !BATTERY_OK();
    // Water sensor
    WATER_SNS_DDR &= ~(1<<WATER_SNS_P);
    WATER_SNS_DDR |=  (1<<WATER_PWR_P);
    WATER_SNS_ON(); // Force check water after start
    _delay_us(9);
    WaterOk = !WATER_OK();
    WATER_SNS_OFF();
    // Pumps
    PUMP_DDR |= (1<<PUMP1P)|(1<<PUMP2P)|(1<<PUMP3P)|(1<<PUMP4P);
    PumpsLoad();
}

FORCE_INLINE void Task_Sleep(void) {
    if(IsPumping || (EState != StIdle) || EBeep.IsYelling) return;     // Get out if in menu or if beeper beeps
    // Prepare to sleep
    MustSleep = true;
    LCD_BCKLT_OFF();
    set_sleep_mode(SLEEP_MODE_PWR_SAVE);    // Left timer2 enabled
    sleep_enable();
    while(MustSleep) {    // Will get out of cycle either in case of pumping, or in case of keypress
        // Disable watchdog
        ATOMIC_BLOCK(ATOMIC_FORCEON) {
            wdt_reset();
            wdt_disable();
        }
        sleep_cpu();
        // Awake happened. WDT is setup in IRQ.
        // Check if key pressed
        current_time_ms_touch += 1000;  // As IRQ triggers every second
        if(QTouchActivityDetected()) MustSleep = false;
    } // While must sleep
}

// ============================== Pumps ========================================
void PumpOn(uint8_t APump) {
    IsPumping = true;
    switch (APump) {
        case 1: PUMP_PORT |= (1<<PUMP1P); break;
        case 2: PUMP_PORT |= (1<<PUMP2P); break;
        case 3: PUMP_PORT |= (1<<PUMP3P); break;
        case 4: PUMP_PORT |= (1<<PUMP4P); break;
        default:
            IsPumping = false;
            break;
    }
}
void PumpOffAll(void) {
    IsPumping = false;
    PUMP_PORT &= ~((1<<PUMP1P)|(1<<PUMP2P)|(1<<PUMP3P)|(1<<PUMP4P));
}

// Automatically switches pumps on and off. Disabled in any menu activity.
FORCE_INLINE void Task_Pump(void) {
    if(EState != StIdle) return;
    for(uint8_t i=0; i<PUMP_COUNT; i++) {
        // Beep if time to pump
        if(Pumps[i].Enabled && (Pumps[i].State == PmpMustPump) && !IsPumping) {
            Pumps[i].State = PmpBeeping;
            IsPumping = true;
            Beep(BEEP_LONG);
            DelayReset(&Pumps[i].Counter);
        }
        // Switch on needed pump
        if(Pumps[i].State == PmpBeeping) {
            if(DelayElapsed(&Pumps[i].Counter, 999)) {  // Hope beep was finished now
                Pumps[i].State = PmpPumping;
                // Reset second counter
                Time.SecondPassed = false;
                Pumps[i].Counter = 0;
                PumpOn(i+1);
            }
        }
        // Check if time to shutdown
        if(Pumps[i].State == PmpPumping) {
            if(Time.SecondPassed) {
                Time.SecondPassed = false;
                Pumps[i].Counter++;
                if(Pumps[i].Counter >= Pumps[i].Duration) {
                    PumpOffAll();
                    Pumps[i].State = PmpIdle;
                }
            } // if(Time.SecondPassed)
        } // if PmpPumping
    } // for
}

// Load/save pumps settings
void PumpsLoad(void) {
    PumpsSettingsChanged = false;
    // Check if EEPROM is empty
    if(eeprom_read_byte(0) == 0xFF) {   // EEPROM is empty, set default values
        for(uint8_t i=0; i<PUMP_COUNT; i++) {
            Pumps[i].Period = 1;
            Pumps[i].PeriodLeft = 1;
            Pumps[i].Duration = 1;
            Pumps[i].ID = i+1;
        }
        Pumps[0].Enabled = true;
        Pumps[0].Duration = 30;
        Pumps[0].Period = 1;
        Pumps[0].PeriodLeft = 1;
        Pumps[0].DelayMode = ModeHours;
        PumpsSave();
    }   // if empty
    else
        eeprom_read_block(&Pumps, 0, (PUMP_COUNT * sizeof(struct pump_t)));
}
FORCE_INLINE void PumpsSave(void) {
    PumpsSettingsChanged = false;
    eeprom_write_block(&Pumps, 0, (PUMP_COUNT * sizeof(struct pump_t)));
}

// ===================== Check water and battery ===============================
void CheckWater(void) {
    if(!POWER_OK()) return;
    WATER_SNS_ON();
    _delay_us(9);
    // Check if water state changed
    WaterOk = WATER_OK();
    WATER_SNS_OFF();
    if(WaterOk) LCD_PrintString_P(0, 0, PSTR(MSG_WATER_OK), false);
    else        LCD_PrintString_P(0, 0, PSTR(MSG_NO_WATER), false);
}
void CheckBattery(void) {
    if(!POWER_OK()) return;
    BatteryMeasure();
    // Check if battery state changed
    BatteryOk = BATTERY_OK();
    if(BatteryOk) {
        LCD_PrintString_P(4, 1, PSTR(MSG_BATTERY_OK1), false);
        LCD_PrintString_P(3, 2, PSTR(MSG_BATTERY_OK2), false);
    }
    else {
        LCD_PrintString_P(4, 1, PSTR(MSG_BATTERY_DISCHARGED1), false);
        LCD_PrintString_P(3, 2, PSTR(MSG_BATTERY_DISCHARGED2), false);
    }
}

// ============================= Events ========================================
// Time events: recalled from interrupt. Do it quick!
FORCE_INLINE void EVENT_NewSecond(void) {
    static bool Blink = false;
    if (((EState == StIdle) || (EState == StBacklight)) && POWER_OK()) {
        CheckWater();
        // Display blinking ':' to show time is passing
        LCD_GotoXYstr(PRINT_TIME_X+2, PRINT_TIME_Y);
        if(Blink) LCD_DrawChar(' ', false);
        else      LCD_DrawChar(':', false);
        Blink = !Blink;
    } // if state
}
FORCE_INLINE void EVENT_NewMinute(void) {
    if (((EState == StIdle) || (EState == StBacklight)) && POWER_OK()) {
        LCD_PrintTime(PRINT_TIME_X, PRINT_TIME_Y);
        CheckBattery();
        // Beep if needed
        if(!WaterOk || !BatteryOk || !Time.IsSetCorrectly) {
            // Beep every 15 minutes
            if (
                ((Time.MinTens == 0) && (Time.MinUnits == 1)) ||    // 01, not 00 for not ot mess with pumping
                ((Time.MinTens == 1) && (Time.MinUnits == 5)) ||    // 15
                ((Time.MinTens == 3) && (Time.MinUnits == 0)) ||    // 30
                ((Time.MinTens == 4) && (Time.MinUnits == 5))       // 45
            ) {
                Beep(BEEP_LONG);
                MustSleep = false;   // Get out of sleep cycle in Sleep_Task to beep correctly
            } // if time
        } // if not ok
    } // if state
}
FORCE_INLINE void EVENT_NewHour(void) {
    // Must count time and check pumps even if power is not ok
    if((EState != StIdle) || !Time.IsSetCorrectly) return;
    // Iterate pumps, switch on if needed
    for(uint8_t i=0; i<PUMP_COUNT; i++) if(Pumps[i].Enabled) {
        // Decrease PeriodLeft counter for hours mode
        if(Pumps[i].DelayMode == ModeHours) if(Pumps[i].PeriodLeft > 0) Pumps[i].PeriodLeft--;
        // Check if time to wake-up and pump
        if((Pumps[i].PeriodLeft == 0) && ((Pumps[i].DelayMode == ModeHours) || (Pumps[i].StartHour == Time.Hour))) {
            Pumps[i].State = PmpMustPump;
            Pumps[i].PeriodLeft = Pumps[i].Period;
            MustSleep = false;   // Get out of sleep cycle in Sleep_Task
        }
    } // for i
}
FORCE_INLINE void EVENT_NewDay(void) {
    if((EState != StIdle) || !Time.IsSetCorrectly) return;
    // Decrease PeriodLeft counter for days mode
    for(uint8_t i=0; i<PUMP_COUNT; i++) if(Pumps[i].Enabled)
        if(Pumps[i].DelayMode == ModeDays) if(Pumps[i].PeriodLeft > 0) Pumps[i].PeriodLeft--;
}
