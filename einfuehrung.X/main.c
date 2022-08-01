/* 
 * File:   main.c
 * Author: Luca Krause
 * 
 * main-File:
 * Hier wird der gesamte Spielablauf des 
 * Spiels PONG durchgeführt und initialisiert.
 *
 * Created on 12. April 2022, 11:12
 */
#include "utils.h"
#include "display.h"
#include "debug.h"
#include "drehencoder.h"
#include "ball.h"
#include "init.h"
#include "eeprom.h"
#include <util/delay.h>

//Ball und sein alter Zustand
struct BALL ball;
struct BALL oldBall;

//beide Schläger
struct BAT batLeft = {};
struct BAT batRight = {};

//Beide Spieler
struct PLAYER playerLeft;
struct PLAYER playerRight;

//Highscores
uint16_t highscore = 0;
uint16_t highscoreNow = 0;
//Adresse des Highscores
uint16_t* add = (uint16_t*)0x0000;

//Invertierung der Drehencoder
signed char invertA = 1;
signed char invertB = 1;

//Hat ein Spieler einen Punkt erzielt?
char reset = 0;

//Setzt den Ball auf seine Startgeschwindigkeit
void ballInterrupts(void) {
    OCR1A = START_SPEED;      // Vergleichswert Timer wieder auf 0
    sei();  //Interrupts erlauben
}

/*
 * Setzt einen übergebenen Schläger 
 * und begrenzt seine Y-Position auf vordefinierte Werte
 * params:
 *      bat - Schläger, der bewegt wird
 */
static void clipBat(struct BAT *bat, signed int move) {
    signed char res = bat->Y + move;
    if (res < START_FIELD) {
        res = START_FIELD;
    } else if (res > SCREEN_SIZE_Y - BAT_SIZE) {
        res = SCREEN_SIZE_Y - BAT_SIZE;
    }
    bat->Y = (char)res;
}

/**
 * Lädt beide Schläger neu und setzt deren Drehencoder-Zähler zurück
 */
void updateBats(void) {
    //Für konsistente Daten Interrupts unterbinden
    cli();
    clipBat(&batLeft, interruptCounterA/ENCODER_DIVISOR * invertA);
    clipBat(&batRight, interruptCounterB/ENCODER_DIVISOR * invertB);
    interruptCounterA = interruptCounterA%ENCODER_DIVISOR;
    interruptCounterB = interruptCounterB%ENCODER_DIVISOR;
    sei();
    showBats(batLeft, batRight);
}

/*
 * Setzt das Spielfeld nach jedem erspielten Punkt zurück.
 * Die Schläger bleiben auf der alten Position.
 */
void resetAfterPoint(void) {
    LCDClear();
    LCDLine();
    initBall();
    
    showBats(batLeft, batRight);
    reloadPoints();
    reset = 0;
}

/*
 * Pong - Hauptmethode
 * Hier werden alle Methoden ausgeführt
 */
int main(void) {
    //Setzen des Initialwertes auf dem EEProm
    //writeWord(add, (uint16_t) 0);
    
    //Initialisieren
    setup();
    
    //Hauptschleife
    while(1) {
        //Schläger aktualisieren
        updateBats();
        //Ball auf dem Spielfeld aktualisieren
        deleteBall(oldBall);
        showBall(ball);
        
        if (reset) {
            //Ein Spieler hat einen Punkt erzielt
            highscore = readWord(add);
            //Highscore im Speicher neu setzen, wenn er kleiner ist
            if (highscore < highscoreNow) {
                writeWord(add, (uint16_t)highscoreNow);
                highscore = highscoreNow;
            }
            highscoreNow = 0;
            resetAfterPoint();
        }
        //Punkte neu laden für den aktuellen Highscore
        reloadPoints();
    }
    return (EXIT_SUCCESS);
}