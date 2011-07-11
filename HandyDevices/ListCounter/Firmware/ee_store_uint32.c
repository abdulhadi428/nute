#include <avr/eeprom.h>
#include "ee_store_uint32.h"

void EE_CheckIfErased(void) {
    struct EE_Cell_t EECell;
    eeprom_read_block(&EECell, (uint8_t*)(EE_START_ADDR), EE_CELL_SIZE);
    if(
        (EECell.Counter == 0xFF) &&
        (EECell.Number == 0xFFFFFFFF)
    ) {
        // Write initial values
        EECell.Counter = 0;
        EECell.Number = 0;
        eeprom_write_block(&EECell, (uint8_t*)EE_START_ADDR, EE_CELL_SIZE);
    }
}

uint32_t EE_ReadUint32(void) {
    uint16_t Addr;
    struct EE_Cell_t EECell;
    // Find address of last written value
    Addr = EE_FindCurrentAddr();
    // Read cell
    eeprom_read_block(&EECell, (uint8_t*)(Addr), EE_CELL_SIZE);

    return EECell.Number;
}

void EE_WriteUint32(uint32_t ANumber) {
    uint16_t Addr;
    struct EE_Cell_t EECell;
    // Find address of last written value
    Addr = EE_FindCurrentAddr();
    // Read cell
    eeprom_read_block(&EECell, (uint8_t*)(Addr), EE_CELL_SIZE);
    // Circle buffer
    Addr += EE_CELL_SIZE;   // Next address
    if (Addr == (EE_CELL_SIZE*EE_BUF_SIZE)) Addr = 0;
    // Setup cell
    EECell.Number = ANumber;
    EECell.Counter++;
    // Write cell, payload data first: this will cancel data read in case of write failure
    eeprom_write_block(&EECell.Number, (uint8_t*)(Addr+1), EE_CELL_SIZE-1);
    // Write counter
    eeprom_write_byte((uint8_t*)(Addr), EECell.Counter);
}

uint16_t EE_FindCurrentAddr(void) {
    uint8_t Current, Previous;      // Two counter values
    uint16_t Addr = EE_START_ADDR;  // Address of needed block
    // Read first counter value
    Previous = eeprom_read_byte((uint8_t*)EE_START_ADDR);
    // Iterate buffer until non-equal values is found
    for (uint8_t i = 0; i < EE_BUF_SIZE; i++) {
        Addr += EE_CELL_SIZE;
        Current = eeprom_read_byte((uint8_t*)(Addr));
        Previous++;
        if (Current != Previous) break; 
    }
    return (Addr - EE_CELL_SIZE);   // return address of last written value
}
