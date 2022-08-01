/* 
 * File:   display.h
 * Author: Luca Krause
 * 
 * Dieses Header-File stellt die Kommunikation 
 * mit dem Display DEM12864 dar.
 * 
 *
 * Created on 21. April 2022, 13:27
 */

#ifndef DISPLAY_H
#define	DISPLAY_H

#ifdef	__cplusplus
extern "C" {
#endif


#ifdef	__cplusplus
}
#endif

#include <avr/io.h>

#include "utils.h"
#include "font.h"

//Control Bits
#define LCD_CS1 0
#define LCD_CS2 1
#define LCD_RESET 2
#define LCD_RW 3
#define LCD_DI 4
#define LCD_ENABLE 5

//Commands
#define LCD_ON 0x3F
#define LCD_PAGE 0xB8
#define LCD_COLUMN 0x40
#define LCD_START_LINE 0xC0

/**
 * Sendet ein Kommando an das LCD-Display
 * @param command Kommando für das Display
 */
void LCDCommand(char command);

/**
 * Initialisiert das LCD-Display
 */
void init_LCD(void);

/**
 * Setzt die Seite des LCD-Displays
 * @param page zu setzende Page
 */
void LCDPageSet(unsigned char page);

/**
 * Setzt die Spalte des LCD-Displays
 * @param column zu setzende Spalte
 */
void LCDColumnSet(unsigned char column);

/*
 * Setzt das gesamte Display zurück
 */
void LCDClear(void);

/**
 * Setzt eine 1-Bit breite Linie auf dem Display
 * -> 8. Bit der ersten Page
 */
void LCDLine(void);


/**
 * Schreibt ein Daten-Byte an das Display
 * @param x Spalte
 * @param y Page: 0-7
 * @param data Daten
 */
void LCDWriteByte(char x, char y, char data);

/**
 * Schreibt mehrere Bytes hintereinander an das Display
 * (arbeitet schneller, als mehrfach LCDWriteByte aufzurufen)
 * -> funktioniert nur, wenn die Daten nur einen Chip ansprechen,
 * siehe Ausgabe der Punkte
 * @param x Spalte
 * @param y Page: 0-7
 * @param data Daten
 * @param length Länge der Daten in Bytes
 */
void LCDWriteBytes(char x, char y, const char *data, char length);

/**
 * Zeigt zwei Schläger auf dem Display an
 * @param batLeft Linker Schläger
 * @param batRight Rechter Schläger
 */
void showBats(struct BAT batLeft, struct BAT batRight);

/**
 * Zeigt den Ball auf dem Display an
 * @param ball zu zeigender Ball
 */
void showBall(struct BALL ball);

/**
 * Löscht den Ball auf dem Display
 * @param ball Ball
 */
void deleteBall(struct BALL ball);

/**
 * Lädt die Punkte auf das Display
 * @param points Punkte
 * @param x Start-Spalte
 * @param y Start-Page
 */
void loadPoints(unsigned int points, int x, int y);

/*
 * Lädt beide Punkte und Highscores neu
 */
void reloadPoints(void);

#endif	/* DISPLAY_H */

