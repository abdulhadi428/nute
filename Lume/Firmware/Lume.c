#include "Lume.h"
#include "common.h"
#include <util/delay.h>
#include <util/atomic.h>

// ============================ Types & variables ==============================
struct {
    uint8_t Hour;
    uint8_t HyperMinute;
    uint8_t Second;
} Time;

enum {ModeRegular, ModeSetHours, ModeSetMinutes} Mode;

struct {
    uint8_t HByte, MByte;   // Control bytes
    // PWMs
    uint8_t PWM_Top;
    struct PWM_t Min0PWM, Min1PWM;
} LControl;

volatile uint16_t TickCounter;

// Tables of accordance
const uint8_t HTable[12] = {
    0b0, // H0 == H12
    0b0, // H1
    0b0, // H2
    0b0, // H3
    0b0, // H4
    0b0, // H5
    0b0, // H6
    0b0, // H7
    0b0, // H8
    0b0, // H9
    0b0, // H10
    0b0, // H11
};
const uint8_t MTable[24][2] = { // First byte for minutes bits, second for minute byte
    {0b10000000, 0b00101000},   // M12
    {0b00000000, 0b01000000},   // M0.5
    {0b00000000, 0b10000000},   // M1
    {0b00000000, 0b01000001},   // M1.5
    {0b00000000, 0b10001000},   // M2
    {0b00000000, 0b01000010},   // M2.5
    {0b00000000, 0b10010000},   // M3
    {0b00000000, 0b01000011},   // M3.5
    {0b00000000, 0b10011000},   // M4
    {0b00000000, 0b01000100},   // M4.5
    {0b00000000, 0b10100000},   // M5
    {0b00000000, 0b01000101},   // M5.5
    {0b00000000, 0b10101000},   // M6
    {0b01000000, 0b00000000},   // M6.5
    {0b10000000, 0b00000000},   // M7
    {0b01000000, 0b00000001},   // M7.5
    {0b10000000, 0b00001000},   // M8
    {0b01000000, 0b00000010},   // M8.5
    {0b10000000, 0b00010000},   // M9
    {0b01000000, 0b00000011},   // M9.5
    {0b10000000, 0b00011000},   // M10
    {0b01000000, 0b00000100},   // M10.5
    {0b10000000, 0b00100000},   // M11
    {0b01000000, 0b00000101},   // M11.5
};

// ============================ Implementation =================================
int main (void) {
    GeneralInit ();

    sei();

    while (1) {  // forever
        TASK_Toggle();
    }	// while 1
}

void GeneralInit(void) {
    // Light control
    L_DDR  |=   (1<<DATA_IN)|(1<<LATCH)|(1<<SRCLK);
    L_PORT &= ~((1<<DATA_IN)|(1<<LATCH)|(1<<SRCLK));
    M_PWM_DDR  |= (1<<M0PWM)|(1<<M1PWM);
    H_PWM_DDR  |= (1<<H0PWM)|(1<<H1PWM);
    // Hours PWM
    TCCR1A = (1<<COM1A1)|(1<<COM1A0)|(1<<COM1B1)|(1<<COM1B0)|(0<<WGM11)|(1<<WGM10);
    TCCR1B = (0<<WGM13)|(1<<WGM12)|(1<<CS12)|(0<<CS11)|(0<<CS10);
    OCR1AL = 0;
    OCR1BL = 0;
    // Minutes PWM & delay counter
    TCCR0A = (1<<COM0A1)|(1<<COM0A0)|(1<<COM0B1)|(1<<COM0B0)|(1<<WGM01)|(1<<WGM00);
    TCCR0B = (0<<CS02)|(0<<CS01)|(1<<CS00);
    OCR0A = 200;
    OCR0B = 20;
    TIMSK0 = (1<<TOIE0);

    // Serial port setup
    UCSR0B = (1<<TXEN0);    // TX enable, irqs disable
    UCSR0C = (1<<UMSEL01)|(1<<UMSEL00)|(0<<UDORD0)|(0<<UCPHA0)|(0<<UCPOL0); // MSB first, leading edge, idle low
    UBRR0  = 0; // Max speed

    // Keys


    // Timer2: realtime clock counter
    //TCCR2B = (0<<CS22)|(0<<CS21)|(1<<CS20);	// DEBUG: no division
    TCCR2B = (0<<CS22)|(1<<CS21)|(0<<CS20);	// DEBUG: div 8
    //TCCR2B = (0<<CS22)|(1<<CS21)|(1<<CS20);	// DEBUG: div 32
    ASSR  = (1<<AS2);				// Set Async mode of the timer
    TCCR2A = 0;
    //TCCR2B = (1<<CS22)|(0<<CS21)|(1<<CS20);	// RELEASE: div 128, once per second
    TIMSK2 |= (1<<TOIE2);

    // Setup initial values
    Time.Second = 1;
    Time.HyperMinute = 0;
    Time.Hour = 0;

    LControl.PWM_Top = 250;
    LControl.Min0PWM.OCRX = &OCR0A;
    LControl.Min1PWM.OCRX = &OCR0B;

    Mode = ModeRegular;

    // Start-up time setup
    EVENT_NewHour();
    EVENT_NewHyperMinute();
}

