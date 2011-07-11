/* 
 * File:   main.c
 * Author: Kreyl Laurelindo
 *
 * Created on 21 Февраль 2011 г., 22:23
 */

#include <avr/io.h>
#include <stdbool.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include "main.h"
#include "delay_util.h"
#include "battery.h"
#include "common.h"
#include "cc1101.h"
#include "leds.h"

// ============================= Types =========================================
enum Mood_t {Nobody, Neutral, Bad, VeryBad};
struct {
    uint8_t Addr;
    uint8_t CycleCounter;
    bool CC_IsSleeping;
    enum Mood_t SelfMood, FarMood;
} Grape;

struct {
    uint16_t Timer;
    uint16_t Delay;
    bool IsPressed;
    bool FirstTime;
} Btn;

struct {
    bool ItsMoodShow;
    uint16_t Timer;
} MoodShow;

void MoodShow_Task(void);

void ShowMood(void);
void HideMood(void);

// ============================== General ======================================
int main(void) {
    GeneralInit();
    DDRB |= 1<<PB0; // DEBUG
    sei();
    while (1) {
        wdt_reset();    // Reset watchdog
        CC_Task();
//        Battery_Task();
        LEDs_Task();
        Btn_Task();
        MoodShow_Task();
    } // while
}

FORCE_INLINE void GeneralInit(void) {
    //wdt_enable(WDTO_2S);
    ACSR = 1<<ACD;  // Disable analog comparator

    CC_Init();
    LedsInit();
    DelayInit();

    // Grape init
    Grape.Addr = eeprom_read_byte(EE_GRAPE_ADDR);
    if (Grape.Addr == 0xFF) Grape.Addr = 0;   // Erased eeprom
    Grape.CycleCounter = 0;
    Grape.FarMood = Nobody;
    Grape.SelfMood = Neutral;

    // Button
    BTN_DDR &= ~(1<<BTN);
    BTN_PORT |= (1<<BTN);
    Btn.FirstTime = true;

    // Mood LEDs
    MDLED_DDR |= (1<<MDLED_RED)|(1<<MDLED_GREEN);
    MoodShow.ItsMoodShow = false;

    // Setup Timer1: cycle timings
    TCNT1 = 0;
    TCCR1A = 0;
    OCR1A = ((uint16_t)Grape.Addr) * PKT_DURATION;  // TX start
    ICR1 = SUBCYCLE_DURATION;                       // TX + RX/Sleep duration
    TIMSK |= (1<<OCIE1A)|(1<<TICIE1);
    CYC_TIMER_START();

    // Battery
    //BatteryInit();
}

// ============================== Tasks ========================================
void CC_Task (void) {
    // Handle packet if received
    if (CC.NewPacketReceived) {
        CC.NewPacketReceived = false;
        EVENT_NewPacket();
    }

    // Check if sleeping

    // Do with CC what needed
    CC_GET_STATE();
    switch (CC.State){
        case CC_STB_RX_OVF:
            CC_FLUSH_RX_FIFO();
            break;
        case CC_STB_TX_UNDF:
            CC_FLUSH_TX_FIFO();
            break;

        case CC_STB_RX:
            break;

        default: // Just get out in case of RX, TX, FSTXON, CALIBRATE, SETTLING
            //PORTB &= ~(1<<PB0); // DEBUG
            break;
    }//Switch
}

// Handle rise & fade
void LEDs_Task(void) {
    if (!DelayElapsed(&ELeds.Timer, LED_STEP_DELAY)) return;

    if      (ELeds.CurrentPWM < ELeds.DesiredPWM) ELeds.CurrentPWM++;
    else if (ELeds.CurrentPWM > ELeds.DesiredPWM) ELeds.CurrentPWM--;
    else {  // Current = desired
        if (ELeds.DesiredPWM == 0) {    // Bottom of fade out, switch to next LED
            PWMStop();
            LedSwitch(Off);             // Turn off current LED
            if (Grape.FarMood != Nobody) {  // otherwise, left things as is
                ChooseNextLED();
                ELeds.DesiredPWM = PWM_MAX;
                PWMStart();
            } // if somebody
        } // if bottom
        else ELeds.DesiredPWM = 0;
    } // if current == desired
    PWMSet(ELeds.CurrentPWM);
}

