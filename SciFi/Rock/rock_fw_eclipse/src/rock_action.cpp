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
#include "kl_ini.h"
#include "lca.h"

#include "ff.h"
#include "diskio.h"

#include "uart.h"

Rock_t ERock;

void Rock_t::Task() {
    // Random generator
    if (++Rnd13 > 3) Rnd13 = 1;
    if (++Rnd19 > 9) Rnd19 = 1;
    if (++Rnd0_97 > 97) Rnd0_97 = 0;

    // ==== Handle activated artifact ====
    if (!IsActivated) return;
    // Check if Showtime is over
    if (Delay.Elapsed(&Timer, ActivatedTime)) {
        Lcd.Cls();          // Clear screen
        Leds.BacklightOff();
        DecreaseCharge();
        ESns.Enable();
        IsActivated = false;
    }
    else {
        if (ESnd.State == sndStopped) {
            if (!SoundPlayed) {
                ESnd.Play(ActivationSounds[Type]);  // Activation sound
                SoundPlayed = true;
            }
            else if (ActivitySoundRepeatCount != 0) {
                ESnd.Play(ActivitySounds[Type]);  // Sound during activity
                if (ActivitySoundRepeatCount != 0xFF) ActivitySoundRepeatCount--;
            }
        } // if stopped
    }
}

void Rock_t::Init() {
    Type = atEmpty;
    ChargeCount = 0;
    Rnd13 = 1;
    Rnd19 = 1;
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
    if ((H == 0) && (E == 0) && (C == 0)) {
        Type = atEmpty;
        ChargeCount = 0;
    }
    // ==== C field ====
    else if (C > 0) {
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
    ESns.Disable();                             // Disable sensors
    Leds.SetEqualAll(ArtTypeColors[Type]);      // Light indication
    if (ESnd.State != sndStopped) ESnd.Stop();  // Prepare sound indication
    // Variables initialization
    ActivatedTime = ACTIVATED_TIME_DEFAULT;
    SoundPlayed = false;
    ActivitySoundRepeatCount = 0;
    IsActivated = true;
    // Read string to display
    char *LastSymbol = ReadString(ArtTypeStrings[Type], ArtChargeStrings[ChargeCount], &Lcd.TextToShow[0], LCD_TEXT_SIZE_MAX, "lcd_text.txt");
    if (LastSymbol == NULL) return;
    // Do what needed depending on artifact type
    switch (Type) {
        // Monopolar
        case atRadX:
            if (ChargeCount == 2) ActivatedTime = 300000;   // == 5 min.
            break;
        case atPsiKleschi:
            if (ChargeCount == 3) {     // Add random string
                *LastSymbol++ = ' ';    // Add space at end
                ReadString(ArtTypeStrings[Type], ArtCh3RndStrings[Rnd19], LastSymbol, LCD_TEXT_SIZE_MAX, "lcd_text.txt");
            }
            break;
        case atPetlya:
            if ((ChargeCount == 3) || (ChargeCount == 2)) ActivitySoundRepeatCount = 0xFF;
            break;
        // Bipolar
        case atKusok:
            if (ChargeCount == 1) {     // Add random string
                *LastSymbol++ = ' ';    // Add space at end
                ReadString(ArtTypeStrings[Type], ArtCh1RndStrings[Rnd13], LastSymbol, LCD_TEXT_SIZE_MAX, "lcd_text.txt");
            }
            break;
        case atFlash:
            if (ChargeCount == 3) {
                AddRandURL(LastSymbol);
                ActivitySoundRepeatCount = 0xFF;
            }
            break;
        case atGirya:
            if (ChargeCount == 3) ActivitySoundRepeatCount = 12;
            break;
        case atPsiKleschi_Flash:
            AddRandURL(LastSymbol);
            break;
        case atPsiKleschi_BlackJack:
            AddRandURL(LastSymbol);
            break;

        default: break;
    } // switch
    Leds.BacklightOn();
    Lcd.PrintText();

    Delay.Reset(&Timer);    // Reset delay to check if showtime is over
}

void Rock_t::DecreaseCharge() {
    switch (Type) {
        // Monopolar
        case atGirya:
        case atRadX:
            H.DecreaseModule();
            break;
        case atPsiKleschi:
        case atShpala:
            E.DecreaseModule();
            break;
        case atBlackJack:
        case atPetlya:
            C.DecreaseModule();
            break;
        // Bipolar
        case atFlash:
        case atKusok:
        case atVyvert:
        case atSlomo:
            H.DecreaseModule();
            E.DecreaseModule();
            break;
        default: break;
    } // switch
    ChooseType();
}

void Rock_t::AddRandURL(char *ALastSymbol) {
    *ALastSymbol++ = ' ';    // Add space at end
    strcpy(ALastSymbol, URL_Template);
    ALastSymbol += strlen(URL_Template);
    // Make URL end
    uint32_t tmpu32 = idA[Rnd0_97];
    //tmpu32 = 0xDEADBEEF;  // DEBUG
    for(int8_t i=28; i>=0; i-=4)
        *ALastSymbol++ = UintToSmallHexChar(((uint8_t)(tmpu32 >> i)) & 0x0F);
    *ALastSymbol++ = '\'';
    *ALastSymbol = '\0';
}


// ================================ Indication =================================
void Rock_t::ShowFieldExistance(FieldType_t AFType) { // TODO: sound field existance
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
void FieldPoint_t::DecreaseModule() {
    if (Value > 0) Value--;
    if (Value < 0) Value++;
    // Set Counter depending on Value
    if      (Value == -3) Counter = -THRESHOLD3;
    else if (Value == -2) Counter = -THRESHOLD2;
    else if (Value == -1) Counter = -THRESHOLD1;
    else if (Value ==  1) Counter =  THRESHOLD1;
    else if (Value ==  2) Counter =  THRESHOLD2;
    else if (Value ==  3) Counter =  THRESHOLD3;
}
