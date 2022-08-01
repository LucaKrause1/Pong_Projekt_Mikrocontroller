/* 
 * File:   init.h
 * Author: Luca Krause
 *
 * Dieses Header-File wird zum Initialisieren 
 * der Komponenten und des internen Status genutzt.
 * 
 * Created on 12. Mai 2022, 11:30
 */

#ifndef INIT_H
#define	INIT_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "utils.h"
#include "ball.h"
#include "display.h"
#include "drehencoder.h"
#include "eeprom.h"

/**
 * Initialisiert die beiden Bälle:
 * Alter und Neuer Zustand
 */
void initBall(void);
    
/**
 * Ist für alle Initialisierungen zuständig
 * -> ruft alle internen init-Methoden auf
 */
void setup(void);

#ifdef	__cplusplus
}
#endif

#endif	/* INIT_H */

