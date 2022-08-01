/* 
 * File:   display.c
 * Author: Luca Krause
 *
 * Created on 21. April 2022, 13:27
 */

#include <stdlib.h>

#include "display.h"
#include "utils.h"

/**
 * Sendet die Daten/das Kommando an das LCD-Display
 */
void sendLCD(void) {
    _delay_us(5);
    SetBit(PORTC, LCD_ENABLE);
    _delay_us(5);
    ClearBit(PORTC, LCD_ENABLE);
    _delay_us(5);
}

void LCDCommand(char command){
    DDRD = 0xFF;
    PORTD = 0;
    ClearBit(PORTC, LCD_RW);
    ClearBit(PORTC, LCD_DI);
    PORTD = command;
    sendLCD();
}

/**
 * Setzt das LCD-Display zurück
 */
void resetLCD(void) {
    ClearBit(PORTC, LCD_RESET);
    SetBit(PORTC, LCD_RESET);
}

void init_LCD(void) {
    DDRD = 0xFF;
    DDRC |= 0x3F; //7 Bit Register, aus dem 6 für das Display verwendet werden
                 // 0x3F = 0b0111111
    resetLCD();
    SetBit(PORTC, LCD_CS1);
    SetBit(PORTC, LCD_CS2);
    LCDCommand(LCD_ON);
    LCDCommand(LCD_START_LINE);//StartLine = 0
}

void LCDPageSet(unsigned char page) {
    LCDCommand(LCD_PAGE | page);
}

void LCDColumnSet(unsigned char column) {
    LCDCommand(LCD_COLUMN | column);
}

void LCDClear(void) {
    SetBit(PORTC, LCD_CS1);
    SetBit(PORTC, LCD_CS2);
    
    int i = 0;
    for (i = 0; i < 8; i++) {
        LCDPageSet(i); //starte oben links
        int j;
        for (j = 0; j < CHIP_WIDTH; j++) {
            SetBit(PORTC, LCD_DI);
            ClearBit(PORTC, LCD_RW);
            PORTD = 0;
            sendLCD();
        }
        LCDColumnSet(0);
    }
}

void LCDLine(void) {
    SetBit(PORTC, LCD_CS1);
    SetBit(PORTC, LCD_CS2);
    LCDColumnSet(0);
    LCDPageSet(0);
    SetBit(PORTC, LCD_DI);
    ClearBit(PORTC, LCD_RW);
    PORTD = 1<<7;
    int i;
    for (i = 0; i < CHIP_WIDTH; i++) {
        sendLCD();
    }
}

void LCDWriteByte(char x, char y, char data) {
    //Nur einen Chip ansprechen
    if (x < CHIP_WIDTH) {
        SetBit(PORTC, LCD_CS1);
        ClearBit(PORTC, LCD_CS2);
    } else if (x < CHIP_WIDTH * 2) {
        ClearBit(PORTC, LCD_CS1);
        SetBit(PORTC, LCD_CS2);
    }
    LCDColumnSet(x);
    LCDPageSet(y);
    SetBit(PORTC, LCD_DI);
    ClearBit(PORTC, LCD_RW);
    PORTD = data;
    sendLCD();
}

void LCDWriteBytes(char x, char y, const char *data, char length) {
    //Nur einen Chip ansprechen
    if (x < CHIP_WIDTH) {
        SetBit(PORTC, LCD_CS1);
        ClearBit(PORTC, LCD_CS2);
    } else if (x < CHIP_WIDTH * 2) {
        ClearBit(PORTC, LCD_CS1);
        SetBit(PORTC, LCD_CS2);
    }
    LCDColumnSet(x);
    LCDPageSet(y);
    SetBit(PORTC, LCD_DI);
    ClearBit(PORTC, LCD_RW);
    
    int i;
    for (i = 0; i < length; i++) {
        PORTD = data[i];
        sendLCD();
    }
}

/**
 * Zeigt einen Schläger auf dem Display an
 * @param bat zu zeigender Schläger
 */
static void showBat(struct BAT bat) {
    //Schläger kann sich über sieben Byte verteilen
    char batChars[8] = {0,0,0,0,0,0,0,0};
    char posStart = bat.Y;
    char posEnd = bat.Y + BAT_SIZE - 1;//Dargestellte Größe beträgt "BAT_SIZE"

    //7-Byte-Berechnung
    char firstByte = 0xFF << (posStart%8);
    char secondByte = 0xFF >> (7 - (posEnd%8));
    batChars[bat.Y/8] = firstByte;
    batChars[bat.Y/8+1] = secondByte;
    
    //Ausgabe
    int i = 1;
    for (i = 1; i < 8; i++)
    {
        LCDColumnSet(bat.X);
        LCDPageSet(i);
        LCDWriteByte(bat.X, i, batChars[i]);
    }
}

