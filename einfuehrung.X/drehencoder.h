/* 
 * File:   drehencoder.h
 * Author: Luca Krause
 * 
 * Dieses Header-File stellt die Kommunikation 
 * zwei Drehencodern dar.
 *
 * Created on 21. April 2022, 14:52
 */

#ifndef DREHENCODER_H
#define	DREHENCODER_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include "utils.h"

enum state {NO_ROTATION, CLOCKWISE, COUNTERCLOCKWISE};

//noch zu handelnde Interrupts der Drehencoder
extern volatile signed int interruptCounterA;
extern volatile signed int interruptCounterB;

/**
 * Initialisiert die Drehencoder
 */
void initRotary(void);

/**
 * Bei einem Hardware-Interrupt durch einen Drehencoder 
 * entsteht eine Bitkombination, bei der beide Bits unterschiedlich sind,
 * woraus die Drehrichtung erkannt werden kann.
 */
ISR(PCINT0_vect);

/**
 * Interrupt-Servie-Routine fuer die Invertierung der Schläger
 */
ISR(TIMER0_COMPA_vect);

#endif	/* DREHENCODER_H */