void ChooseNextLED(void) {
    switch (Grape.FarMood) {
        case Neutral:   // Greens only
            if (ELeds.LedColor != Green) ELeds.LedColor = Green;
            ELeds.LedGreenNumber++;
            if (ELeds.LedGreenNumber >= GREEN_COUNT) ELeds.LedGreenNumber = 0;
            break;

        case Bad:   // Greens and Reds
            if (ELeds.LedColor == Red) {    // Set to green
                ELeds.LedColor = Green;
                ELeds.LedGreenNumber++;
                if (ELeds.LedGreenNumber >= GREEN_COUNT) ELeds.LedGreenNumber = 0;
            }
            else {  // Set to red
                ELeds.LedColor = Red;
                ELeds.LedRedNumber++;
                if (ELeds.LedRedNumber >= RED_COUNT) ELeds.LedRedNumber = 0;
            }
            break;

        case VeryBad:   // Reds only
            if (ELeds.LedColor != Red) ELeds.LedColor = Red;
            ELeds.LedRedNumber++;
            if (ELeds.LedRedNumber >= RED_COUNT) ELeds.LedRedNumber = 0;
            break;

        default:
            break;
    } // Switch
}

void Btn_Task(void) {
    if (!DelayElapsed(&Btn.Timer, 100)) return;
    if (BTN_IS_PRESSED() && !Btn.IsPressed) {   // Keypress occured
        Btn.IsPressed = true;
        // switch mood
        if (!Btn.FirstTime) {
            switch (Grape.SelfMood) {
                case Neutral: Grape.SelfMood = Bad;     break;
                case Bad:     Grape.SelfMood = VeryBad; break;
                case VeryBad: Grape.SelfMood = Neutral; break;
                default: break;
            } // switch
        }
        ShowMood();
        Btn.FirstTime = false;
    } // if presed
    else if (!BTN_IS_PRESSED() && Btn.IsPressed) {  // Key depress occured
        Btn.IsPressed = false;
    }
}


void MoodShow_Task(void) {
    if (!MoodShow.ItsMoodShow) return;
    if (DelayElapsed(&MoodShow.Timer, 2000)) HideMood();
}
void ShowMood(void) {
    MoodShow.ItsMoodShow = true;
    DelayReset(&MoodShow.Timer);
    switch (Grape.SelfMood) {
        case Neutral:
            MDLED_GREEN_ON();
            MDLED_RED_OFF();
            break;
        case Bad:
            MDLED_GREEN_ON();
            MDLED_RED_ON();
            break;
        case VeryBad:
            MDLED_GREEN_OFF();
            MDLED_RED_ON();
            break;
        default: break;
    }
}
void HideMood(void) {
    MDLED_GREEN_OFF();
    MDLED_RED_OFF();
    MoodShow.ItsMoodShow = false;
    Btn.FirstTime = true;
}

// ============================== Events =======================================
FORCE_INLINE void EVENT_NewPacket(void) {
    Grape.FarMood = CC.RX_Pkt.Mood;
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        // Adjust our timer if sender address is lower, and do not otherwise
        if (CC.RX_Pkt.SenderAddr < Grape.Addr) {
            CYC_TIMER_STOP();
            Grape.CycleCounter = CC.RX_Pkt.SenderCycle;
            TCNT1 = CC.RX_Pkt.SenderTime;
            CYC_TIMER_START();
        } // if addr
    } // Atomic
}

void EVENT_ADCMeasureCompleted(void) {
    // Choose mode of LED
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (Battery.ADCValue <= BAT_U_DISCHARGED) {
        }
    }
}

// ============================ Interrupts =====================================
// Transmit interrupt
ISR(TIMER1_COMPA_vect) {
    PBHI;   // DEBUG
    //_delay_us(500);
    CYC_TIMER_STOP();
    CC_ENTER_IDLE();
    // Prepare packet & transmit it
    CC.TX_Pkt.Mood = Grape.SelfMood;
    CC.TX_Pkt.SenderAddr = Grape.Addr;
    CC.TX_Pkt.SenderCycle = Grape.CycleCounter;
    CC.TX_Pkt.SenderTime = TCNT1;   // No need in ATOMIC here as we are in IRQ handler
    CC_WriteTX (&CC.TX_PktArray[0], CC_DATA_LNG); // Write bytes to FIFO 
    CC_ENTER_TX();
    // Wait for packet to transmit completely
    while (!CC_GDO0_IS_HI());   // After this, SYNC word is transmitted
    while (CC_GDO0_IS_HI());    // After this, packet is transmitted
    if (Grape.CycleCounter == 0) CC_ENTER_RX();  // Enter RX-after-TX
    CYC_TIMER_START();
    CC_GDO0_IRQ_RESET();    // Do not check RX immediately
    PBLO;   // DEBUG
}

// SubCycle end interrupt
ISR(TIMER1_CAPT_vect) { // Means overflow IRQ
    //PORTC |= 1<<PC6;
    //_delay_us(500);
    // Handle cycle counter
    if (++Grape.CycleCounter >= CYCLE_COUNT) {  // Zero cycle begins
        Grape.CycleCounter = 0;
        Grape.FarMood = Nobody; // Reset mood
        CC_ENTER_RX();          // Enter RX-before-TX
    }
    else CC_ENTER_IDLE();   // Non-zero cycle
    //PORTC &= ~(1<<PC6);
}


