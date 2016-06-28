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
__attribute__ ((naked)) void delay1us(unsigned int uiK) {

	asm("			lsl	r3, r0, #2		\n\t"		// Multiplikation *4
			"Label:		cmp	r3, #0			\n\t"// 1
			"			beq	Ende			\n\t"// 2
			"			sub	r3, r3, #1		\n\t"// 1
			"			b	Label			\n\t"// 2
			"Ende:		mov	pc, lr			\n\t");
}

//**************************************************************
// UP-Name:			display_write(data,RS)
// Beschreibung:	Schreibt auf das LCD mit
// 					RS=0	Kommando
// 					RS=1	Zeichenausgabe
//**************************************************************
void lcd_write(unsigned char data, unsigned char rs) {
	LCD_PORT->PIO_CODR = LCD_RW;					// Schreibzugriff
	if (rs)
		LCD_PORT->PIO_SODR = LCD_RS;			// Register-Select
	else
		LCD_PORT->PIO_CODR = LCD_RS;
	LCD_PORT->PIO_CODR = LCD_DBUS;					// Datenleitungen löschen
	if (data & 0x01)
		LCD_PORT->PIO_SODR = LCD_D0;	// Datenbits setzen, wenn notwendig
	if (data & 0x02)
		LCD_PORT->PIO_SODR = LCD_D1;
	if (data & 0x04)
		LCD_PORT->PIO_SODR = LCD_D2;
	if (data & 0x08)
		LCD_PORT->PIO_SODR = LCD_D3;
	if (data & 0x10)
		LCD_PORT->PIO_SODR = LCD_D4;
	if (data & 0x20)
		LCD_PORT->PIO_SODR = LCD_D5;
	if (data & 0x40)
		LCD_PORT->PIO_SODR = LCD_D6;
	if (data & 0x80)
		LCD_PORT->PIO_SODR = LCD_D7;
	delay1us(1);									// kurz warten
	LCD_PORT->PIO_SODR = LCD_E;						// Enable setzen
	delay1us(50);									// 50 us warten
	LCD_PORT->PIO_CODR = LCD_E;						// Enable zurücksetzen
}

//**************************************************************
// UP_Name:			display_init()
// Beschreibung:	Initialisiert das Display im 8-Bit-Modus
//**************************************************************
void display_init(void) {
	LCD_PORT->PIO_PPUDR = LCD_BUS;					// Pullups aus
	LCD_PORT->PIO_OER = LCD_BUS;					// Port-Pins als Ausgang
	LCD_PORT->PIO_CODR = LCD_BUS;					// Port-Pins auf '0'
	delay1us(15000);								// 15ms warten
	lcd_write(0x30, 0);								// Initialisierung 1. Mal
	delay1us(4100);									// 4.1ms warten
	lcd_write(0x30, 0);								// Initialisierung 2. Mal
	delay1us(100);									// 100us warten
	lcd_write(0x30, 0);								// Initialisierung 3. Mal
	lcd_write(0x38, 0);			// 8-Bit-Modus, 2/4-zeiliges Display, 5x7 Font
	lcd_write(0x0C, 0);								// Display an, Cursor aus
	lcd_write(0x06, 0);		// Cursor inkrementieren und Scrollen ausschalten
	lcd_write(0x01, 0);								// Display löschen
	delay1us(5000);									// 5ms warten
}

//**************************************************************
// UP-Name:			display_putchar(data)
// Übergabewert:	auszugebenes Zeichen
// Beschreibung:	gibt ein Zeichen auf dem LCD aus
//**************************************************************
void display_putchar(unsigned char data) {
	lcd_write(data, 1);
}

//**************************************************************
// UP-Name:			display_putstr(*p)
// Übergabewert:	Die auszugebene Zeichenkette
// Beschreibung:	gibt einen String auf dem LCD aus
//**************************************************************
void display_putstr(char *p) {
	while (*p) {
		display_putchar(*p);
		p++;
	}
}

