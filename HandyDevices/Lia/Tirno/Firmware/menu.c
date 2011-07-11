#include "menu.h"
#include "lcd110x.h"
#include "tirno.h"
#include <util/delay.h>
#include <avr/pgmspace.h>

// =============================== Defines =====================================
#define LIST_INDENT     2  // Indent of list

#define OPTIONS_COUNT   2
#define OPTIONS_TOP     2

// ============================= Global variables ==============================
prog_char Option0[] = "Переименовать";
prog_char Option1[] = "Состыковать  ";
prog_char *EOptions[] = {Option0, Option1};

prog_char FLLetters[6][2] = {
    {'а', 'п'},
    {'р', 'я'},
    {'a', 'p'},
    {'q', 0x80},
    {'0', '?'},
    {' ', '/'},
};

struct {
    uint8_t x, y, Letter;
} ECursor;

uint8_t EIndx;      // Options Indx
uint8_t CharIndx;   // Indx of choosen Char in string edited
enum State_t EState;

// ========================== Implementation ===================================
// ============================ States =========================================
void SetState(enum State_t AState) {
    LCD_Clear();
    EState = AState;
    switch(AState) {
        case StateList:
            DisplayList(0); // Display list of lockets
            break;

        case StateSearch:
            LCD_PrintString(0, 0, EL.L[EL.Indx].S, false);  // Print current locket name
            LCD_PrintString_P(0, 2, PSTR("dBm:"), false);
            LCD_PrintString_P(0, 7, PSTR("Список"), true);
            LCD_DrawGauge();
            EVENT_StartingSearch();
            break;

        case StateOptions:
            LCD_PrintString(0, 0, EL.L[EL.Indx].S, false);  // Print current locket name
            // Display pseudo-buttons
            LCD_PrintString_P(0,  7, PSTR("  Ok  "), true);
            LCD_PrintString_P(10, 7, PSTR("Отмена"), true);
            // Display options
            LCD_PrintString_P(0, OPTIONS_TOP,   EOptions[0], true);
            LCD_PrintString_P(0, OPTIONS_TOP+1, EOptions[1], false);
            EIndx = 0;  // Set current temporary indx
            break;

        case StateRename_ChoosePlace:
            LCD_GotoXY(0, 0);
            // Print current locket name with first letter highlighted
            LCD_DrawChar(EL.L[EL.Indx].S[0], true);
            LCD_PrintString(1, 0, &EL.L[EL.Indx].S[1], false);
            // Display pseudo-buttons
            LCD_PrintString_P(0,  7, PSTR("Изменить"), true);
            LCD_PrintString_P(10, 7, PSTR("Готово"), true);
            EIndx = 0;
            CharIndx = 0;
            break;

        case StatePair:
            LCD_PrintString(0, 0, EL.L[EL.Indx].S, false);  // Print current locket name
            LCD_PrintString_P(0, 1, PSTR("Стыковка"), false);
            // Display pseudo-buttons
            LCD_PrintString_P(0,  7, PSTR("  Ok  "), true);
            LCD_PrintString_P(10, 7, PSTR("Отмена"), true);
            break;

        default: break;
    } // switch
}

