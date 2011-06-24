/*
 * functional
 *
 *  Created on: 24.06.2011
 *      Author: Kreyl
 */

#ifndef FUNCTIONAL_
#define FUNCTIONAL_

// Defines
#define MAX_CHARGE_COUNT	3
// Field trescholds
#define THRESHOLD0		    0
#define THRESHOLD1		    100
#define THRESHOLD2		    200
#define THRESHOLD3          300

// Types
typedef enum {atEmpty=1, atSlomo=2, atGirya=3, atPetlya=4, atShpala=5, atRadX=6, atBlackJack=7, atPsiKleschi=8, atVyvert=9, atKusok=10, atFlash=11} ArtType_t;
typedef enum {ftHP, ftHM, ftEP, ftEM, ftCP, ftCM} FieldType_t;

class Artifact_t {
private:

public:
	ArtType_t Type;
	uint8_t ChargeCount;
	int32_t FieldH, FieldE, FieldC;
	void Init(void);
	void Activate(void);
	void Charge(FieldType_t AFType);
};


extern Artifact_t EArt;



#endif /* FUNCTIONAL_ */