//**************************************************************
// UP-Name:			display_set_cursor()
// Übergabewert:	x-Position, y-Position
// Beschreibung:	stellt Cursor an Position x,y (Spalte,Zeile)
// 					Für x ist Wert zwischen 0 und 15 erlaubt.
// 					Für y ist 0 (1.Zeile) + 1 (2.Zeile) erlaubt.
//**************************************************************
void display_set_cursor(unsigned char x, unsigned char y) {
	lcd_write((0x80 + y * 0x40 + x), 0);
}

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
void display_printf(char *string, ...) {
	char buffer1[20];								// Puffer für Formatstring
	unsigned char i;								// Laufvariable
#if FULL_PRINTF == 1
	char buffer2[17];								// Puffer für sprintf
#else
	char Tabelle[] = "0123456789abcdef";			// Tabelle für Konvertierung
	unsigned int ui;								// Vorzeichenlose Variable
	signed int si;								// Vorzeichenbehaftete Variable
	double d;										// Variable Typ 'double'
#endif
	va_list ap;										// Argumentenpointer
	char *p;										// Pointer auf String

	va_start(ap, string);					// Start variable Argumentenliste
	for (p = string; *p; p++) {						// Umwandlungsangaben suchen
		if (*p == '%') {						// Beginn der Umwandlungsangabe
			i = 0;									// Startwert
			do {				// Ende der Umwandlungsangabe; Umwandlung in:
				buffer1[i++] = *p++;// Umwandlungsstring wird im Puffer abgelegt
				if (i > 18)
					i = 18;				// Damit es keinen Bufferoverflow gibt
			} while (!((*p == 'd') | (*p == 'i') |	// dezimal mit Vorzeichen
					(*p == 'o') |					// oktal
					(*p == 'x') | (*p == 'X') |		// hexadezimal
					(*p == 'u') |					// dezimal ohne Vorzeichen
					(*p == 'c') |					// einzelnes Zeichen
					(*p == 's') |					// string
					(*p == 'f') |					// float
					(*p == 'e') | (*p == 'E') |		// Gleitpunktzahl
					(*p == 'g') | (*p == 'G') |		// Gleitpunktzahl
					(*p == '%')));				// '%'
			buffer1[i++] = *p;		// nun noch die Angabe des Umwandlungswertes
			buffer1[i] = 0;				// hiermit ist der Puffer abgeschlossen
// Ab hier: wahlweise mit voller PRINTF-Funktionalität
#if FULL_PRINTF == 1
					if ((*p=='d')|(*p == 'i') |// Argument ist Typ 'int'
							(*p=='o')|
							(*p=='x')|(*p == 'X') |
							(*p=='u')|
							(*p=='c')|
							(*p=='s')) {
						snprintf(buffer2,17,buffer1,va_arg(ap,int));
						display_putstr(buffer2);
					}
					if ((*p=='f')|		// Argument ist Typ 'double'
							(*p=='e')|(*p=='E')|
							(*p=='g')|(*p=='G')) {
						snprintf(buffer2,17,buffer1,va_arg(ap,double));
						display_putstr(buffer2);
					}
// Ab hier: ... oder mit eingeschränkter PRINTF-Funktionalität
#else
			if ((*p == 'd') | (*p == 'i')) {// Ausgabe als vorzeichenbehaftete Zahl
				si = va_arg(ap, int);
				if (si < 0) {
					display_putchar('-');
					si = -si;
				}
				if (si == 0) {
					display_putchar('0');
				} else {
					for (i = 15; i && si; --i) {
						buffer1[i] = Tabelle[(si % 10)];
						si /= 10;
					}
					for (; i < 15; display_putchar(buffer1[++i]))
						;
				}
			}
			if ((*p == 'x') | (*p == 'X')) {	// Ausgabe als hexadezimale Zahl
				ui = va_arg(ap, unsigned int);
				if (ui == 0) {
					display_putchar('0');
				} else {
					for (i = 15; i && ui; --i) {
						buffer1[i] = Tabelle[(ui % 16)];
						ui /= 16;
					}
					for (; i < 15; display_putchar(buffer1[++i]))
						;
				}
			}
			if ((*p == 'o') |	// Unbekannter Typ; Ausgabe wird übersprungen
					(*p == 'u') | (*p == 'c') | (*p == 's')) {
				ui = va_arg(ap, unsigned int);
			}
			if ((*p == 'f') |	// Unbekannter Typ; Ausgabe wird übersprungen
					(*p == 'e') | (*p == 'E') | (*p == 'g') | (*p == 'G')) {
				d = va_arg(ap, double);
			}
// Ab hier: wieder für beide Versionen
#endif
			if ((*p == '%'))
				display_putchar(*p);	// Argument ist '%'
		} else
			display_putchar(*p);				// kein Steuerzeichen gefunden
	}
	va_end(ap);									// Ende variable Argumentenliste
}

#define PUSH1 AT91C_PIO_PA1
#define PUSH2 AT91C_PIO_PA0
#define PUSH3 AT91C_PIO_PA31

unsigned char GetTaste() {
	AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_PIOA);
	unsigned int status = AT91C_BASE_PIOA->PIO_PDSR;
	return (!(status & PUSH1) | !(status & PUSH2) << 1 | !(status & PUSH3) << 2);
}

