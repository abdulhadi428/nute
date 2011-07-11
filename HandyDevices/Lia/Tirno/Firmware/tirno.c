/* 
 * File:   tirno.c
 * Author: Laurelindo
 *
 * Created on 27 06 2010 ã., 0:30
 */
#include <avr/io.h>
#include <stdbool.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include "tirno.h"
#include "delay_util.h"
//#include "battery.h"
#include "common.h"
#include "cc2500.h"
#include "keys.h"
#include "menu.h"

struct Lockets_t EL; 

struct {
    uint8_t ID[3];
    bool PktRcvd;
    uint16_t CC_Timer;
} ETirno;


// ============================== General ======================================
int main(void) {
    GeneralInit();

    SetState(StateList);

    uint16_t FTimer;
    DelayReset(&FTimer);

    while(1) {
        wdt_reset();    // Reset watchdog
        Keys_Task();
        CC_Task();

        // Clear dBm
        if(EState == StateSearch) {
            if(DelayElapsed(&FTimer, 1000)) {
                if(!ETirno.PktRcvd) {
                    LCD_PrintString(5, 2, "     ", false);
                    LCD_GaugeValue(1);
                }
                ETirno.PktRcvd = false;
            }
        }
        //Battery_Task();
    } // while
}

FORCE_INLINE void GeneralInit(void) {
    //wdt_enable(WDTO_2S);
    ACSR = 1<<ACD;  // Disable analog comparator

    // Read self serial
    //eeprom_read_block(ETirno.ID, 0, 3);
    ETirno.ID[0] = 0;   // DEBUG
    ETirno.ID[1] = 0;
    ETirno.ID[2] = 0;

    // Init lockets
    for(uint8_t i=0; i<LOCKET_COUNT; i++) eeReadLocket(i);

    DelayInit();
    LCD_Init();
    KeysInit();
    sei();

    // CC init
    CC_Init();
    //CC_SetChannel(0);
    CC_SetAddress(4);   //Never changes in CC itself
    //CC_ENTER_RX();


    // Setup Timer1: cycle timings
/*
    TCNT1 = 0;
    TCCR1A = 0;
    OCR1A = ((uint16_t)Corma.Addr) * PKT_DURATION;  // TX start
    ICR1 = SUBCYCLE_DURATION;                       // TX + RX/Sleep duration
    TIMSK |= (1<<OCIE1A)|(1<<TICIE1);
    CYC_TIMER_START();
*/

    // Battery
    //BatteryInit();
}

// ============================== Tasks ========================================
#define TX_PERIOD   270
#define RX_PERIOD   200
void CC_Task (void) {
    // Handle packet if received
    if (CC.NewPacketReceived) {
        CC.NewPacketReceived = false;
        EVENT_NewPacket();
    }

    // Do with CC what needed
    CC_GET_STATE();
    switch (CC.State){
        case CC_STB_RX_OVF:
            CC_FLUSH_RX_FIFO();
            break;
        case CC_STB_TX_UNDF:
            CC_FLUSH_TX_FIFO();
            break;

        case CC_STB_IDLE:
            if(EState == StateSearch) { // No deal if we are not in search
                // Check if time to TX
                if(DelayElapsed(&ETirno.CC_Timer, TX_PERIOD)) {
                    // Prepare Call packet
                    CC.TX_Pkt.Address = ETirno.ID[0];
                    CC.TX_Pkt.CommandID = PKT_ID_CALL;
                    CC.TX_Pkt.Data[0] = ETirno.ID[2];
                    CC.TX_Pkt.Data[1] = ETirno.ID[1];
                    CC_WriteTX (&CC.TX_PktArray[0], CC_PKT_LENGTH); // Write bytes to FIFO
                    CC_ENTER_TX();  // will switch to RX automatically (ref. to RF settings)
                } // if time
            } // if search
            break;

        case CC_STB_RX:
            // Get out if time has come
            if(DelayElapsed(&ETirno.CC_Timer, RX_PERIOD)) CC_ENTER_IDLE();
            break;

        default: // Just get out in case of RX, TX, FSTXON, CALIBRATE, SETTLING
            break;
    }//Switch
}

