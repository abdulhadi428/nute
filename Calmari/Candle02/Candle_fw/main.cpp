/*
 * main.cpp
 *
 *  Created on: 18.07.2011
 *      Author: Kreyl
 */

#include "main.h"
#include "avr/power.h"
#include "avr/pgmspace.h"

#include "uart_soft.h"
#include "sensors.h"
#include "battery.h"
#include "cc2500.h"

#define PRINT_RX_PKT

Light_t ELight;

// ========================= Implementation ====================================
int main(void) {
    GeneralInit();
    UARTSendString("Candle\r");
    while(1) {
        wdt_reset();
        EKeys.Task();
        ELight.Task();
        Battery.Task();
        IndicateCharging_Task();
        CC.Task();
    }
    return 0;
}

void GeneralInit(void) {
    wdt_enable(WDTO_2S);
    ACSR = 1<<ACD;  // Disable analog comparator
    clock_prescale_set(clock_div_8);    // 1 MHz
    Battery.Init();
    Delay.Init();
    UARTInit();
    CC.Init();
    ELight.Init();
    EKeys.Init();
    sei();
}

void Shutdown (void) {

}

// ============================== Tasks ========================================
void IndicateCharging_Task(void) {
    if (!Battery.IsCharging) return;
    // Fade and brighten
    if (Delay.Elapsed(&ELight.BlinkTimer, CHARGE_BLINK_T)) {
        if (ELight.Ramp == RampUp) {
            ELight.Ramp = RampDown;
            ELight.SetDesiredColor(0, 0, 4);
        }
        else {
            ELight.Ramp = RampUp;
            ELight.SetDesiredColor(0, 0, 255);
        }
    }
}

void CC_t::Task() {
    if (NewPacketReceived) {    // Load data from FIFO
        NewPacketReceived = false;
        uint8_t FifoSize = ReadRegister(CC_RXBYTES); // Get FIFO bytes count
        if (FifoSize > 0) {
            ReadRX((uint8_t*)&CC.RX_Pkt, CC_PKT_DATA_LENGTH+2);
            EVENT_NewPacket();
        }
    }

    // If in sleep mode, check if it is time to wake-up; otherwise return
    if (IsSleeping) {
        if(Delay.Elapsed(&Timer, CC_RX_OFF_PERIOD)) IsSleeping = false;
        else return;
    }

    GetState();
    switch (State) {
        case CC_STB_RX_OVF:  FlushRxFifo(); break;
        case CC_STB_TX_UNDF: FlushTxFifo(); break;
        case CC_STB_IDLE:
            switch (ELight.ActiveState) {
                case NotActive: // Receive color
                    EnterRx();
                    CC_GDO0_IRQ_ENABLE();
                    Delay.Reset(&Timer);
                    break;
                case Active:    // Transmit color
                    CC_GDO0_IRQ_DISABLE();
                    TX_Pkt.R = ELight.R.Desired;
                    TX_Pkt.G = ELight.G.Desired;
                    TX_Pkt.B = ELight.B.Desired;
                    WriteTX((uint8_t *)&TX_Pkt, CC_PKT_DATA_LENGTH);
                    EnterTx();
                    break;
                case ActiveShutdown:    // Transmit shutdown
                    CC_GDO0_IRQ_DISABLE();
                    TX_Pkt.R = 0;
                    TX_Pkt.G = 0;
                    TX_Pkt.B = 0;
                    WriteTX((uint8_t *)&TX_Pkt, CC_PKT_DATA_LENGTH);
                    EnterTx();
                    break;
            } // switch
            break;
            case CC_STB_RX:
                // Check if CC_RX_ON_PERIOD delay elapsed
                if (Delay.Elapsed(&Timer, CC_RX_ON_PERIOD)) { // Time to switch off
                    CC_GDO0_IRQ_DISABLE();
                    EnterIdle();
                    PowerDown();
                    IsSleeping = true;
                }// if timer
                break;
    } // switch
}

// ============================= Events ========================================
void EVENT_KeyDown(void) {
    UARTSendString("Down\r");
    if (ELight.Indx == 0) return;
    ELight.Indx--;
    ELight.SetTableColor();
}
void EVENT_KeyUp(void) {
    UARTSendString("Up\r");
    if (ELight.Indx == (COLOR_COUNT-1)) return;
    ELight.Indx++;
    ELight.SetTableColor();
}
void EVENT_KeyOnOff(void) {
    UARTSendString("OnOff\r");
    if (ELight.IsOn) {
        ELight.IsOn = false;
        ELight.ActiveState = NotActive;
        ELight.SetDesiredColor(0, 0, 0);
        EKeys.DisableAllButOnOff();
    }
    else {
        ELight.IsOn = true;
        ELight.SetTableColor();
        EKeys.EnableAll();
    }
}
void EVENT_KeyLit(void) {
    UARTSendString("Lit\r");
    if (ELight.ActiveState == NotActive) {
        ELight.ActiveState = Active;
        ELight.BlinkState = BlinkOn;
        Delay.Bypass(&ELight.BlinkTimer, LED_BLINK_ON_T);
    }
    else {
        ELight.ActiveState = NotActive;
        ELight.BlinkState = BlinkDisabled;
    }
}

