/* 
 * File:   tirno.h
 * Author: Laurelindo
 *
 * Created on 24.9.2010
 */

#ifndef _TIRNO_H
#define	_TIRNO_H

#include <avr/io.h>
#include <inttypes.h>
#include "lcd110x.h"
#include "lcd_font.h"

// ================================ List =======================================
#define LOCKET_COUNT    11
#define LOCKET_NAME_L   15
#define LOCKET_EE_ADDR  4
// =============================== General =====================================

// ================================== Timer ====================================
// Divider = 64
#define TCCR1B_ON   (1<<WGM13)|(1<<WGM12)|(0<<CS02)|(1<<CS01)|(1<<CS00)
#define TCCR1B_OFF  (1<<WGM13)|(1<<WGM12)|(0<<CS02)|(0<<CS01)|(0<<CS00)

#define CYC_TIMER_START()    TCCR1B = TCCR1B_ON
#define CYC_TIMER_STOP()     TCCR1B = TCCR1B_OFF

#define CYCLE_COUNT         4
//#define SUBCYCLE_DURATION   3960    // 251.7 ms
#define SUBCYCLE_DURATION   5040    // 322 ms

// =============================== Types =======================================
// Locket
struct Locket_t {
    uint8_t Addr;
    uint16_t Channel;
    char S[LOCKET_NAME_L];
};
struct Lockets_t {
    struct Locket_t L[LOCKET_COUNT];
    uint8_t Indx;
    uint8_t TopIndx;    // Indx of locket at top of screen
};

extern struct Lockets_t EL;
extern uint32_t EID;

// =============================== Prototypes ==================================
// General
void GeneralInit(void);
void eeReadLocket(uint8_t ID);
void eeWriteLocket(uint8_t ID);
//void CC_Call(void);

// Tasks
void CC_Task (void);

// Events
void EVENT_StartingSearch(void);
void EVENT_NewPacket(void);


#endif	/* _CALMA3C_H */

