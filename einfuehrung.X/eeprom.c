/* 
 * File:   eeprom.c
 * Author: Luca Krause
 *
 * Created on 12. Mai 2022, 14:19
 */

#include "eeprom.h"

void writeWord(uint16_t* p, uint16_t value) {
    eeprom_write_word(p, value);
}

uint16_t readWord(uint16_t* p) {
    return eeprom_read_word(p);
}
