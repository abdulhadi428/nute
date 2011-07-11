/* 
 * File:   Lia.c
 * Author: Laurelindo
 *
 * Created on 14.11.2010 г., 0:37
 */
#include <avr/io.h>
#include <stdbool.h>
#include <avr/wdt.h>
#include "Lia.h"
#include "delay_util.h"
#include "common.h"
#include "cc2500.h"

#include <util/delay.h>

//#define DEBUG_UART
#ifdef DEBUG_UART
#include "uart_soft.h"
#endif

// ============================= Types =========================================
struct {
    uint16_t Timer;
    bool JustEnteredRX;
    bool DeepSleep;
} CC_Srv;

struct {
    uint8_t ID2, ID1;   // Tirno's ID
    uint8_t Address;
    uint8_t Chnls[7], CurntCh;
} ELia;

// ============================== General ======================================
int main(void) {
    GeneralInit();

    #ifdef DEBUG_UART
    UARTInit();
    UARTSendString_P(PSTR("Lia is here\r"));
    #endif

    uint16_t FTimer;
    DelayReset(&FTimer);

    sei(); 
    while (1) {
        wdt_reset();    // Reset watchdog
        CC_Task();

        if(DelayElapsed(&FTimer, 500)) LED_OFF();
    } // while
}

FORCE_INLINE void GeneralInit(void) {
    wdt_enable(WDTO_2S);
    // Shutdown all unneeded
    PRR = (1<<PRTWI)|(1<<PRSPI);
    ACSR = 1<<ACD;  // Disable analog comparator
    // Setup timer
    DelayInit();    // Time counter
    // LED
    #ifndef DEBUG_UART
    LED_DDR |= 1<<LED_P;
    LED_OFF();
    #endif

    // CC init
    //CC_Srv.JustEnteredRX = false;
    //CC_Srv.DeepSleep = false;
    DelayReset(&CC_Srv.Timer);
    CC_Init();
    CC_SetAddress(4);
}

// ============================== Tasks ========================================
#define CC_RX_PERIOD    270
#define CC_RX_DURATION  200

void CC_Task(void) {
    // Handle packet if received
    if (CC.NewPacketReceived) {
        CC.NewPacketReceived = false;
        EVENT_NewPacket();
        return;
    }
/*
    // If in sleep mode, check if it is time to wake-up; otherwise return
    if (CC_Srv.DeepSleep) {
        if (DelayElapsed(&CC_Srv.Timer, CC_RX_OFF_DELAY)) CC_Srv.DeepSleep = false;
        else return;
    }
*/
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
            // Enter RX if time has come
            if(DelayElapsed(&CC_Srv.Timer, CC_RX_PERIOD)) CC_ENTER_RX();
            break;

        case CC_STB_RX:
            // Check if time to get out of RX
            if(DelayElapsed(&CC_Srv.Timer, CC_RX_DURATION)) CC_ENTER_IDLE();
            break;

/*
 * // Idle mode means that CC just has awaken
            CC_ENTER_RX();
            CC_Srv.JustEnteredRX = true;
            break;

        case CC_STB_RX:
            // Reset timer if just entered RX
            if (CC_Srv.JustEnteredRX) {
                CC_Srv.JustEnteredRX = false;
                DelayReset(&CC_Srv.Timer);
            }
            else {
                // Check if CC_RX_ON_DELAY delay elapsed
                if (DelayElapsed(&CC_Srv.Timer, CC_RX_ON_DELAY)) { // Time to switch off
                    CC_ENTER_IDLE();
                    CC_POWERDOWN();
                    CC_Srv.DeepSleep = true;
                }// if timer
            }// if not RX
            break;
*/

        default: // Just get out in case of RX, TX, FSTXON, CALIBRATE, SETTLING
            break;
    }//Switch
}


// ============================== Events =======================================
FORCE_INLINE void EVENT_NewPacket(void) {
    if(CC.RX_Pkt.CommandID == PKT_ID_CALL) {
        // Send Call packet
        CC.TX_Pkt.Address = 0;      // Broadcast
        CC.TX_Pkt.CommandID = PKT_ID_CALL;
        CC.TX_Pkt.Data[0] = 0;
        CC.TX_Pkt.Data[1] = 0;
        CC_WriteTX (&CC.TX_PktArray[0], CC_PKT_LENGTH); // Write bytes to FIFO
        CC_ENTER_TX();
    }
    DelayReset(&CC_Srv.Timer);

#ifdef DEBUG_UART
    UARTSendUint(CC.RX_Pkt.PacketID);
    UARTSend(' ');
    UARTSendAsHex(CC.RX_Pkt.Data[1]);
    UARTSend(' ');
    UARTSendAsHex(CC.RX_Pkt.Data[2]);
    UARTSend(' ');
    UARTSendAsHex(CC.RX_Pkt.Data[3]);

    UARTSend(' ');
    UARTSend(' ');
    UARTSendUint(CC.RX_Pkt.RSSI);
    UARTSend(' ');
    UARTSendAsHex(CC.RX_Pkt.LQI);
    UARTNewLine();
    #endif
}
