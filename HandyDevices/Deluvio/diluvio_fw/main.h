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
#define PUMP_MAX_DURATION   3600

// Water sensor
#define WATER_SNS_DDR   DDRA
#define WATER_SNS_PORT  PORTA
#define WATER_SNS_PIN   PINA
#define WATER_SNS_P     PA1
#define WATER_PWR_P     PA0
#define WATER_OK()      bit_is_set(WATER_SNS_PIN, WATER_SNS_P)
#define WATER_SNS_OFF() WATER_SNS_PORT &= ~(1<<WATER_PWR_P)
#define WATER_SNS_ON()  WATER_SNS_PORT |=  (1<<WATER_PWR_P)

enum PumpDelayMode_t {ModeDays, ModeHours};
enum PumpState_t {PmpIdle, PmpMustPump, PmpBeeping, PmpPumping};
struct pump_t {
    bool Enabled;
    enum PumpDelayMode_t DelayMode;
    uint8_t Period;
    uint8_t PeriodLeft;
    uint8_t StartHour;
    uint8_t Duration;
    enum PumpState_t State;
    uint16_t Counter;
    uint8_t ID;
};

extern struct pump_t Pumps[PUMP_COUNT];
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

void EVENT_NewMinute(void);
void EVENT_NewHour(void);
void EVENT_NewDay(void);

#endif	/* MAIN_H */

