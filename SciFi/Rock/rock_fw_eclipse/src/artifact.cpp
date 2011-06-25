/*
 * artifact.cpp
 *
 *  Created on: 24.06.2011
 *      Author: Kreyl
 */
#include "artifact.h"
#include "delay_util.h"

Artifact_t EArt;

void Artifact_t::Task() {
    if (CurrendField != ftNone) {
        if (Delay.Elapsed(&Timer, CHARGE_INDICATION)) {
            CurrendField = ftNone;
            Leds.SetMode(lmFadeAllAwayAndStop);
        }
    }
}

void Artifact_t::Init() {
    Type = atEmpty;
    ChargeCount = 0;
    CurrendField = ftNone;
}

void Artifact_t::Charge(FieldType_t AFType) {
    //int8_t FCharges;
    switch (AFType) {
        case ftHP:
            // Decide if need to increase
            if ((Type == atBlackJack) || (Type == atPetlya)) return;
            H++;                            // Increase FieldPoints
            if (H.HasChanged) ChooseType(); // Decide which kind of artifact we have now
            ShowHP();                       // Indicate field existance
            break;

        case ftHM:
            // Decide if need to increase
            if ((Type == atBlackJack) || (Type == atPetlya)) return;
            H--;
            if (H.HasChanged) ChooseType(); // Decide which kind of artifact we have now
            break;

        case ftEP:
            // Decide if need to increase
            if ((Type == atBlackJack) || (Type == atPetlya)) return;
            E++;
            if (E.HasChanged) ChooseType(); // Decide which kind of artifact we have now
            break;

        case ftEM:
            // Decide if need to increase
            if ((Type == atBlackJack) || (Type == atPetlya)) return;
            E--;
            if (E.HasChanged) ChooseType(); // Decide which kind of artifact we have now
            break;

        case ftCP:
            // Decide if need to increase
            if ((Type != atEmpty) && (Type != atBlackJack) && (Type != atPetlya)) return;
            C++;
            if (C.HasChanged) ChooseType(); // Decide which kind of artifact we have now
            break;

        case ftCM:
            // Decide if need to increase
            if ((Type != atEmpty) && (Type != atBlackJack) && (Type != atPetlya)) return;
            C--;
            if (C.HasChanged) ChooseType(); // Decide which kind of artifact we have now
            break;
    } // switch
}

void Artifact_t::ChooseType() {
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
        ChargeCount = MaxV(H.Module(), E.Module());
    }
    else if ((H > 0) && (E < 0)) {
        Type = atKusok;
        ChargeCount = MaxV(H.Module(), E.Module());
    }
    else if ((H < 0) && (E > 0)) {
        Type = atVyvert;
        ChargeCount = MaxV(H.Module(), E.Module());
    }
    else if ((H < 0) && (E < 0)) {
        Type = atSlomo;
        ChargeCount = MaxV(H.Module(), E.Module());
    }
}

void Artifact_t::Activate() {

}

// ================================ Indication =================================
void Artifact_t::ShowHP() {
    Delay.Reset(&Timer);
    if(CurrendField == ftNone) {
        Leds.SetRunningColor(ChargeCount);
    } //
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
