/* 
 * File:   utils.h
 * Author: Luca Krause
 * 
 * Hier sind alle nützlichen Dinge zu finden, die in 
 * vielen der anderen files benötigt werden.
 *
 * Created on 22. April 2022, 10:05
 */

#ifndef UTILS_H
#define	UTILS_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

#define F_CPU 1000000UL

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h>

//Zum Setzen und Löschen von Bits in einem Register
//Port = Register, Bit = zu setzendes Bit
#define SetBit(PORT, BIT) (PORT |= (1<<BIT)); 
#define ClearBit(PORT, BIT) (PORT &= ~(1<<BIT)); 

#define SCREEN_SIZE_X 128
#define SCREEN_SIZE_Y 64
//Breite eines Chips des Displays
#define CHIP_WIDTH 64

//y-Wert, wo das Spielfeld anfängt
#define START_FIELD 8

//FONT-Größe
#define FONT_SIZE 6

//X-Schlägerpositionen auf dem Spielfeld
#define BAT_LEFT 1
#define BAT_RIGHT 126

//Zeichen "0" im Ascii-Code
#define NUMBER_START_ASCII 48

//Start-Geschwindigkeit des Balls
#define START_SPEED 5000
//Maximalgeschwindigkeit des Balls
#define END_SPEED 2000
//Schrittgröße der Schläger bei Bewegen der Encoder
//1 = 2 Schritte, 2 = 1 Schritt
#define ENCODER_DIVISOR 2

//Schlägergröße
#define BAT_SIZE 9

//Hat ein Spieler einen Punkt erzielt?
extern char reset;

//beide Schläger
extern struct BAT batLeft;
extern struct BAT batRight;

//Beide Spieler
extern struct PLAYER playerLeft;
extern struct PLAYER playerRight;

//Highscores
extern uint16_t highscore;
extern uint16_t highscoreNow;

//Adresse des Highscores
extern uint16_t* add;

//Invertierung der Drehencoder
extern signed char invertA;
extern signed char invertB;

//Wie wurde ein Schläger getroffen?
typedef enum {
    //Kein Treffer
    HIT_NO = 0, 
    //Am oberen Ende getroffen
    HIT_UPPER,
    //In der Mitte getroffen
    HIT_MID,
    //Am unteren Ende getroffen
    HIT_LOWER
}hit_type;

//Ball
struct BALL {
    unsigned char X;
    unsigned char Y;
    char movX;
    char movY;
};

//Schläger
struct BAT {
    unsigned char X;
    unsigned char Y;
};

//Spieler
struct PLAYER {
    //unsigned int ist hierbei 16 Bit breit
    unsigned int points;
};

#endif	/* UTILS_H */