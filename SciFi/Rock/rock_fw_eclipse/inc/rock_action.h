/*
 * Rock action logic
 *
 *  Created on: 24.06.2011
 *      Author: Kreyl
 */

#ifndef FUNCTIONAL_
#define FUNCTIONAL_

#include <inttypes.h>
#include "leds_pca.h"

#include "rock_tables.h"

class FieldPoint_t {
private:
public:
    int32_t Counter;
    int8_t Value;
    bool HasChanged;
    void operator++ (int);
    void operator-- (int);
    bool operator == (const int8_t AInt) { return (Value == AInt); }
    bool operator >  (const int8_t AInt) { return (Value >  AInt); }
    bool operator <  (const int8_t AInt) { return (Value <  AInt); }
    uint8_t Module(void) { return ((Value < 0)? (-Value) : Value); }
    void DecreaseModule(void);
};

class Rock_t {
private:
    uint32_t Timer, ActivatedTime;
    bool IsActivated;
    bool SoundPlayed;
    uint8_t ActivitySoundRepeatCount;
    uint8_t Rnd13, Rnd19, Rnd0_97;   // Random numbers
    uint8_t MaxV(uint8_t a, uint8_t b) { return ((a > b)? a : b); }
    uint8_t MinV(uint8_t a, uint8_t b) { return ((a < b)? a : b); }
    // Indication
    void ShowChargeCount(void);
    void DecreaseCharge(void);
    // Inner use
    char UintToSmallHexChar(uint8_t b) { return ((b<=0x09) ? (b+'0') : (b+'a'-10)); }
    void AddRandURL(char *ALastSymbol);
public:
    void ChooseType(void);
    void Activate(void);
    void ShowFieldExistance(FieldType_t AFType);
    // User use
	ArtType_t Type;
	uint8_t ChargeCount;
	void TryToActivate(ActType_t);
	// Inner use
	FieldPoint_t H, E, C;
	void Init(void);
	void Task(void);
	void Charge(FieldType_t AFType);
};


extern Rock_t ERock;

#endif /* FUNCTIONAL_ */
