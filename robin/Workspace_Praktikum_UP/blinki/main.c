#include "include/AT91SAM7S64.h"							// Register-Deklarationen

#define TYPICAL
// #define POINTER
// #define DIRECT

#define LED1 AT91C_PIO_PA30	// LED1 angeschlossen an Port-Pin 30
#define LED2 AT91C_PIO_PA28 // LED2?

// delay: Funktion, die eine Verzögerung verursacht
void delay(){
	unsigned int i;
	for (i=0;i<800000u;i++);
}

// main: Hauptprogramm
int main(void){

	AT91C_BASE_PIOA->PIO_OER = LED1 | LED2;							// Ausgang freigeben
	AT91C_BASE_PIOA->PIO_PPUDR = LED1 | LED2;							// Pull-Up ausschalten

	while (1){													// ab hier: Endlosschleife
#ifdef	TYPICAL
		// Zur Beschreibung von Peripheriemodulen werden typischerweise
		// Registerdefinitionen einer Header-Datei eingebunden (hier: 'AT91SAM7S64.h')
		AT91C_BASE_PIOA->PIO_CODR = LED1 | LED2;							// LED aus
		delay();													// Pause
		AT91C_BASE_PIOA->PIO_SODR = LED1 | LED2;							// LED an
		delay();													// Pause
#endif		
#ifdef 	POINTER
		// Alternativ: Version mit Zeigerdefinition
		volatile unsigned int *CODR = (unsigned int *) 0xFFFFF434;	// Zeigerdefiniton 'Clear Output Data Register'
		*CODR = 0x40000000;											// LED aus
		delay();													// Pause
		volatile unsigned int *SODR = (unsigned int *) 0xFFFFF430;	// Zeigerdefiniton 'Set Output Data Register'
		*SODR = 0x40000000;											// LED an
		delay();													// Pause
#endif		
#ifdef 	DIRECT	
		// Alternativ: direkte Version
		*((volatile unsigned int *) 0xFFFFF434) = 0x40000000;		// LED aus
		delay();													// Pause
		*((volatile unsigned int *) 0xFFFFF430) = 0x40000000;		// LED an
		delay();
#endif		
	}
}
