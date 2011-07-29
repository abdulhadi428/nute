/*
 * File:   calma3c.c
 * Author: Laurelindo
 *
 * Created on 20 2009 Рі., 0:16
 */
#include <avr/io.h>
#include <inttypes.h>
#include <stdbool.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include "olwen.h"
#include "time_utils.h"
#include "../../../cc_common/common.h"
#include "../../../cc_common/cc2500.h"
#include "color_table.h"

#include <util/delay.h>
// ============================= Types =========================================
struct {
    bool CoverIsOpen,CristalIsExtracted;//, DownIsOn, UfIsOn, BothIsOn;
    uint16_t Timer, HoldTimer;
    uint16_t PollTime;
    uint8_t Step;
} ESens;

struct Color_t {
    uint8_t Red, Green, Blue, Uf;
};
struct {
    uint16_t Timer;
    struct Color_t CristalColor,DesiredColor, CurrentColor, OldColor, SavedColor, SavedUfColor, SleepColor;
    uint8_t Indx;
    bool UfIsOn;
    bool ShdnIsOn;
} ELight;

bool MustSleep; // toggles COVER events

// ============================== General ======================================
int main(void) {
    GeneralInit();

    sei();
    while (1) {
        wdt_reset();    // Reset watchdog
        CC_Task();
        SENS_Task ();
        Light_Task ();
        Sleep_Task ();
    } // while
}

FORCE_INLINE void GeneralInit(void) {
    wdt_enable(WDTO_2S);
	
    ACSR = 1<<ACD;  // Disable analog comparator
    // Shutdown all unneeded
    PRR = (1<<PRTWI)|(1<<PRSPI)|(1<<PRADC);

    MustSleep = false;
    // Setup timer
    TimerInit();

    // Light
    LED_DDR  |= (1<<RED_P)|(1<<GREEN_P)|(1<<BLUE_P);
    LED_PORT &= ~((1<<RED_P)|(1<<GREEN_P)|(1<<BLUE_P));
    LED_UF_DDR |=(1<<UF_P);
    LED_UF_PORT &=~(1<<UF_P);
    // Led power

    TCCR0A = (1<<WGM01)|(1<<WGM00);
    TCCR0B = (0<<WGM02)|(0<<CS02)|(0<<CS01)|(1<<CS00);
    TCCR2A = (1<<WGM21)|(1<<WGM20);
    TCCR2B = (0<<WGM22)|(0<<CS22)|(0<<CS21)|(1<<CS20);
    TCCR1A = (1<<WGM11)|(1<<WGM10);
    TCCR1B = (0<<WGM12)|(0<<CS12)|(0<<CS11)|(1<<CS10);
    TimerResetDelay(&ELight.Timer);
    ELight.Indx =1;
   // SetTableColor();
    SetDesiredColor(0, 0x90,0 ,0);   // Initial fade
    ELight.UfIsOn = false;
	ESens.CoverIsOpen=true;

    // Sensors
    SENS_DDR  &= ~((1<<SENS_DOWN)|(1<<SENS_UP)|(1<<SENS_UF_SWITCH));
    SENS_PORT &= ~((1<<SENS_DOWN)|(1<<SENS_UP)|(1<<SENS_UF_SWITCH)); // No pull-ups

	COVER_DDR  &= ~(1<<COVER_P);
	//COVER_PORT &= ~(1<<COVER_P);// No pull-up
	COVER_PORT|= (1<<COVER_P); // Yes pull-up
	
	CRISTAL_DDR  &= ~(1<<CRISTAL_P);
	CRISTAL_PORT|= (1<<CRISTAL_P); // Yes pull-up
	
	ESens.CristalIsExtracted=false;
    ESens.PollTime   = SENS_POLL_TIME;
    ESens.Step=SENS_STATE_START;
    TimerResetDelay (&ESens.Timer);

    // CC init
    CC_Init();
    CC_SetAddress(4);   // Never changes in CC itself
}

