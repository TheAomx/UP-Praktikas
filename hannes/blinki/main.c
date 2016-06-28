#include "include/AT91SAM7S64.h"							// Register-Deklarationen

#define TYPICAL
// #define POINTER
// #define DIRECT

#define LED1 AT91C_PIO_PA30									// LED1 angeschlossen an Port-Pin 30
#define LED2 AT91C_PIO_PA2
#define LED3 AT91C_PIO_PA28
#define LED4 AT91C_PIO_PA17
#define LED5 AT91C_PIO_PA18

#define PUSH1 AT91C_PIO_PA1
#define PUSH2 AT91C_PIO_PA0
#define PUSH3 AT91C_PIO_PA31

#define LED_ALL (LED1 | LED2 | LED3 | LED4 | LED5)

// delay: Funktion, die eine Verzögerung verursacht
void delay(int times) {
	unsigned int i;
	for (i = 0; i < times; i++)
		;
}

#define get_input()\
	AT91C_BASE_PIOA

enum DIRECTION {
	LEFT, RIGHT
};
// main: Hauptprogramm
int main(void) {
	unsigned int leds[] = { LED1, LED2, LED3, LED4, LED5 };

	AT91C_BASE_PIOA->PIO_OER = LED_ALL;					// Ausgang freigeben
	AT91C_BASE_PIOA->PIO_PPUDR = LED_ALL;				// Pull-Up ausschalten
	AT91C_BASE_PIOA->PIO_CODR = LED_ALL;				// Alle LED aus

	int i = 0;
	char direction = RIGHT;
	unsigned int *button = (unsigned int *)(AT91C_BASE_PIOA->PIO_PDSR);

	unsigned int button_state=0;
	while (1) {										// ab hier: Endlosschleife
#ifdef	TYPICAL
		// Zur Beschreibung von Peripheriemodulen werden typischerweise
		// Registerdefinitionen einer Header-Datei eingebunden (hier: 'AT91SAM7S64.h')
		AT91C_BASE_PIOA->PIO_SODR = leds[i];						// LED aus
		delay(50000u);													// Pause
		AT91C_BASE_PIOA->PIO_CODR = leds[i];						// LED an
		delay(50000u);													// Pause
		//i++;
		//i %= 5;

		if (*button & PUSH1)
			delay(50000u);

		if (!(i % 4))
			direction = LEFT;
		if (!i)
			direction = RIGHT;

		if (direction == RIGHT)
			i++;
		if (direction == LEFT)
			i--;

#endif		
#ifdef 	POINTER
		// Alternativ: Version mit Zeigerdefinition
		volatile unsigned int *CODR = (unsigned int *) 0xFFFFF434;// Zeigerdefiniton 'Clear Output Data Register'
		*CODR = 0x40000000;// LED aus
		delay();// Pause
		volatile unsigned int *SODR = (unsigned int *) 0xFFFFF430;// Zeigerdefiniton 'Set Output Data Register'
		*SODR = 0x40000000;// LED an
		delay();// Pause
#endif		
#ifdef 	DIRECT	
		// Alternativ: direkte Version
		*((volatile unsigned int *) 0xFFFFF434) = 0x40000000;// LED aus
		delay();// Pause
		*((volatile unsigned int *) 0xFFFFF430) = 0x40000000;// LED an
		delay();
#endif		
	}
}