void EVENT_ChargeStarted(void) {
    // Shutdown CC
//    CC_ENTER_IDLE();
//    CC_POWERDOWN();
    EKeys.DisableAll();
    ELight.BlinkState = BlinkDisabled;
    ELight.Ramp = RampUp;
}
void EVENT_ChargeEnded(void) {
    EKeys.EnableAll();
    ELight.Ramp = RampDown;
    ELight.SetDesiredColor(0, 0, 0);
}

void EVENT_ADCMeasureCompleted(void) {

}

void EVENT_NewPacket(void) {
    ELight.SetDesiredColor(CC.RX_Pkt.R, CC.RX_Pkt.G, CC.RX_Pkt.B);
    #ifdef PRINT_RX_PKT
    UARTSendUint(CC.RX_Pkt.R);
    UARTSend(' ');
    UARTSendAsHex(CC.RX_Pkt.G);
    UARTSend(' ');
    UARTSendAsHex(CC.RX_Pkt.B);
    UARTSend(' ');
    UARTSend(' ');
    UARTSendUint(CC.RX_Pkt.RSSI);
    UARTSend(' ');
    UARTSendAsHex(CC.RX_Pkt.LQI);
    UARTNewLine();
    #endif
}

// ============================= Light =========================================
void Light_t::Init() {
    LED_DDR  |=   (1<<RED_P)|(1<<GREEN_P)|(1<<BLUE_P);
    LED_PORT &= ~((1<<RED_P)|(1<<GREEN_P)|(1<<BLUE_P));
    TCCR0A = (1<<WGM01)|(1<<WGM00);
    TCCR0B = (0<<WGM02)|(0<<CS02)|(1<<CS01)|(0<<CS00);
    TCCR2A = (1<<WGM21)|(1<<WGM20);
    TCCR2B = (0<<WGM22)|(0<<CS22)|(1<<CS21)|(0<<CS20);
    Delay.Reset(&Timer);
    R.TCCRxA = &TCCR0A;
    G.TCCRxA = &TCCR0A;
    B.TCCRxA = &TCCR2A;
    R.OCRx   = &OCR0A;
    G.OCRx   = &OCR0B;
    B.OCRx   = &OCR2B;
    R.TccrOnValue  = ((1<<COM0A1)|(0<<COM0A0));
    G.TccrOnValue  = ((1<<COM0B1)|(0<<COM0B0));
    B.TccrOnValue  = ((1<<COM2B1)|(0<<COM2B0));
    R.TccrOffValue = (uint8_t)(~((1<<COM0A1)|(1<<COM0A0)));
    G.TccrOffValue = (uint8_t)(~((1<<COM0B1)|(1<<COM0B0)));
    B.TccrOffValue = (uint8_t)(~((1<<COM2B1)|(1<<COM2B0)));

    Indx = 0;
    SetTableColor();
    IsOn = true;
}

void Light_t::SetTableColor(void) {
    R.Desired = GetColor(Indx, 0);
    G.Desired = GetColor(Indx, 1);
    B.Desired = GetColor(Indx, 2);
    UARTSendUint(Indx);
    UARTSend(':');
    UARTSendAsHex(R.Desired);
    UARTSend(' ');
    UARTSendAsHex(G.Desired);
    UARTSend(' ');
    UARTSendAsHex(B.Desired);
    UARTNewLine();
}

void Light_t::Task() {
    if (BlinkState == BlinkOff) {
        if (Delay.Elapsed(&BlinkTimer, LED_BLINK_OFF_T)) {
            BlinkState = BlinkOn;
            AllOn();
        }
    }
    else {
        if (BlinkState == BlinkOn) {
            if (Delay.Elapsed(&BlinkTimer, LED_BLINK_ON_T)) {
                BlinkState = BlinkOff;
                AllOff();
                return;
            }
        }
        // Adjust colors
        if (Delay.Elapsed(&Timer, LED_STEP_T)) AllAdjust();
    }
}

void Channel_t::Adjust() {
    if (Desired < Current) {
        Current--;
        if (Current == 0) ATOMIC_BLOCK(ATOMIC_FORCEON) { *TCCRxA &= TccrOffValue; }
    }
    else if (Desired > Current) {
        if (Current == 0) ATOMIC_BLOCK(ATOMIC_FORCEON) { *TCCRxA |= TccrOnValue; }
        Current++;
    }
    ATOMIC_BLOCK(ATOMIC_FORCEON) { *OCRx = Current; }
}
