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
    int32_t Counter;
public:
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
    bool SoundPlayed, HaveActivitySound;
    uint8_t Rnd13, Rnd19;   // Random numbers
    void ChooseType(void);
    uint8_t MaxV(uint8_t a, uint8_t b) { return ((a > b)? a : b); }
    uint8_t MinV(uint8_t a, uint8_t b) { return ((a < b)? a : b); }
    // Indication
    void ShowChargeCount(void);
    void Activate(void);
    void DisplayText(void);
    void DecreaseCharge(void);
public:
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
