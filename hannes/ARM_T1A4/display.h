/*
 * displah.h
 *
 *  Created on: 27.04.2016
 *      Author: hannes
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_
//********************************************************************
// display.c									Version 02.04.2012 hpw
//********************************************************************
#define 	FULL_PRINTF	0
#include	"include/AT91SAM7S64.h"
#include	"include/display.h"
#include	"include/stdarg.h"
#if FULL_PRINTF == 1
#include "include/stdio.h"						// Definiton I/O
#endif

//**************************************************************
// delay1us(uiK) verzögert um uiK * 1us bei MCK=24Hz
// uiK <= 1.073.741.823 us = 17,8957 sec
//**************************************************************
__attribute__ ((naked)) void delay1us(unsigned int uiK);

//**************************************************************
// UP-Name:			display_write(data,RS)
// Beschreibung:	Schreibt auf das LCD mit
// 					RS=0	Kommando
// 					RS=1	Zeichenausgabe
//**************************************************************
void lcd_write(unsigned char data, unsigned char rs);
//**************************************************************
// UP_Name:			display_init()
// Beschreibung:	Initialisiert das Display im 8-Bit-Modus
//**************************************************************
void display_init(void);
//**************************************************************
// UP-Name:			display_putchar(data)
// Übergabewert:	auszugebenes Zeichen
// Beschreibung:	gibt ein Zeichen auf dem LCD aus
//**************************************************************
void display_putchar(unsigned char data);

//**************************************************************
// UP-Name:			display_putstr(*p)
// Übergabewert:	Die auszugebene Zeichenkette
// Beschreibung:	gibt einen String auf dem LCD aus
//**************************************************************
void display_putstr(char *p);

//**************************************************************
// UP-Name:			display_set_cursor()
// Übergabewert:	x-Position, y-Position
// Beschreibung:	stellt Cursor an Position x,y (Spalte,Zeile)
// 					Für x ist Wert zwischen 0 und 15 erlaubt.
// 					Für y ist 0 (1.Zeile) + 1 (2.Zeile) erlaubt.
//**************************************************************
void display_set_cursor(unsigned char x, unsigned char y);

//**************************************************************
// Up-Name:			display_printf()
// Beschreibung:	schreibt einen String ins LCD-Display. Die
//					Anzahl der Argumente ist beliebig.
//					display_printf() interpretiert in der
//					eingeschränkten Funktionalität aber nur
//					Variablen des Types Int: '%d' und Hex: '%x'
//					Für volle Funktionalität muss der Ausdruck
//					FULL_PRINTF == 1 in angegeben sein.
//**************************************************************
void display_printf(char *string, ...);
unsigned int GetTaste();
#endif /* DISPLAY_H_ */
