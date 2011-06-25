/*
 * functional
 *
 *  Created on: 24.06.2011
 *      Author: Kreyl
 */

#ifndef FUNCTIONAL_
#define FUNCTIONAL_

#include <inttypes.h>

// Defines
#define MAX_CHARGE_COUNT	3
// Field trescholds
#define THRESHOLD1		    100
#define THRESHOLD2		    200
#define THRESHOLD3          300
#define THRESHOLD_MAX       400

// Timings
#define CHARGE_INDICATION   2000

// Types
typedef enum {atEmpty = 1, atSlomo = 2, atGirya = 3, atPetlya = 4, atShpala = 5, atRadX=6, atBlackJack=7, atPsiKleschi=8, atVyvert=9, atKusok=10, atFlash=11} ArtType_t;
typedef enum {ftNone, ftHP, ftHM, ftEP, ftEM, ftCP, ftCM} FieldType_t;

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
};

class Artifact_t {
private:
    FieldType_t CurrendField;
    uint32_t Timer;
    int8_t FieldToChargePlus(int32_t *AField);
    void ChooseType(void);
    uint8_t MaxV(uint8_t a, uint8_t b) { return ((a > b)? a : b); }
    // Indication
    void ShowHP(void);
    void ShowHM(void);
public:
    // User use
	ArtType_t Type;
	uint8_t ChargeCount;
	// Inner use
	FieldPoint_t H, E, C;
	void Init(void);
	void Task(void);
	void Activate(void);
	void Charge(FieldType_t AFType);
};


extern Artifact_t EArt;



#endif /* FUNCTIONAL_ */
