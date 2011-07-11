/* 
 * File:   list_counter.c
 * Author: Laurelindo
 *
 * Created on 30.07.2010, 11:09
 */
#include <avr/io.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include "list_counter.h"
#include "time_utils.h"
#include "common.h"
#include "matrix_lcd.h"
#include "ee_store_uint32.h"

// ============================= Global =========================================
uint32_t ECounter;
bool ListIn;

// ============================== General ======================================
int main(void) {
    GeneralInit();
    // Display initial count
    LCD_cls();
    LCD_PrintString_P(0, 0, PSTR("List count:"));
    ECounter = EE_ReadUint32();
    LCD_PrintUint32(0, 1, ECounter);

    while (1) {
        TASK_Lister();
        wdt_reset();    // Reset watchdog
    } // while
}

FORCE_INLINE void GeneralInit(void) {
    // Watchdog
    wdt_enable(WDTO_1S);
    // Sensor
    SENSOR_DDR  &= ~(1<<SENSOR_P);  // Sensor is input
    //SENSOR_PORT &= ~(1<<SENSOR_P);  // Pull-up is off
    SENSOR_PORT |= (1<<SENSOR_P);   // Pull-up is on
    ListIn = false;

    // Init LCD
    LCD_Init();
    // Display Ostranna info
    LCD_PrintString_P(4, 0, PSTR("Ostranna"));
    LCD_PrintString_P(0, 1, PSTR("ListCounter v1.0"));

    // Repeat to allow watchdog to reset
    _delay_ms(500);
    wdt_reset();
    _delay_ms(500);
    wdt_reset();
    _delay_ms(500);
    wdt_reset();
    _delay_ms(500);
    wdt_reset();

    // Check if EEPROM is erased
    EE_CheckIfErased();
}

// ============================== Tasks ========================================
FORCE_INLINE void TASK_Lister(void) {
    _delay_ms(500);
    if((!ListIn) && LIST_IS_IN()) {         // New list enters
        ListIn = true;
        EVENT_NewList();
    }
    else if(ListIn && (!LIST_IS_IN())) {    // List leaves
        ListIn = false;
    }
}

// ============================== Events =======================================
void EVENT_NewList(void) {
    // Show star to blink
    LCD_SetCursor(15, 0);
    LCD_WriteChar('*');
    // Increase counter, save it and print it
    ECounter++;
    EE_WriteUint32(ECounter);
    LCD_PrintString_P(0, 1, PSTR("                "));  // Clear lower string
    LCD_PrintUint32(0, 1, ECounter);
    // Hide star
    LCD_SetCursor(15, 0);
    LCD_WriteChar(' ');
}