// ============================== Events =======================================
void EVENT_KeyUp(void) {
    switch(EState) {
        case StateList: // Move down by the list
            if(EL.Indx > 0) EL.Indx--;
            if(EL.Indx < EL.TopIndx) {    // Top of screen, draw previous items
                LCD_Clear();
                DisplayList(EL.Indx-6);
            }
            else {
                LCD_PrintString(LIST_INDENT, (EL.Indx - EL.TopIndx +1), EL.L[EL.Indx+1].S, false);   // Unhighlight previous element
                LCD_PrintString(LIST_INDENT,  EL.Indx - EL.TopIndx    , EL.L[EL.Indx  ].S, true);    // Highlight current element
            }
            break;

        case StateOptions:  // Move down by the list
            if(EIndx > 0) EIndx--;
            LCD_PrintString_P(0, (OPTIONS_TOP + EIndx+1), EOptions[EIndx+1], false);  // Unhighlight previous element
            LCD_PrintString_P(0, (OPTIONS_TOP + EIndx  ), EOptions[EIndx  ], true);   // Highlight current element
            break;

        case StateRename_ChoosePlace:
            if(CharIndx > 0) {
                CharIndx--;
                LCD_GotoXYstr(CharIndx, 0);
                LCD_DrawChar(EL.L[EL.Indx].S[CharIndx], true);  // Highlight current char
                LCD_DrawChar(EL.L[EL.Indx].S[CharIndx+1], false); // Unhighlight previous char
            }
            break;

        case StateRename_ChooseLetter:  // Move up
            if(ECursor.y > 1) {
                LCD_GotoXYstr(ECursor.x, ECursor.y);
                LCD_DrawChar(ECursor.Letter, false);    // Unhighlight letter
                ECursor.y--;
                ECursor.Letter = GetLetter();           // Get new letter
                LCD_GotoXYstr(ECursor.x, ECursor.y);
                LCD_DrawChar(ECursor.Letter, true);     // Highlight letter
            }
            break;

        default: break;
    } // switch
}
void EVENT_KeyDown(void) {
    switch(EState) {
        case StateList: // Move down by the list
            if(EL.Indx < LOCKET_COUNT-1) EL.Indx++;
            // Handle more than 7 lockets
            if(EL.Indx > (EL.TopIndx + 6)) {    // Bottom of screen, draw next items
                LCD_Clear();
                DisplayList(EL.Indx);
            }
            else {
                LCD_PrintString(LIST_INDENT, (EL.Indx - EL.TopIndx - 1), EL.L[EL.Indx-1].S, false);  // Unhighlight previous element
                LCD_PrintString(LIST_INDENT,  EL.Indx - EL.TopIndx     , EL.L[EL.Indx  ].S, true);   // Highlight current element
            }
            break;

        case StateOptions:  // Move down by the list
            if(EIndx < OPTIONS_COUNT-1) EIndx++;
            LCD_PrintString_P(0, (OPTIONS_TOP + EIndx-1), EOptions[EIndx-1], false);  // Unhighlight previous element
            LCD_PrintString_P(0, (OPTIONS_TOP + EIndx  ), EOptions[EIndx  ], true);   // Highlight current element
            break;

        case StateRename_ChoosePlace:
            if(CharIndx < LOCKET_NAME_L-1) {
                LCD_GotoXYstr(CharIndx, 0);
                LCD_DrawChar(EL.L[EL.Indx].S[CharIndx], false); // Unhighlight previous char
                CharIndx++;
                LCD_DrawChar(EL.L[EL.Indx].S[CharIndx], true); // Highlight current char
            }
            break;

        case StateRename_ChooseLetter:  // Move down
            if(ECursor.y < LCD_STR_HEIGHT-2) {
                LCD_GotoXYstr(ECursor.x, ECursor.y);
                LCD_DrawChar(ECursor.Letter, false);    // Unhighlight letter
                ECursor.y++;
                ECursor.Letter = GetLetter();           // Get new letter
                LCD_GotoXYstr(ECursor.x, ECursor.y);
                LCD_DrawChar(ECursor.Letter, true);     // Highlight letter
            }
            break;

        default: break;
    } // switch
}
void EVENT_KeyLeft(void) {
    switch(EState) {
        case StateList:
            // Blink button
            LCD_PrintString_P(0, 7, PSTR("     "), false);
            _delay_ms(150);
            LCD_PrintString_P(0, 7, PSTR("Поиск"), true);
            _delay_ms(200);
            SetState(StateSearch);
            break;

        case StateSearch:
            // Blink button
            LCD_PrintString_P(0, 7, PSTR("      "), false);
            _delay_ms(150);
            LCD_PrintString_P(0, 7, PSTR("Список"), true);
            _delay_ms(200);
            SetState(StateList);
            break;

        case StateOptions:
            // Blink button
            LCD_PrintString_P(0, 7, PSTR("      "), false);
            _delay_ms(150);
            LCD_PrintString_P(0, 7, PSTR("  Ok  "), true);
            _delay_ms(200);
            // Set state depending on option
            if(EIndx == 0) SetState(StateRename_ChoosePlace);
            else           SetState(StatePair);
            break;

        case StateRename_ChoosePlace:
            // Setup buttons
            LCD_PrintString_P(0, 7, PSTR("        "), false);
            LCD_PrintString_P(10, 7, PSTR("  ->  "), true);
            _delay_ms(150);
            LCD_PrintString_P(0, 7, PSTR("Выбрать"), true);
            EState = StateRename_ChooseLetter;
            // Get choosen letter
            uint8_t Choosen = EL.L[EL.Indx].S[CharIndx];
            if((Choosen >= 'A') && (Choosen <= 'Z')) Choosen += 'a'-'A';    // Change to uppercase
            if((Choosen >= 'А') && (Choosen <= 'Я')) Choosen += 'а'-'А';    // Change to uppercase
            // Display alphabet
            uint8_t cMax, cMin;
            for(uint8_t r=0; r<=5; r++) {   // Iterate rows
            //uint8_t r = 0;
                LCD_GotoXY(0, r+1);
                cMax = pgm_read_byte(&FLLetters[r][1]);
                cMin = pgm_read_byte(&FLLetters[r][0]);
                for(uint16_t c = cMin; c <= cMax; c++)
                    if(c == Choosen) {
                        LCD_DrawChar(c, true);
                        ECursor.x = c-cMin;
                        ECursor.y = r+1;
                        ECursor.Letter = c;
                    }
                    else LCD_DrawChar(c, false);
            }// r
            break;

        case StateRename_ChooseLetter:  // Choice is done
            // Blink button
            LCD_PrintString_P(0, 7, PSTR("        "), false);
            LCD_PrintString_P(10, 7, PSTR("Готово"), true);
            _delay_ms(50);
            // Remove letters
            for(uint8_t i=1; i<=6; i++) LCD_PrintString_P(0, i, PSTR("                "), false);
            LCD_PrintString_P(0, 7, PSTR("Изменить"), true);
            EState = StateRename_ChoosePlace;
            // Replace letter
            EL.L[EL.Indx].S[CharIndx] = ECursor.Letter;
            if(CharIndx == 0) { // Change to uppercase
                if((ECursor.Letter >= 'а') && (ECursor.Letter <= 'я')) EL.L[EL.Indx].S[CharIndx] = ECursor.Letter - ('a'-'A');
                if((ECursor.Letter >= 'a') && (ECursor.Letter <= 'z')) EL.L[EL.Indx].S[CharIndx] = ECursor.Letter - ('a'-'A');
            }
            // Move one char right
            LCD_GotoXYstr(CharIndx, 0);
            if(CharIndx < LOCKET_NAME_L-1) {
                LCD_DrawChar(EL.L[EL.Indx].S[CharIndx], false);
                CharIndx++;
                LCD_DrawChar(EL.L[EL.Indx].S[CharIndx], true);
            }
            else LCD_DrawChar(EL.L[EL.Indx].S[CharIndx], true);
            break;

            case StatePair:

                break;

        default: break;
    } // switch
}
void EVENT_KeyRight(void) {
    switch(EState) {
        case StateList:
            // Blink button
            LCD_PrintString_P(11, 7, PSTR("     "), false);
            _delay_ms(150);
            LCD_PrintString_P(11, 7, PSTR("Опции"), true);
            _delay_ms(200);
            // Display current item options
            SetState(StateOptions);
            break;

        case StateOptions:
        case StatePair:
        case StateSearch:
            // Blink button
            LCD_PrintString_P(10, 7, PSTR("      "), false);
            _delay_ms(150);
            LCD_PrintString_P(10, 7, PSTR("Отмена"), true);
            _delay_ms(200);
            // Return to list
            SetState(StateList);
            break;

        case StateRename_ChoosePlace:
            // Blink button
            LCD_PrintString_P(10, 7, PSTR("      "), false);
            _delay_ms(150);
            LCD_PrintString_P(10, 7, PSTR("Готово"), true);
            _delay_ms(200);
            eeWriteLocket(EL.Indx); // Save to EEPROM
            // Return to list
            SetState(StateList);
            break;

        case StateRename_ChooseLetter:              // Move cursor right
            LCD_GotoXYstr(ECursor.x, ECursor.y);
            LCD_DrawChar(ECursor.Letter, false);    // Unhighlight letter
            if(ECursor.x == LCD_STR_WIDTH-1) ECursor.x = 0;
            else ECursor.x++;
            ECursor.Letter = GetLetter();           // Get new letter
            LCD_GotoXYstr(ECursor.x, ECursor.y);
            LCD_DrawChar(ECursor.Letter, true);     // Highlight letter
            break;

        default: break;
    } // switch
}

// ============================= Inner use =====================================
void DisplayList(uint8_t StartElement) {
    uint8_t i = StartElement;
    EL.TopIndx = StartElement;
    for(uint8_t y=0; y<LCD_STR_HEIGHT-1; y++) { // -1, because we need place for menu
        if(i >= LOCKET_COUNT) break;
        LCD_PrintUint(0, y, i+1);
        LCD_PrintString(LIST_INDENT, y, EL.L[i].S, (i==EL.Indx));
        i++;
    }
    // Display pseudo-buttons
    LCD_PrintString_P(0, 7, PSTR("Поиск"), true);
    LCD_PrintString_P(11, 7, PSTR("Опции"), true);
}

uint8_t GetLetter(void) {
    return (ECursor.x + pgm_read_byte(&FLLetters[ECursor.y-1][0]));
}