/*
FORCE_INLINE void EVENT_NewPacket(void) {
    if (CC.RX_Pkt.CommandID == PKT_ID_CALL) {
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
             Count this one if did not do it yet
            bool IsCounted = false;
            for (uint8_t i=0; i<Corma.OthersCounter; i++)
                if (Corma.OtherAddr[i] == CC.RX_Pkt.SenderAddr) {
                    IsCounted = true;
                    break;
                }
            if (!IsCounted)
                Corma.OtherAddr[Corma.OthersCounter++] = CC.RX_Pkt.SenderAddr;
            
             Adjust our timer if sender address is lower, and do not otherwise
            if (CC.RX_Pkt.SenderAddr < Corma.Addr) {
                CYC_TIMER_STOP();
                Corma.CycleCounter = CC.RX_Pkt.SenderCycle;
                TCNT1 = CC.RX_Pkt.SenderTime;
                CYC_TIMER_START();
            } // if addr
        } // Atomic
    } // if PKT_ID_Call
}
void EVENT_ADCMeasureCompleted(void) {
     Choose mode of LED
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (Battery.ADCValue <= BAT_U_DISCHARGED) {
            ELED.Blink = false;
        }
        else ELED.Blink = true;
    }
}
*/

// ============================ Interrupts =====================================
// Transmit interrupt
/*
ISR(TIMER1_COMPA_vect) {
    PORTA &= ~(1<<PA4);
    PORTA |= (1<<PA3); // DEBUG
    CYC_TIMER_STOP();
     Prepare packet & transmit it
    CC.TX_Pkt.ToAddr = 0;      // Broadcast
    CC.TX_Pkt.CommandID = PKT_ID_CALL;
    CC.TX_Pkt.SenderAddr = Corma.Addr;
    CC.TX_Pkt.SenderCycle = Corma.CycleCounter;
    CC.TX_Pkt.SenderTime = TCNT1;   // No need in ATOMIC as we are in IRQ handler
    CC_WriteTX (&CC.TX_PktArray[0], CC_PKT_LENGTH); // Write bytes to FIFO
    CC_EnterTX();
     Wait for packet to transmit completely
    while (!CC_GDO0_IS_HI());   // After this, SYNC word is transmitted
    while (CC_GDO0_IS_HI());    // After this, packet is transmitted
    if (Corma.CycleCounter == 0) CC_EnterRX();  // Enter RX after TX
    CYC_TIMER_START();
    PORTA &= ~(1<<PA3); // DEBUG
}

 SubCycle end interrupt
ISR(TIMER1_CAPT_vect) { // Means overflow IRQ
    PORTA |= (1<<PA1);// DEBUG
    _delay_us(500);
     Handle cycle counter
    if (++Corma.CycleCounter >= CYCLE_COUNT) {  // Zero cycle begins
        Corma.CycleCounter = 0;
         Flinch!
        MotorSetCount (Corma.OthersCounter);
        Corma.OthersCounter = 0;
         Enter RX-before-TX
        CC_EnterRX();
    }
    else CC_ENTER_IDLE();   // Non-zero cycle
    PORTA &= ~(1<<PA1);// DEBUG
}

*/

// ============================== Events =======================================
FORCE_INLINE void EVENT_NewPacket(void) {
    if(EState == StateSearch) {
        ETirno.PktRcvd = true;
        LCD_PrintString(5, 2, "     ", false);
        int8_t RSSI = CC_RSSI2dBm();
        LCD_PrintInt(5, 2, RSSI);
        // Draw gauge
        RSSI += 100;
        if(RSSI > 95) RSSI = 95;
        if (RSSI < 1) RSSI = 1;
        LCD_GaugeValue(RSSI);
    }
}

// Setup CC to search here
void EVENT_StartingSearch(void) {
    DelayReset(&ETirno.CC_Timer);
    // Packet will be sent in time, no need to hurry
}

// ========================= Service routines ==================================
void eeReadLocket(uint8_t ID) {
    uint16_t eeaddr = LOCKET_EE_ADDR;
    uint8_t i=0;
    while(i++ < ID) eeaddr += sizeof(struct Locket_t);  // Get ee address
    eeprom_read_block(&EL.L[ID], (void*)eeaddr, sizeof(struct Locket_t));
    if(EL.L[ID].S[0] == 0xFF) {  // Handle uninitialized ee
        EL.L[ID].S[0] = 'Ö';
        EL.L[ID].S[1] = 'å';
        EL.L[ID].S[2] = 'ë';
        EL.L[ID].S[3] = 'ü';
        EL.L[ID].S[4] = ' ';
        EL.L[ID].S[5] = ID < 10? ('0') : ('1');
        EL.L[ID].S[6] = ID < 10? ('0' + ID) : ('0' + ID - 10);
        for(uint8_t j=7; j<LOCKET_NAME_L-1; j++) EL.L[ID].S[j] = ' ';
        EL.L[ID].S[LOCKET_NAME_L-1] = '\0';
    }
}
void eeWriteLocket(uint8_t ID) {
    uint16_t eeaddr = LOCKET_EE_ADDR;
    uint8_t i=0;
    while(i++ < ID) eeaddr += sizeof(struct Locket_t);  // Get ee address
    eeprom_write_block(&EL.L[ID], (void*)eeaddr, sizeof(struct Locket_t));
}
