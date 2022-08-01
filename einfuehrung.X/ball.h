/* 
 * File:   ball.h
 * Author: Luca Krause
 * 
 * Dieses Header-File stellt die Interaktion 
 * mit einem alten und neuen Ball dar.
 * Enthalten ist außerdem ein Timer, mit dem 
 * der Ball aktualisiert wird.
 *
 * Created on 12. Mai 2022, 11:19
 */

#ifndef BALL_H
#define	BALL_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "utils.h"

//Ball und sein alter Zustand
extern struct BALL ball;
extern struct BALL oldBall;

/**
 * Interrupt-Servie-Routine für die Bewegung des Balls
 */
ISR(TIMER1_COMPA_vect);

#ifdef	__cplusplus
}
#endif

#endif	/* BALL_H */

