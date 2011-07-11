/*
 * File:   list_counter.h
 * Author: Laurelindo
 *
 * Created on 30.07.2010, 11:09
 */

#ifndef _LIST_COUNTER_H
#define	_LIST_COUNTER_H

// ================================= Sensor ====================================
#define SENSOR_DDR  DDRC
#define SENSOR_PORT PORTC
#define SENSOR_PIN  PINC
#define SENSOR_P    PC4

#define LIST_IS_IN()    bit_is_clear(SENSOR_PIN, SENSOR_P)

// =============================== Prototypes ==================================
void GeneralInit(void);

// Tasks
void TASK_Lister(void);

// Events
void EVENT_NewList(void);

#endif