// =============================== Tasks =======================================
// Check if need toggle something
void TASK_Toggle(void) {
    if(!POWER_OK()) return;

    TogglePWM(&LControl.Min0PWM);
    TogglePWM(&LControl.Min1PWM);
}

// =============================== Light control ===============================
FORCE_INLINE void WriteControlBytes(void) {
    while (bit_is_clear(UCSR0A, UDRE0));    // Wait until buffer is empty
    UDR0 = LControl.MByte;
    while (bit_is_clear(UCSR0A, UDRE0));    // Wait until buffer is empty
    UDR0 = LControl.HByte;
    // Toggle latch
    UCSR0A = (1<<TXC0);                     // Reset TXC0 flag
    while (bit_is_clear(UCSR0A, TXC0));     // Wait until shift register is empty
    L_PORT |=  (1<<LATCH);
    nop();
    L_PORT &= ~(1<<LATCH);
}

void TogglePWM(struct PWM_t *pwm) {
    if(pwm->Mode == PWMHold) return;    // Nothing to do here
    if(!TimerDelayElapsed(&(pwm->Timer), pwm->Delay)) return;   // Not in time
    if(pwm->Mode == PWMRise) {
        pwm->Value++;
        if(pwm->Value == LControl.PWM_Top) pwm->Mode = PWMHold;
        *(pwm->OCRX) = pwm->Value;
    } // if mode
    else {
        pwm->Value--;
        if(pwm->Value == 0) pwm->Mode = PWMHold;
    }
    *(pwm->OCRX) = pwm->Value;
}

// ============================ Events =========================================
void EVENT_NewHour(void) {
    // Setup Hours byte
//    uint8_t HTmp = LControl.HByte & 0b11000000; // Save bits that belong to minutes
//    LControl.HByte = HTmp + 0b00111111;//HTable[Time.Hour];
}
// Switch on needed minute channels, setup their start PWM and pepare to change
void EVENT_NewHyperMinute(void) {
    // Setup Minutes byte
    LControl.HByte &= 0b00111111;   // Clear minutes bits
    LControl.MByte = 0;             // Clear minutes byte
    // Here is switching logic
    //RiseMinute(Time.HyperMinute);
    FadeMinute(Time.HyperMinute);
/*
    switch(Time.HyperMinute) {
        case 0: RiseMinute(M12);  break;
        case 1: RiseMinute(M0_5); break;
    } // switch
*/

    //LControl.MByte = MTable[Time.HyperMinute];
    // Write bytes to setup LEDs
    WriteControlBytes();
}

void RiseMinute(uint8_t AMinute) {
    // Setup channel
    LControl.HByte |= MTable[AMinute][0];
    LControl.MByte |= MTable[AMinute][1];
    // Setup PWM
    if(AMinute & 0x01) SetupPWM(&LControl.Min1PWM, PWMRise);    // odd => integers
    else               SetupPWM(&LControl.Min0PWM, PWMRise);    // if even => fractional
}
void FadeMinute(uint8_t AMinute) {
    // Setup channel
    LControl.HByte |= MTable[AMinute][0];
    LControl.MByte |= MTable[AMinute][1];
    // Setup PWM
    if(AMinute & 0x01) SetupPWM(&LControl.Min1PWM, PWMFade);    // odd => integers
    else               SetupPWM(&LControl.Min0PWM, PWMFade);    // if even => fractional

}

void SetupPWM(struct PWM_t *pwm, enum PWMMode_t mode) {
    pwm->Mode = mode;
    TimerResetDelay(&(pwm->Timer));
    if(mode == PWMRise) {
        pwm->Value = 0;
        *(pwm->OCRX) = 0;
        pwm->Delay = PWMDelay1;
    }
    else {
        pwm->Value = LControl.PWM_Top;
        *(pwm->OCRX) = LControl.PWM_Top;
        pwm->Delay = PWMDelay1;     // !!!!!!!FIXME
    } // else
}


// ============================ Delay utils ====================================
bool TimerDelayElapsed(uint16_t *AVar, const uint16_t ADelay) {
    uint16_t ttmp;
    bool Result = false;
    ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
        ttmp = TickCounter;
        if ((ttmp - *AVar) >= ADelay) {
            *AVar = ttmp;   // Reset delay
            Result = true;
        }
    } // Atomic
    return Result;
}

void TimerResetDelay(uint16_t *AVar) {
    ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
        *AVar = TickCounter;
    }
}


// =========================== Interrupts ======================================
// Time counter
ISR(TIMER2_OVF_vect) { 
    //H_PWM_PORT ^= (1<<H0PWM);

    // Get out in settings mode
    if (Mode != ModeRegular) return;

    Time.Second++;
    if(Time.Second > 150) {    // 150 seconds in one hyperminute
        Time.Second = 1;
        Time.HyperMinute++;
        if(Time.HyperMinute > 23) { // 24 HyperMinutes in one hour
            Time.HyperMinute = 0;
            Time.Hour++;
            if(Time.Hour > 11) Time.Hour = 0;
    //        if (POWER_OK()) EVENT_NewHour();
        } // if Hyperminute > 23
        if (POWER_OK()) EVENT_NewHyperMinute();
    }// Second
}

// Delay counter
ISR(TIMER0_OVF_vect) {
    TickCounter++;
}
