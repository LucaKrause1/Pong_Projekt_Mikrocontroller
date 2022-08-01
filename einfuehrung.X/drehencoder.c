/* 
 * File:   drehencoder.c
 * Author: Luca Krause
 *
 * Created on 21. April 2022, 14:52
 */

#include "drehencoder.h"
#include "debug.h"
#include <avr/interrupt.h>

//noch zu handelnde Interrupts der Drehencoder
volatile signed int interruptCounterA = 0;
volatile signed int interruptCounterB = 0;

//Gibt die Werte der Drehencoder + deren Schalter zurück
#define GET_ENCODER_A ((PINB & 0b00000110)>>PB1)
#define GET_ENCODER_B ((PINB & 0b11000000)>>PB6)
#define GET_PUSH_A ((PINB & 0b00000001)>>PB0)
#define GET_PUSH_B ((PINB & 0b00001000)>>PB3)

//alte Zustände der Drehencoder + deren Schalter
static volatile char oldEncoderA = 0;
static volatile char oldEncoderB = 0;
static volatile unsigned char oldPushA = 0;
static volatile unsigned char oldPushB = 0;

//Zähler, wie lange ein Schläger schon/nicht mehr invertiert ist
static volatile char counterA = 0;
static volatile char counterB = 0;

//darf ein Schläger gerade umgedreht werden? 1 = true, 0 = false
static volatile char invertAChange = 1;
static volatile char invertBChange = 1;


//LUT der Drehencoder (alter und neuer Zustand als Eingabe)
static const signed char grayDecode[16] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};

/**
 * Startet die Pin-Change-Interrupts an den benötigten Pins
 */
static void startInterrupts(void) {
	PCICR |= (1<<PCIE0);
	PCMSK0 |= (1<<PCINT0) | (1<<PCINT1) | (1<<PCINT2) | (1<<PCINT3) | (1<<PCINT6) | (1<<PCINT7);
	sei();
}

/**
 * Startet den Timer, um den Timer für die Schlägerinvertierung zu aktivieren
 */
void startTimerPush(void) {    
    TCCR0B |= 1<<WGM02;// Timer im CTC-Mode mit OCR0A als Top
    TCCR0B |= 0b101; //Systemtakt durch 1024
    TIMSK0 |= 1<<OCIE0A;
    OCR0A = 255;
}

void initRotary(void) {
	//Input Pins
    ClearBit(DDRB, PCINT0);
	ClearBit(DDRB, PCINT1);
	ClearBit(DDRB, PCINT2);
    ClearBit(DDRB, PCINT3);
	//ClearMask(DDRB, PCINT4);
	ClearBit(DDRB, PCINT6);
	ClearBit(DDRB, PCINT7);
    oldEncoderA = GET_ENCODER_A;
    oldEncoderB = GET_ENCODER_B;
    oldPushA = GET_PUSH_A;
    oldPushB = GET_PUSH_B;
    startTimerPush();
	startInterrupts();
}

ISR(PCINT0_vect) {
    char encoderA = GET_ENCODER_A;
    char encoderB = GET_ENCODER_B;
    unsigned char indexA = (encoderA<<2)|(oldEncoderA);
    unsigned char indexB = (encoderB<<2)|(oldEncoderB);
    signed char resA = grayDecode[indexA];
    signed char resB = grayDecode[indexB];
    
    char pushA = GET_PUSH_A;
    char pushB = GET_PUSH_B;
    
    char A = pushA ^ oldPushA;
    char B = pushB ^ oldPushB;
    
    oldPushA = pushA;
    oldPushB = pushB;

    //darf der Schläger invertiert werden?
    if (A && invertAChange) {
        invertA = -1;
        invertAChange = 0;
    }
    if (B && invertBChange) {
        invertB = -1;
        invertBChange = 0;
    }
    
    interruptCounterB += resA;
    oldEncoderA = encoderA;
    interruptCounterA += resB;
    oldEncoderB = encoderB;
}

ISR(TIMER0_COMPA_vect) {
    //Zähler hochzählen wenn invertiert 
    //oder wenn gerade erst wieder in die richtige Richtung
    if (invertA == -1 || (!invertAChange && invertA == 1)) {
        counterA++;
    }
    if (invertB == -1 || (!invertBChange && invertB == 1)) {
        counterB++;
    }
    static const char max = 20;
    //zurückdrehen, aber weitere Drehung erstmal nicht erlauben
    if (!invertAChange && counterA == max) {
        if (invertA == 1) {
            invertAChange = 1;
        }
        invertA = 1;
        counterA = 0;
    }
    if (!invertBChange && counterB == max) {
        if (invertB == 1) {
            invertBChange = 1;
        }
        invertB = 1;
        counterB = 0;
    }
}
