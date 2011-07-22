/*
 * File:   listener.c of The One project
 * Author: Kreyl Laurelindo
 *
 * Created on 2011.07.20
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <stdbool.h>
#include <avr/wdt.h>
#include <util/atomic.h>
#include <avr/eeprom.h>
#include <util/delay.h>

#include "retranslator.h"
#include "cc1101.h"
#include "time_utils.h"
#include "common.h"
#include "battery.h"

// ================================= Global ====================================
enum CCPwr_t {HiPwr, LoPwr};
struct {
    uint32_t PktID;
    uint16_t Timer, RepeatDelay;
    enum CCPwr_t Pwr;
    bool TransmissionNeeded;
} CCSrv;

uint16_t LedTimer;

// =============================== General =====================================
int main(void) {
    GeneralInit();
    DDRA = (1<<PA0)|(1<<PA1);   // DEBUG
    // ******** Main cycle *********
    while (1){
        wdt_reset();    // Reset watchdog
        CC_Task();
        LED_Task();
        Battery_Task();
    } // while 1
}

FORCE_INLINE void GeneralInit(void){
    wdt_enable(WDTO_2S);
    ACSR = 1<<ACD;  // Disable analog comparator
    TimerInit();

    // LED init
    LED_DDR |= (1<<LED_P);
    LED_ON();
    _delay_ms(702);


    // CC init
    CC_Init();
    CC_SetChannel(45);   // Set needed channel
    CCSrv.RepeatDelay = eeprom_read_byte(EE_ID_ADDRESS);
    if (CCSrv.RepeatDelay == 0xFF) CCSrv.RepeatDelay = 0;   // Erased eeprom
    CCSrv.RepeatDelay <<= 6;    // = *64; pkt duration = 20ms, duration of two packets = 40 ms

    // Battery
    BatteryInit();
    sei();
}

// ================================ Tasks ======================================
void LED_Task(void) {
    // LED ON in case of discharged battery
    if ((Battery.ADCState == ADCNoMeasure) && BATTERY_IS_DISCHARGED()) {
        LED_ON();
        return;
    }
    // Blink otherwise
    if (TimerDelayElapsed(&LedTimer, LED_ON_PERIOD)) LED_OFF();
}

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
            PORTA &= ~((1<< PA0)|(1<<PA1));
            if(CCSrv.TransmissionNeeded) {
                if (CCSrv.Pwr == HiPwr) {
                    if (TimerDelayElapsed(&CCSrv.Timer, CCSrv.RepeatDelay)) {
                        PORTA |= (1<< PA0);
                        CC_SetPwr(CC_PWR_10DBM);// Set Hi TX power
                        // LED on
                        TimerResetDelay(&LedTimer);
                        LED_ON();
                        // Prepare CALL packet
                        ATOMIC_BLOCK(ATOMIC_FORCEON) {
                            CC.TX_Pkt.PacketID = CCSrv.PktID;
                        }
                        CC_WriteTX (&CC.TX_PktArray[0], CC_PKT_LEN); // Write bytes to FIFO
                        CC_ENTER_TX();
                        CCSrv.Pwr = LoPwr;      // Next time transmit at lo pwr
                    } // if delay elapsed
                } // if Hi Pwr
                else { // Lo Power
                    PORTA |= (1<< PA0);
                    // Transmit at lo pwr immediately after Hi Pwr transmission ended
                    CC_SetPwr(CC_PWR_M6DBM);    // Set Lo TX power
                    // Prepare CALL packet
                    CC_WriteTX (&CC.TX_PktArray[0], CC_PKT_LEN); // Write bytes to FIFO
                    CC_ENTER_TX();
                    CCSrv.Pwr = HiPwr;          // Next time transmit at hi pwr
                    CCSrv.TransmissionNeeded = false;
                }
            } // if(CCSrv.TransmissionNeeded)
            else CC_ENTER_RX();
            break;

        default: // Just get out in case of RX, TX, FSTXON, CALIBRATE, SETTLING
            break;
    }//Switch
}


// =============================== Events ======================================
FORCE_INLINE void EVENT_NewPacket(void) {
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        if (CC.RX_Pkt.PacketID != CCSrv.PktID) {    // Handle only new packets
            CCSrv.PktID = CC.RX_Pkt.PacketID;
            CCSrv.TransmissionNeeded = true;
            TimerResetDelay(&CCSrv.Timer);
        }
    }
}