void showBats(struct BAT batLeft, struct BAT batRight) {
    showBat(batLeft);
    showBat(batRight);
}

void showBall(struct BALL ball) {
    //Bit-Darstellung des Balls:
    // 1 1 1
    // 1 1 1
    // 1 1 1
    unsigned char pos = ball.Y;
    char bytes[2] = {0,0};
    unsigned char mod = pos % 8;
    char page = ball.Y / 8;

    //ausgeben
    switch (mod) {
        case 0:
        {
            bytes[0] = 0b10000000;
            bytes[1] = 0b00000011;
            LCDWriteByte(ball.X-1,  page-1, bytes[0]);
            LCDWriteByte(ball.X,    page-1, bytes[0]);
            LCDWriteByte(ball.X+1,  page-1, bytes[0]);
            LCDWriteByte(ball.X-1,  page,   bytes[1]);
            LCDWriteByte(ball.X,    page,   bytes[1]);
            LCDWriteByte(ball.X+1,  page,   bytes[1]);
        }
            break;
        case 7:
        {
            bytes[0] = 0b11000000;
            bytes[1] = 0b00000001;
            LCDWriteByte(ball.X-1,  page,   bytes[0]);
            LCDWriteByte(ball.X,    page,   bytes[0]);
            LCDWriteByte(ball.X+1,  page,   bytes[0]);
            LCDWriteByte(ball.X-1,  page+1, bytes[1]);
            LCDWriteByte(ball.X,    page+1, bytes[1]);
            LCDWriteByte(ball.X+1,  page+1, bytes[1]);
        }
            break;
        default:
        {
            bytes[0] = 0b00000111<<(mod-1);
            LCDWriteByte(ball.X-1,  page,   bytes[0]);
            LCDWriteByte(ball.X,    page,   bytes[0]);
            LCDWriteByte(ball.X+1,  page,   bytes[0]);
        }
            break;
    }
}

void deleteBall(struct BALL ball) {
    //Bit-Darstellung des Balls:
    // 1 1 1
    // 1 1 1
    // 1 1 1
    char pos = ball.Y;
    char mod = pos % 8;
    char page = pos / 8;
    char res = 0;

    //ausgeben
    switch (mod) {
        case 0:
        {
            LCDWriteByte(ball.X-1,  page-1, res);
            LCDWriteByte(ball.X,    page-1, res);
            LCDWriteByte(ball.X+1,  page-1, res);
            LCDWriteByte(ball.X-1,  page,   res);
            LCDWriteByte(ball.X,    page,   res);
            LCDWriteByte(ball.X+1,  page,   res);
        }
            break;
        case 7:
        {
            LCDWriteByte(ball.X-1,  page,   res);
            LCDWriteByte(ball.X,    page,   res);
            LCDWriteByte(ball.X+1,  page,   res);
            LCDWriteByte(ball.X-1,  page+1, res);
            LCDWriteByte(ball.X,    page+1, res);
            LCDWriteByte(ball.X+1,  page+1, res);
        }
            break;
        default:
        {
            LCDWriteByte(ball.X-1,  page,   res);
            LCDWriteByte(ball.X,    page,   res);
            LCDWriteByte(ball.X+1,  page,   res);
        }
            break;
    }
}

void loadPoints(unsigned int points, int x, int y) {
    char newPoints[5][FONT_SIZE];
    unsigned char i,j = 0;
    const char *(oldPoints[5]);
    //chars aus Punkten berechnen
    if (points == 0) {
        oldPoints[5-j-1] = font[0+NUMBER_START_ASCII];
        j++;
    } else {
        while (points != 0 && j != 5) {
            unsigned int res = points % 10;
            oldPoints[5-j-1] = font[res+NUMBER_START_ASCII];
            points /= 10;
            j++;
        }
    }
    
    //alle anderen chars auf 0b00000000 setzen
    while (j != 5) {
        oldPoints[5-j-1] = font[0];
        j++;
    }
    
    //schwarzen Balken als Spielfeldbegrenzung hinzufügen
    for (i = 0; i < 5; i++) {
        for (j = 0; j < FONT_SIZE; j++) {
            newPoints[i][j] = oldPoints[i][j] | 0b10000000;
        }
    }
    
    //kompletten Punkte übertragen
    LCDWriteBytes(x, y, *newPoints, FONT_SIZE*5);
}

void reloadPoints(void) {
    loadPoints(playerLeft.points, 0, 0);
    loadPoints(playerRight.points, 96, 0);
    loadPoints(highscore, 64, 0);
    loadPoints(highscoreNow, 32, 0);
}