FORCE_INLINE void SetDesiredColor (uint8_t ARed, uint8_t AGreen, uint8_t ABlue, uint8_t AUf) {
    ELight.DesiredColor.Red   = ARed;
    ELight.DesiredColor.Green = AGreen;
    ELight.DesiredColor.Blue  = ABlue;
    ELight.DesiredColor.Uf  = AUf;
}
FORCE_INLINE void SetCurrentColor (uint8_t ARed, uint8_t AGreen, uint8_t ABlue, uint8_t AUf) {
    ELight.CurrentColor.Red   = ARed;
    ELight.CurrentColor.Green = AGreen;
    ELight.CurrentColor.Blue  = ABlue;
    ELight.CurrentColor.Uf  = AUf;
}
void SetTableColor(void) {
    ELight.DesiredColor.Red   = pgm_read_byte(&ColorTable[ELight.Indx][0]);
    ELight.DesiredColor.Green = pgm_read_byte(&ColorTable[ELight.Indx][1]);
    ELight.DesiredColor.Blue  = pgm_read_byte(&ColorTable[ELight.Indx][2]);
   // ELight.DesiredColor.Blue=ELight.DesiredColor.Blue *0.95; // так нельзяшеньки
	ELight.DesiredColor.Uf = 0;
}


// ============================== Tasks ========================================
void SENS_Task (void) {
    if (!TimerDelayElapsed (&ESens.Timer, ESens.PollTime)) return;
    // Reset to default wait time
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        ESens.PollTime = SENS_POLL_TIME;
    }
	if (!(COVER_IS_UP()) )EVENT_CoverClosed();
		else EVENT_CoverOpened();
	if (!(ESens.CoverIsOpen))return;  // при закрытой крышке игнорируем все датчики
	
   if (CRISTAL_IS_INSERT()) EVENT_CristalInserted();
		else EVENT_CristalExtract();
	if 	(ESens.CristalIsExtracted) return;// при извлеченном кристале игнорируем все датчики
		
    switch (ESens.Step)
    {
        case SENS_STATE_START:
            if (SENS_UP_IS_ON()) ESens.Step=SENS_STATE_UP;             // Touch occured
            if (SENS_DOWN_IS_ON()) ESens.Step=SENS_STATE_DOWN;          // Touch occured
            if (SENS_UF_SWITCH_IS_ON()) ESens.Step=SENS_STATE_UF_SWITCH; // Touch occured
            if (ESens.Step!=SENS_STATE_START) TimerResetDelay(&ESens.HoldTimer);  // Reset hold timer
            break;
        case SENS_STATE_UP:
            if (SENS_DOWN_IS_ON()) ESens.Step=SENS_STATE_BOTH; // Touch occured
            if (!(SENS_UP_IS_ON())) ESens.Step=SENS_STATE_START; // Detouch occured
			if (!(COVER_IS_UP()) )ESens.Step=SENS_STATE_START;  // крышка закрыта
            if (TimerDelayElapsed(&ESens.HoldTimer, SENS_HOLD_TICK_TIMEOUT)) EVENT_UpHoldTick();
            break;
        case SENS_STATE_DOWN:
            if (SENS_UP_IS_ON()) ESens.Step=SENS_STATE_BOTH;    // Touch occured
            if (!(SENS_DOWN_IS_ON())) ESens.Step=SENS_STATE_START; // Detouch occured   
            if (TimerDelayElapsed(&ESens.HoldTimer, SENS_HOLD_TICK_TIMEOUT)) EVENT_DownHoldTick();
            break;
        case SENS_STATE_UF_SWITCH:
            if (!(SENS_UF_SWITCH_IS_ON())) ESens.Step=SENS_STATE_START; // Detouch occured
			if (ESens.Step==SENS_STATE_START) break; 
            if (TimerDelayElapsed(&ESens.HoldTimer, SENS_HOLD_TICK_TIMEOUT)) EVENT_UfSwitchTouched();
            break;  
        case SENS_STATE_BOTH:
            if (!(SENS_UP_IS_ON())) ESens.Step=SENS_STATE_START; // Detouch occured
            if (!(SENS_DOWN_IS_ON())) ESens.Step=SENS_STATE_START; // Detouch occured 
			if (ESens.Step==SENS_STATE_START) break; 
		    if (TimerDelayElapsed(&ESens.HoldTimer, SENS_HOLD_TICK_TIMEOUT)) EVENT_BothTouched();
            break; 
    }
}

