#pragma once

typedef enum _EXPLOSION_EFFECT {
	BlastWave = 0,
	FireCloud = 1,
	Radiation = 2,
	MaxExplosionEffect = 3
} EXPLOSION_EFFECT;

typedef enum _DAMAGE_SEVERITY {
	RandomSelect = -3,
	Death = -2,
	None = -1,
	Graze = 0,				//царапина
	Light = 1,				//неопасное
	Insidious = 2,			//коварное
	Serious = 3,			//опасное
	Critical =4,			//смертельное
	MaxDamageSeverity = 5
} DAMAGE_SEVERITY;

extern const DAMAGE_SEVERITY	RandomSelectPerTenPercent[10];
extern const char* ExplosionDesc[MaxExplosionEffect][MaxDamageSeverity];
