/*
 * File:   main.h
 * Author: Kreyl Laurelindo
 *
 * Created on 29 Ноябрь 2010 г., 19:49
 */

#ifndef MAIN_H
#define	MAIN_H

#include "common.h"
#include <avr/io.h>
#include <stdbool.h>

// Power ok
#define PWROK_DDR   DDRA
#define PWROK_PORT  PORTA
#define PWROK_PIN   PINA
#define PWROK_P     PA3
#define POWER_OK()  bit_is_set(PWROK_PIN, PWROK_P)

// Pumps
#define PUMP_COUNT  4
#define PUMP_DDR    DDRA
#define PUMP_PORT   PORTA
#define PUMP1P      PA4
#define PUMP2P      PA5
#define PUMP3P      PA6
#define PUMP4P      PA7
// Max pump ON duration, s
#define PUMP_MAX_DURATION   252

// Water sensor
#define WATER_SNS_DDR    DDRA
#define WATER_SNS_PORT   PORTA
#define WATER_SNS_PIN    PINA
#define WATER_IN_P       PA1
#define WATER_R_P        PA0

#define WATER_IN_IS_HI() bit_is_set  (WATER_SNS_PIN, WATER_IN_P)
#define WATER_IN_IS_LO() bit_is_clear(WATER_SNS_PIN, WATER_IN_P)
#define WATER_R_LO()     WATER_SNS_PORT &= ~(1<<WATER_R_P)
#define WATER_R_HI()     WATER_SNS_PORT |=  (1<<WATER_R_P)

typedef enum {ModeDays, ModeHours} PumpPeriodMode_t;
typedef enum {PmpIdle, PmpMustPump, PmpBeeping, PmpPumping} PumpState_t;
typedef struct {
    bool Enabled;
    PumpPeriodMode_t PeriodMode;
    uint8_t Period;
    uint8_t PeriodLeft;
    uint8_t StartHour;
    uint8_t Duration;
    PumpState_t State;
    uint16_t Counter;
    uint8_t ID;
} pump_t;

extern pump_t Pumps[PUMP_COUNT];
extern bool IsPumping, PumpsSettingsChanged;

// ============================== Prototypes ===================================
void GeneralInit(void);

void PumpOn(uint8_t APump);
void PumpOffAll(void);
void Task_Pump(void);
void PumpsLoad(void);
void PumpsSave(void);

void CheckWater(void);
void CheckBattery(void);

void Task_Sleep(void);

#endif	/* MAIN_H */

