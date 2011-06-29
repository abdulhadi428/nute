/*
 * functional
 *
 *  Created on: 24.06.2011
 *      Author: Kreyl
 */

#ifndef FUNCTIONAL_
#define FUNCTIONAL_

#include <inttypes.h>
#include "leds_pca.h"

// Defines
#define MAX_CHARGE_COUNT	3
// Field trescholds
#define THRESHOLD1		    100
#define THRESHOLD2		    200
#define THRESHOLD3          300
#define THRESHOLD_MAX       400

// Timings
#define FIELD_INDICATION_TIMEOUT    2000

// Field type colors & delays
#define FIELD_BLINK_ON_TIME     153
#define FIELD_BLINK_OFF_TIME    2007
const Color_t FieldColors[] = {
        {0,   0,   0  },    // ftNone
        {255, 0,   0  },    // ftHP
        {255, 255, 0  },    // ftHM
        {255, 0,   0  },    // ftEP
        {255, 0,   0  },    // ftEM
        {255, 0,   0  },    // ftCP
        {255, 0,   0  },    // ftCM
};
// Artifact type colors & delays
const uint32_t ArtChargeRunDelays[] = {702, 702, 405, 81};  // Zero one is dummy, next are equivivalence to charge count
const Color_t ArtTypeColors [] = {
        {0,   0,   0  },    // atEmpty
        {0,   0,   0  },    // atSlomo
        {0,   0,   0  },    // atFlash
        {255, 255, 0  },    // atGirya
        {0,   0,   0  },    // atPetlya
        {255, 255, 0  },    // atShpala
        {255, 0,   0  },    // atRadX
        {0,   0,   0  },    // atBlackJack
        {0,   0,   0  },    // atPsiKleschi
        {0,   0,   0  },    // atVyvert
        {0,   0,   255},    // atKusok
};

// Types
typedef enum {atEmpty=0, atSlomo=1, atFlash=2, atGirya=3, atPetlya=4, atShpala=5, atRadX=6, atBlackJack=7, atPsiKleschi=8, atVyvert=9, atKusok=10} ArtType_t;
typedef enum {ftNone=0, ftHP=1, ftHM=2, ftEP=3, ftEM=4, ftCP=5, ftCM=6} FieldType_t;

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

class Rock_t {
private:
    uint32_t Timer;
    void ChooseType(void);
    uint8_t MaxV(uint8_t a, uint8_t b) { return ((a > b)? a : b); }
    uint8_t MinV(uint8_t a, uint8_t b) { return ((a < b)? a : b); }
    // Indication
    void ShowFieldExistance(FieldType_t AFType);
    void ShowChargeCount(void);
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


extern Rock_t ERock;



#endif /* FUNCTIONAL_ */
