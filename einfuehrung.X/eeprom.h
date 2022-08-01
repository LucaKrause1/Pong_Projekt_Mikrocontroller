/* 
 * File:   eeprom.h
 * Author: Luca Krause
 * 
 * Dieses Header-File stellt die Kommunikation 
 * mit dem internen Speicher dar.
 *
 * Created on 12. Mai 2022, 14:19
 */

#ifndef EEPROM_H
#define	EEPROM_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "utils.h"

/**
 * Schreibt einen 16-Bit-Wert(word) in den Speicher
 * @param p Adresse
 * @param value Wert
 */
void writeWord(uint16_t* p, uint16_t value);

/**
 * Liest einen 16-Bit-Wert(word) aus dem Speicher
 * @param p Adresse
 * @return Wert
 */
uint16_t readWord(uint16_t* p);

#ifdef	__cplusplus
}
#endif

#endif	/* EEPROM_H */

