#include "leds_pca.h"

#ifndef _ROCK_TABLES_H
#define _ROCK_TABLES_H

// Defines
#define MAX_CHARGE_COUNT    3
#define FILENAME_LEN        13  // = 8+.+3+\0
#define ART_TYPES_COUNT     11  // = None + 10 mono, combo not used
#define FIELD_TYPES_COUNT   7

#define LCD_TEXT_FILENAME   "lcd_text.txt"

// Timings
#define ACTIVATED_TIME_DEFAULT   40000

// Types
typedef enum {
    atEmpty=0,
    atSlomo=1,
    atFlash=2,
    atGirya=3,
    atPetlya=4,
    atShpala=5,
    atRadX=6,
    atBlackJack=7,
    atPsiKleschi=8,
    atVyvert=9,
    atKusok=10,
    // Crosses
    atBlackJack_Shpala=11,
    atRadX_PsiKleschi_Petlya=12,
    atFlash_Petlya=13,
    atFlash_Slomo=14,
    atPsiKleschi_Flash=15,
    atPsiKleschi_BlackJack=16,
    atSlomo_Vyvert_Petlya=17,
    atShpala_PsiKleschi=18,
    atPsiKleschi_Vyvert_Petlya=19
} ArtType_t;
typedef enum {ftNone=0, ftHP=1, ftHM=2, ftEP=3, ftEM=4, ftCP=5, ftCM=6} FieldType_t;
typedef enum {fidCM=42, fidCP=43, fidHM=44, fidHP=45, fidEM=46, fidEP=47} FieldID_t;
typedef enum {actOne, actTwo, actThree} ActType_t;

// Field threscholds
#define THRESHOLD1          10
#define THRESHOLD2          20
#define THRESHOLD3          30
#define THRESHOLD_MAX       400
// Field type colors & delays
#define FIELD_BLINK_ON_TIME     126
#define FIELD_BLINK_OFF_TIME    3000
const Color_t FieldColors[FIELD_TYPES_COUNT] = {
        {0,   0,   0  },    // ftNone
        {255, 0,   0  },    // ftHP
        {27,  0,   255},    // ftHM
        {0,   225, 255},    // ftEP
        {207, 250, 0  },    // ftEM
        {180, 180, 90 },    // ftCP
        {255, 99,  0  },    // ftCM
};
// Artifact type colors, delays & sounds
const uint32_t ArtChargeRunDelays[] = {270, 360, 270, 180};  // Zero one is dummy, next are equivivalence to charge count
const Color_t ArtTypeColors[ART_TYPES_COUNT] = {
        {0,   0,   0  },    // atEmpty
        {0,   255, 0  },    // atSlomo
        {18,  215,  27},    // atFlash
        {27,  0,   255},    // atGirya
        {255, 99,  0  },    // atPetlya
        {207, 250, 0  },    // atShpala
        {255, 0,   0  },    // atRadX
        {180, 180, 90 },    // atBlackJack
        {0,   225, 255},    // atPsiKleschi, light blue
        {255, 0,   255},    // atVyvert
        {0,   0,   255},    // atKusok
};
const char ChargedSounds[ART_TYPES_COUNT][FILENAME_LEN] = {
    "", // atEmpty
    "", // atSlomo
    "", // atFlash
    "", // atGirya
    "", // atPetlya
    "", // atShpala
    "", // atRadX
    "", // atBlackJack
    "", // atPsiKleschi
    "", // atVyvert
    "", // atKusok
};
const char ActivationSounds[ART_TYPES_COUNT][FILENAME_LEN] = {
    "", // atEmpty
    "slo_actn.wav", // atSlomo
    "fla_actn.wav", // atFlash
    "gir_actn.wav", // atGirya
    "pet_actn.wav", // atPetlya
    "shp_actn.wav", // atShpala
    "rad_actn.wav", // atRadX
    "bla_actn.wav", // atBlackJack
    "psi_actn.wav", // atPsiKleschi
    "vyv_actn.wav", // atVyvert
    "kus_actn.wav", // atKusok
};
const char ActivitySounds[ART_TYPES_COUNT][FILENAME_LEN] = {
        "", // atEmpty
        "", // atSlomo
        "fla_rpt.wav", // atFlash
        "gir_rpt.wav", // atGirya
        "pet_rpt.wav", // atPetlya
        "", // atShpala
        "", // atRadX
        "", // atBlackJack
        "", // atPsiKleschi
        "", // atVyvert
        "", // atKusok
    };

const char ArtTypeStrings[ART_TYPES_COUNT][26] = {
        "",             // atEmpty
        "Slomo",        // atSlomo
        "Flash",        // atFlash
        "Girya",        // atGirya
        "Petlya",       // atPetlya
        "Shpala",       // atShpala
        "RadX",         // atRadX
        "BlackJack",    // atBlackJack
        "PsiKleschi",   // atPsiKleschi
        "Vyvert",       // atVyvert
        "Kusok",        // atKusok
};
const char ArtChargeStrings[4][8] = {
        "",
        "Charge1",
        "Charge2",
        "Charge3",
};
const char ArtCh1RndStrings[4][8] = {
        "",
        "Ch1Rnd1",
        "Ch1Rnd2",
        "Ch1Rnd3",
};
const char ArtCh3RndStrings[10][8] = {
        "",
        "Ch3Rnd1",
        "Ch3Rnd2",
        "Ch3Rnd3",
        "Ch3Rnd4",
        "Ch3Rnd5",
        "Ch3Rnd6",
        "Ch3Rnd7",
        "Ch3Rnd8",
        "Ch3Rnd9",
};


#endif
