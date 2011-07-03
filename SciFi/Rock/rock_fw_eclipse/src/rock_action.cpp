/*
 * artifact.cpp
 *
 *  Created on: 24.06.2011
 *      Author: Kreyl
 */
#include "rock_action.h"
#include "delay_util.h"
#include "lcd110x.h"
#include "media.h"
#include "sensors.h"

#include "uart.h"

Rock_t ERock;

void Rock_t::Task() {
    if (IsActivated) {
        if (Delay.Elapsed(&Timer, ACTIVATED_TIMEOUT)) { // Showtime is over
            Lcd.Cls();          // Clear screen
            DecreaseCharge();
            ESns.Enable();
            IsActivated = false;
        }
        else {
            //if (ESnd.State == sndStopped) ESnd.Play(ArtTypeActiveSounds[Type]); // Replay active sound
        }
    } // if IsActivated
}

void Rock_t::Init() {
    Type = atEmpty;
    ChargeCount = 0;
}

void Rock_t::Charge(FieldType_t AFType) {
    switch (AFType) {
        case ftHP:
            // Decide if need to increase
            UART_PrintString("H+\r");
            if ((Type == atBlackJack) || (Type == atPetlya)) return;
            H++;                            // Increase FieldPoints
            if (H.HasChanged) ChooseType(); // Decide which kind of artifact we have now
            ShowFieldExistance(ftHP);       // Indicate field existance if artifact type allows this
            break;

        case ftHM:
            // Decide if need to increase
            if ((Type == atBlackJack) || (Type == atPetlya)) return;
            H--;
            if (H.HasChanged) ChooseType(); // Decide which kind of artifact we have now
            ShowFieldExistance(ftHM);       // Indicate field existance
            break;

        case ftEP:
            // Decide if need to increase
            if ((Type == atBlackJack) || (Type == atPetlya)) return;
            E++;
            if (E.HasChanged) ChooseType(); // Decide which kind of artifact we have now
            ShowFieldExistance(ftEP);       // Indicate field existance
            break;

        case ftEM:
            // Decide if need to increase
            if ((Type == atBlackJack) || (Type == atPetlya)) return;
            E--;
            if (E.HasChanged) ChooseType(); // Decide which kind of artifact we have now
            ShowFieldExistance(ftEM);       // Indicate field existance
            break;

        case ftCP:
            // Decide if need to increase
            if ((Type != atEmpty) && (Type != atBlackJack) && (Type != atPetlya)) return;
            C++;
            if (C.HasChanged) ChooseType(); // Decide which kind of artifact we have now
            ShowFieldExistance(ftCP);       // Indicate field existance
            break;

        case ftCM:
            // Decide if need to increase
            if ((Type != atEmpty) && (Type != atBlackJack) && (Type != atPetlya)) return;
            C--;
            if (C.HasChanged) ChooseType(); // Decide which kind of artifact we have now
            ShowFieldExistance(ftCM);       // Indicate field existance
            break;

        default: break;
    } // switch
}

void Rock_t::ChooseType() {
    if ((H == 0) && (E == 0) && (C == 0)) Type = atEmpty;
    // ==== C field ====
    if (C > 0) {
        Type = atBlackJack;
        ChargeCount = C.Module();
    }
    else if (C < 0) {
        Type = atPetlya;
        ChargeCount = C.Module();
    }
    // ==== H field ====
    else if ((H > 0) && (E == 0)) {
        Type = atRadX;
        ChargeCount = H.Module();
    }
    else if ((H < 0) && (E == 0)) {
        Type = atGirya;
        ChargeCount = H.Module();
    }
    // ==== E field ====
    else if ((E > 0) && (H == 0)) {
        Type = atPsiKleschi;
        ChargeCount = E.Module();
    }
    else if ((E < 0) && (H == 0)) {
        Type = atShpala;
        ChargeCount = E.Module();
    }
    // ==== Bipolar ====
    else if ((H > 0) && (E > 0)) {
        Type = atFlash;
        ChargeCount = MinV(H.Module(), E.Module());
    }
    else if ((H > 0) && (E < 0)) {
        Type = atKusok;
        ChargeCount = MinV(H.Module(), E.Module());
    }
    else if ((H < 0) && (E > 0)) {
        Type = atVyvert;
        ChargeCount = MinV(H.Module(), E.Module());
    }
    else if ((H < 0) && (E < 0)) {
        Type = atSlomo;
        ChargeCount = MinV(H.Module(), E.Module());
    }
    // Indicate current type
    ShowChargeCount();
}