void Light_Task(void) {
    if (!TimerDelayElapsed(&ELight.Timer, LED_STEP_DELAY)) return;
    // Red channel
    if (ELight.CurrentColor.Red != ELight.DesiredColor.Red) {
        if (ELight.DesiredColor.Red < ELight.CurrentColor.Red) {
            ELight.CurrentColor.Red--;
            if (ELight.CurrentColor.Red == 0) LED_RED_DISABLE();
        }
        else {
            if (ELight.CurrentColor.Red == 0) LED_RED_ENABLE();
            ELight.CurrentColor.Red++;
        }
        OCR2B = ELight.CurrentColor.Red;
    }
    // Green channel
    if (ELight.CurrentColor.Green != ELight.DesiredColor.Green) {
        if (ELight.DesiredColor.Green < ELight.CurrentColor.Green) {
            ELight.CurrentColor.Green--;
            if (ELight.CurrentColor.Green == 0) LED_GREEN_DISABLE();
        }
        else {
            if (ELight.CurrentColor.Green == 0) LED_GREEN_ENABLE();
            ELight.CurrentColor.Green++;
        }
        OCR0B = ELight.CurrentColor.Green;
    }
    // Blue channel
    if (ELight.CurrentColor.Blue != ELight.DesiredColor.Blue) {
        if (ELight.DesiredColor.Blue < ELight.CurrentColor.Blue) {
            ELight.CurrentColor.Blue--;
            if (ELight.CurrentColor.Blue == 0) LED_BLUE_DISABLE();
        }
        else {
            if (ELight.CurrentColor.Blue == 0) LED_BLUE_ENABLE();
            ELight.CurrentColor.Blue++;
        }
        OCR0A = ELight.CurrentColor.Blue;
    }
    
    if (ELight.CurrentColor.Uf != ELight.DesiredColor.Uf) {
        if (ELight.DesiredColor.Uf < ELight.CurrentColor.Uf) {
            ELight.CurrentColor.Uf--;
            if (ELight.CurrentColor.Uf == 0) LED_UF_DISABLE();
        }
        else {
            if (ELight.CurrentColor.Uf == 0) LED_UF_ENABLE();
            ELight.CurrentColor.Uf++;
        }
        OCR1A = ELight.CurrentColor.Uf;
    }
}

void CC_Task (void){
    if (MustSleep) return;  // Get out if must sleep
    // Do with CC what needed
    CC_GET_STATE();
    switch (CC.State) {
        case CC_STB_RX_OVF:
            CC_FLUSH_RX_FIFO();
            break;
        case CC_STB_TX_UNDF:
            CC_FLUSH_TX_FIFO();
            break;

        case CC_STB_IDLE:
            // Transmit at once if IDLE
            // Prepare Cale packet
            CC.TX_Pkt.Address = 0;      // Broadcast
            CC.TX_Pkt.PacketID++;       // Increase packet ID, to avoid repeative treatment
            CC.TX_Pkt.CommandID = PKT_ID0_CALE;
            CC.TX_Pkt.Data[0]   = PKT_ID1_CALE;
            // Transmit black color if "shutdown" color is set
            if (
                    (ELight.DesiredColor.Red   == CLR_SHDN_RED) &&
                    (ELight.DesiredColor.Green == CLR_SHDN_GREEN) &&
                    (ELight.DesiredColor.Blue  == CLR_SHDN_BLUE)
                ) {
                CC.TX_Pkt.Data[1] = 0;
                CC.TX_Pkt.Data[2] = 0;
                CC.TX_Pkt.Data[3] = 0;
            }
            else {
                CC.TX_Pkt.Data[1] = ELight.DesiredColor.Red;   // }
                CC.TX_Pkt.Data[2] = ELight.DesiredColor.Green; // }
                CC.TX_Pkt.Data[3] = ELight.DesiredColor.Blue;  // } components of color
            }

            CC_WriteTX (&CC.TX_PktArray[0], CC_PKT_LENGTH); // Write bytes to FIFO
            CC_ENTER_TX();
            break;

        default: // Just get out in case of RX, TX, FSTXON, CALIBRATE, SETTLING
            break;
    }//Switch
}

void Sleep_Task (void) {
    if (!MustSleep) return;
    // Sleep if Current color is black => LED is faded down yet
    if (
            (ELight.CurrentColor.Red   == 0) &&
            (ELight.CurrentColor.Green == 0) &&
            (ELight.CurrentColor.Blue  == 0) &&
            (ELight.CurrentColor.Uf  == 0)
        ) {
        // Enter sleep
        CC_ENTER_IDLE();    // }
        CC_POWERDOWN();     // } Shutdown CC

        // Enable IRQ to wake
        cli();
        COVER_IRQ_SETUP_MASK();
        COVER_IRQ_CLEAR();
        COVER_IRQ_ENABLE();
        sei();
        // Enter sleep mode
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_enable();
        wdt_disable();
        sleep_cpu();    // Sleep now
        // Something happened, wake now
		sleep_disable();
        wdt_enable(WDTO_2S);
		ESens.CoverIsOpen=false;
		EVENT_CoverOpened();// Execute it from here to switch on immediately
		MustSleep=false;
    }
}

