/* 
 * File:   ball.c
 * Author: Luca Krause
 *
 * Created on 12. Mai 2022, 11:19
 */

#include "ball.h"

/**
 * Prüft, ob der Ball einen Schläger getroffen hat.
 * @param bat Schläger, der eventuell getroffen wird
 * @return @hitType - Position, an welcher der Schläger getroffen wurde
 */
hit_type isHit(struct BAT bat) {
    hit_type res = HIT_NO;
    
    unsigned char start = bat.Y;
    unsigned char end = start + BAT_SIZE;

    unsigned char firstMid = start + BAT_SIZE/3;
    unsigned char secondMid = start + 2*(BAT_SIZE/3);

    //HIT_NO ist schon initialisiert
    //Wo trifft der Scläger, wenn er getroffen hat?
    if (ball.Y >= start && ball.Y < firstMid) {
        res = HIT_UPPER;
        ball.movY = -1;
    } else if (ball.Y >= firstMid && ball.Y < secondMid) {
        res = HIT_MID;
        ball.movY = 0;
    } else if (ball.Y >= secondMid && ball.Y < end) {
        res = HIT_LOWER;
        ball.movY = 1;
    }
    
    //aktuellen Highscore hochsetzen
    if (res != HIT_NO) {
        highscoreNow++;
    }

    return res;
}

/**
 * Ball-Position neu berechnen
 */
void updateBall(void) {
    oldBall.X = ball.X;
    oldBall.Y = ball.Y;
    hit_type res = HIT_NO;
    
    //Treffer berechnen
    switch (ball.X) {
        case BAT_LEFT+2: {
            //Auf Höhe des linken Schlägers
                res = isHit(batLeft);
                if (!res) {
                    playerRight.points += 1;
                    reset = 1;
                } else {
                    ball.movX = 1;
                }
            }
            break;
        case BAT_RIGHT-2: {
            //Auf Höhe des rechten Schlägers
                res = isHit(batRight);
                if (!res) {
                    playerLeft.points += 1;
                    reset = 1;
                } else {
                    ball.movX = -1;
                }
            }
            break;
        default:
            break;
    }

    if (!reset) {
        //Abprallen an den Banden berechnen
    	switch (ball.Y) {
    	    case START_FIELD+1: {
    	        //oberes Spielfeldende
    	            ball.movY = 1;
    	        }
    	        break;
    	    case 62-1: {
    	        //unteres Spielfeldende
    	            ball.movY = -1;
    	        }
    	        break;
    	    default:
    	        break;
    	}
        //Bewegung durchführen
        //X-Richtung
    	ball.X += ball.movX;
        //Y-Richtung
    	ball.Y += ball.movY;
        
        if (OCR1A > END_SPEED) {
            OCR1A -= 2;      // Vergleichswert Timer wieder auf 0
        }
    }
}

ISR(TIMER1_COMPA_vect) {
    updateBall();
}
