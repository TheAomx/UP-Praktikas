//********************************************************************
// ARM_T1A1										Version 02.04.2012 hpw
//********************************************************************
#include "include/AT91SAM7S64.h"			// Definition von ARM7 typischen Registern etc.
#include "cToolMod.h"

#define LED1 AT91C_PIO_PA30									// LED1 angeschlossen an Port-Pin 30
#define LED2 AT91C_PIO_PA2
#define LED3 AT91C_PIO_PA28
#define LED4 AT91C_PIO_PA17
#define LED5 AT91C_PIO_PA18

#define PUSH1 AT91C_PIO_PA1
#define PUSH2 AT91C_PIO_PA0
#define PUSH3 AT91C_PIO_PA31

#define LED_ALL (LED1 | LED2 | LED3 | LED4 | LED5)

//#define A1_1
//#define A1_2
#define A1_3

//**************************************************************
// main läßt LED am Port P30 für ca. 1 Minute mit 1 Hz blinken
//**************************************************************

// main: Hauptprogramm
int main(void) {
	unsigned int leds[] = { LED1, LED2, LED3, LED4, LED5 };
	AT91C_BASE_PIOA->PIO_OER = LED_ALL;		// Ausgang freigeben
	AT91C_BASE_PIOA->PIO_PPUDR = LED_ALL;	// Pull-Up ausschalten
	AT91C_BASE_PIOA->PIO_CODR = LED_ALL;	// Alle LED aus// Alle LED aus

#ifdef A1_1
	running_LED(leds);
#endif

#ifdef A1_2
	while(1) {
		unsigned int to_blink = leds[0] | leds[2] | leds[4];
		blink(1, 10, to_blink, 0);
		blink(4, 10, to_blink, 0);
		blink(8, 10, to_blink, 0);
		blink(16, 10, to_blink, 0);
		blink(1, 1, to_blink, 0);
	}
#endif
#ifdef A1_3
	while (1) {
		unsigned int to_blink = leds[0] | leds[2] | leds[4];
		unsigned int to_blink_gegen = leds[1] | leds[3];
		blink(1, 10, to_blink, to_blink_gegen);
		blink(4, 10, to_blink, to_blink_gegen);
		blink(8, 10, to_blink, to_blink_gegen);
		blink(16, 10, to_blink, to_blink_gegen);
		blink(1, 10, to_blink, to_blink_gegen);
	}

#endif

}
