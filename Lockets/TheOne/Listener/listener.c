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

#include "listener.h"
#include "cc1101.h"
#include "time_utils.h"
#include "common.h"
#include "battery.h"
#include "motor.h"

// ================================= Global ====================================
uint32_t CCPktID;

struct {
    uint16_t Timer;
    bool IsOn;
} ELED;
// =============================== General =====================================
int main(void) {
    GeneralInit();
    DDRA = 1<<PA0;   // DEBUG
    // ******** Main cycle *********
    while (1){
        wdt_reset();    // Reset watchdog
        CC_Task();
        LED_Task();
        Battery_Task();
        Motor_TASK();
    } // while 1
}

FORCE_INLINE void GeneralInit(void){
    wdt_enable(WDTO_2S);
    ACSR = 1<<ACD;  // Disable analog comparator
    TimerInit();

    // LED init
    LED_DDR |= (1<<LED_P);
    LED_ON();

    MotorInit();
    // Motor blink at power-on
    MotorSetCount(2);

    // CC init
    CC_Init();
    CC_SetChannel(45);   // Set needed channel

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
    if (ELED.IsOn) {
        if (TimerDelayElapsed(&ELED.Timer, LED_ON_PERIOD)) {
            ELED.IsOn = false;
            LED_OFF();
        }
    }
    else {
        if (TimerDelayElapsed(&ELED.Timer, LED_OFF_PERIOD)) {
            ELED.IsOn = true;
            LED_ON();
        }
    }
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
            PORTA &= ~(1<< PA0);
            CC_ENTER_RX();
            break;

        default: // Just get out in case of RX, TX, FSTXON, CALIBRATE, SETTLING
            break;
    }//Switch
}


// =============================== Events ======================================
FORCE_INLINE void EVENT_NewPacket(void) {
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        if (CC.RX_Pkt.PacketID != CCPktID) {    // Handle only new packets
            CCPktID = CC.RX_Pkt.PacketID;
            MotorSetCount(2);
            LED_ON();
            ELED.IsOn = true;
            TimerResetDelay(&ELED.Timer);
        }
    }
}