void Rock_t::TryToActivate(ActType_t AActType) {
    switch (AActType) {
        case actOne: if ((Type == atVyvert) || (Type == atGirya) || (Type == atSlomo) || (Type == atPsiKleschi)) Activate(); break;
        case actTwo: if ((Type == atShpala) || (Type == atKusok) || (Type == atPetlya))                          Activate(); break;
        case actThree: if ((Type == atRadX) || (Type == atFlash) || (Type == atBlackJack))                       Activate(); break;
    }
}

void Rock_t::Activate() {
    if ((Type == atEmpty) || (ChargeCount == 0)) return;
    // Disable sensors
    ESns.Disable();
    // Light indication
    Leds.SetEqualAll(ArtTypeColors[Type]);
    // Sound indication
    if (ESnd.State != sndStopped) ESnd.Stop();
    else ESnd.Play(ArtTypeActiveSounds[Type]);
    // Display text
    DisplayText();
    // Reset delay to check if showtime is over
    Delay.Reset(&Timer);
    IsActivated = true;
}

void Rock_t::DecreaseCharge() {
//    switch (ChargeCount) {
//
//    } // switch


    ShowChargeCount();
}
void Rock_t::DisplayText() {
    //TODO
}

// ================================ Indication =================================
void Rock_t::ShowFieldExistance(FieldType_t AFType) {
    Delay.Reset(&Timer);        // Reset field dissapering timeout
    if(Leds.Mode != lmRunAndBlink) {
        Leds.BlinkOnTime  = FIELD_BLINK_ON_TIME;
        Leds.BlinkOffTime = FIELD_BLINK_OFF_TIME;
        Leds.SetRunningWithBlink();
    }
    Leds.BlinkColor = FieldColors[(uint8_t)AFType];
    //UART_PrintUint((uint8_t)AFType);
    UART_PrintUint((uint8_t)AFType);
}
void Rock_t::ShowChargeCount(void) {
    Leds.RunDelay = ArtChargeRunDelays[ChargeCount];
    Leds.RunColor = ArtTypeColors[Type];
    Leds.RunLedCount = ChargeCount;
    if(Leds.Mode != lmRunAndBlink) Leds.SetRunningWithBlink();
}


// =============================== FieldPoint_t ================================
void FieldPoint_t::operator ++(int) {
    int8_t OldValue = Value;
    Counter++;
    // Truncate value
    if (Counter > THRESHOLD_MAX) Counter = THRESHOLD_MAX;
    // Negative values
    if       (Counter <  -THRESHOLD2)                             Value = -3;
    else if ((Counter >= -THRESHOLD2) && (Counter < -THRESHOLD1)) Value = -2;
    else if ((Counter >= -THRESHOLD1) && (Counter <  0         )) Value = -1;
    // Positive values
    else if ((Counter >=  0         ) && (Counter <  THRESHOLD1)) Value = 0;
    else if ((Counter >=  THRESHOLD1) && (Counter <  THRESHOLD2)) Value = 1;
    else if ((Counter >=  THRESHOLD2) && (Counter <  THRESHOLD3)) Value = 2;
    else if  (Counter >   THRESHOLD3)                             Value = 3;
    // Check if changed
    HasChanged = (OldValue != Value);
}
void FieldPoint_t::operator --(int) {
    int8_t OldValue = Value;
    Counter--;
    // Truncate value
    if (Counter < -THRESHOLD_MAX) Counter = -THRESHOLD_MAX;
    // Negative values
    if       (Counter <= -THRESHOLD3)                              Value = -3;
    else if ((Counter >  -THRESHOLD3) && (Counter <= -THRESHOLD2)) Value = -2;
    else if ((Counter >  -THRESHOLD2) && (Counter <= -THRESHOLD1)) Value = -1;
    else if ((Counter >  -THRESHOLD1) && (Counter <=  0         )) Value =  0;
    // Positive values
    else if ((Counter >   0         ) && (Counter <=  THRESHOLD1)) Value = 1;
    else if ((Counter >   THRESHOLD1) && (Counter <=  THRESHOLD2)) Value = 2;
    else if  (Counter >   THRESHOLD2)                              Value = 3;
    // Check if changed
    HasChanged = (OldValue != Value);
}