// ============================== Events =======================================
void EVENT_BothTouched(void) {
    if (!(ELight.ShdnIsOn)) ELight.OldColor = ELight.DesiredColor;
    // Set "Shutdown" color to demonstrate shutdown
    SetDesiredColor(CLR_SHDN_RED, CLR_SHDN_GREEN, CLR_SHDN_BLUE,CLR_SHDN_UF);
    ELight.ShdnIsOn=true;
    // Increase sensor reaction time to avoid undesired switch-on events during detouching
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        ESens.PollTime = SENS_BOTHDOWN_IGNORE_TIME;
    }
}

void EVENT_UfSwitchTouched(void) {
    if (ELight.ShdnIsOn) // убираем "Shutdown" color и выставляем значение сохраненного цвета
    {
        ELight.DesiredColor = ELight.OldColor;
        ELight.ShdnIsOn=false;
        return;
    }
    // Toggle Uf on/off
    if (!ELight.UfIsOn) { // Switch on
        ELight.UfIsOn = true;
        ELight.SavedColor = ELight.DesiredColor;
		ELight.DesiredColor=ELight.SavedUfColor;
    }
    else {
        ELight.UfIsOn = false;
		ELight.SavedUfColor = ELight.DesiredColor;
        ELight.DesiredColor = ELight.SavedColor;
    }
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        ESens.PollTime = SENS_UF_TOGGLE_IGNORE_TIME;
    }
}

void EVENT_UpHoldTick(void) {   // Fires every N ms when Up is holded
    if (ELight.ShdnIsOn) // убирам "Shutdown" color и выставляем значение сохраненного цвета
    {
        ELight.DesiredColor = ELight.OldColor;
        ELight.ShdnIsOn=false;
        return;
    }
    
    if (ELight.UfIsOn)  // Уф сейчас включен, прибавляем его яркость
    {
        if (ELight.DesiredColor.Uf<0xFF) ELight.DesiredColor.Uf+= 0x0f;
        return;
    }
    SetTableColor();
    if (ELight.Indx != COLOR_COUNT-1)  ELight.Indx++;
}
void EVENT_DownHoldTick(void) { // Fires every N ms when Down is holded
    if (ELight.ShdnIsOn)  // убирам "Shutdown" color и выставляем значение сохраненного цвета
    {
        ELight.DesiredColor = ELight.OldColor;
        ELight.ShdnIsOn=false;
        return;
    }
    if (ELight.UfIsOn)  // Уф сейчас включен, убавляем его яркость
    {
        if (ELight.DesiredColor.Uf>0x0f) ELight.DesiredColor.Uf-= 0x0f;
        return;
    }
	SetTableColor();
    if (ELight.Indx != 0)  ELight.Indx--;

}


void EVENT_CoverClosed(void) {
	if (!(ESens.CoverIsOpen))return; // уже отработали закрытие
	ELight.SleepColor = ELight.DesiredColor; // Сохраняем цвет до выключения
	SetDesiredColor(0, 0, 0,0);
	MustSleep=true;
	ESens.CoverIsOpen=false;
}
void EVENT_CoverOpened(void) {
	if (ESens.CoverIsOpen)return; // уже отработали открытие
	ELight.DesiredColor = ELight.SleepColor; // Востонавливаем цвет до перехода в спящий режим.
	MustSleep=false;
	ESens.CoverIsOpen=true;
}

void EVENT_CristalExtract(void) {
	if (ESens.CristalIsExtracted)return; // уже отработали извлечение кристала
	ELight.CristalColor = ELight.DesiredColor; // Сохраняем цвет , который был до извлечения.
	SetDesiredColor(0, 0, 0,0);
	SetCurrentColor(1,1,1,1); // это чтоб сразу выключалась
	ESens.CristalIsExtracted=true;
}
void EVENT_CristalInserted(void) {
	if (!(ESens.CristalIsExtracted))return; // уже отработали вставку кристала
	ELight.DesiredColor = ELight.CristalColor; // Востонавливаем цвет  который был до извлечения.
	ESens.CristalIsExtracted=false;
}

// =========================== Interrupts ======================================
EMPTY_INTERRUPT (PCINT0_vect);