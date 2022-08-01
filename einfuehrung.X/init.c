/* 
 * File:   init.c
 * Author: Luca Krause
 *
 * Created on 12. Mai 2022, 11:30
 */

#include "init.h"

/**
 * Initialisiert die Ball-Interrupts, damit sich der Ball bewegt
 */
void initBallInterrupts(void) {
    TCCR1B |= 1<<WGM12;// Timer im CTC-Mode mit OCR1A als Top
    TCCR1B |= 2;        // Systemtakt durch 8
    TIMSK1 |= 1<<OCIE1A; // Timer/Counter1 Output Compare Match A Interrupt Enable
    OCR1A = START_SPEED;      // Vergleichswert Timer wieder zurücksetzen
    
    sei();  //Interrupts erlauben
}

void initBall(void) {
    ball.X = 64;
    ball.Y = 32;
    ball.movX = 1;
    ball.movY = -1;
    oldBall.X = ball.X;
    oldBall.Y = ball.Y;
    oldBall.movX = ball.movX;
    oldBall.movY = ball.movY;
    initBallInterrupts();
    showBall(ball);
}

/**
 * Initialisiert beide Schläger
 */
void initBats(void) {
    batLeft.X = 1;
    batLeft.Y = 32;
    batRight.X = 126;
    batRight.Y = 32;
    showBats(batLeft, batRight);
}

/**
 * Initialisiert die Punkte
 */
void initPoints(void) {
    playerLeft.points = 0;
    playerRight.points = 0;
    
    reloadPoints();
}

/**
 * Initialisiert den Highscore
 */
void initHighscore(void) {
    highscore = readWord(add);
}

void setup(void) {
    //LCD-Display
    _delay_ms(30);
    //immer an lassen, damit das Display nicht immer an/aus geht!
    init_LCD();
    _delay_ms(30);
    LCDClear();
    LCDLine();

    //Drehencoder
    initRotary();

    //Ball
    initBall();

    //Schläger
    initBats();

    //Punkte/Spieler
    initPoints();
    initHighscore();
    reloadPoints();
}
