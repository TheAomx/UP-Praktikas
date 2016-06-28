//********************************************************************
// ARM_T4A1										Version 02.04.2012 hpw
//********************************************************************
#include 	"include/AT91SAM7S64.h"		// Definition von ARM7 typischen Registern etc.

#define		PH2			AT91C_PIO_PA23				// Alle Ports die für den Motor verwendet werden
#define		PH1			AT91C_PIO_PA13
#define		I21			AT91C_PIO_PA20
#define		I11			AT91C_PIO_PA14
#define		I20			AT91C_PIO_PA16
#define		I10			AT91C_PIO_PA15
#define		MOTOR_MASK	(PH2|PH1|I21|I11|I20|I10)

#define PUSH1 AT91C_PIO_PA1
#define PUSH2 AT91C_PIO_PA0
#define PUSH3 AT91C_PIO_PA31
//**************************************************************
// delay2ms(uiK) verzögert um ca. uiK * 2ms
//**************************************************************
void delay2ms(unsigned int uiK) {
	volatile unsigned int uiI;

	while (uiK--)				// folgende for-Schleife wird uiK-mal aufgerufen
		for (uiI = 0; uiI < 3200; uiI++)
			;		//  for-Schleife wird 3200-mal durchlaufen. Dies
					//  verursacht eine Zeitverzögerung von ca. 2ms.
}

// T3 | T2 | T1 | value
//    |    |    | 0
//    |    | x  | 1
//    |  x |    | 2
//    |  x | x  | 3
//  x |    |    | 4
//  x |    | x  | 5
//  x |  x |    | 6
//  x |  x | x  | 7

unsigned char GetTaste() {
	AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_PIOA);
	unsigned int status = AT91C_BASE_PIOA->PIO_PDSR;
	return (!(status & PUSH1) | !(status & PUSH2) << 1 | !(status & PUSH3) << 2);
}

#define STATE_MAX 3 	// 0 ...7 array indexes
#define SPEED_MAX 20 	// times 2ms delay max
#define SPEED_MIN 1 	// times 2ms delay min

//**************************************************************
// main läßt den Schrittmotor durchgehend nach rechts laufen
//**************************************************************
int main() {
	unsigned int transition[4] = { 	// Tabelle für Halbschrittbetrieb
			I10 | I11, 			// Schritt 1|2,  5|6
			I21 | I20 | PH2,	// Schritt 2|3, 6|7
			I21 | I20,			// Schritt 3|4,  7|8
			I10 | I11 | PH1, 	// Schritt 4|5,  8|1
	};

	enum {
		LEFT, RIGHT
	};

	int direction = RIGHT, times_delay = 4, current_state = 0, running = 1;

	AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_PIOA);
	AT91C_BASE_PIOA->PIO_PPUER = PUSH1 | PUSH2 | PUSH3;

	AT91C_BASE_PIOA->PIO_OER = MOTOR_MASK;	// Freigabe der Motor-Port-Pins
	AT91C_BASE_PIOA->PIO_OWER = MOTOR_MASK;	// Freigabe des schreibenden/lesenden
	AT91C_BASE_PIOA->PIO_PPUDR = MOTOR_MASK;// Ausschalten der Pull-up-Widerstände

	// Set initial State 1
	AT91C_BASE_PIOA->PIO_CODR = I10 | I11 | PH2;
	AT91C_BASE_PIOA->PIO_SODR = PH1 | I21 | I20;

	while (1) {
		if (running) {
			if (direction == LEFT && --current_state < 0)
				current_state = STATE_MAX;

			AT91C_BASE_PIOA->PIO_ODSR ^= transition[current_state];

			if (direction == RIGHT && ++current_state > STATE_MAX)
				current_state = 0;
		}

		//	display_set_cursor(0, 0);
		//	display_printf("k: %i", times_delay);
		delay2ms(times_delay);

		switch (GetTaste()) {
		case 1:
			if (++times_delay > SPEED_MAX)
				times_delay = SPEED_MAX;
			break;
		case 2:
			running = !running;
			// Set initial State 1
			AT91C_BASE_PIOA->PIO_CODR = I10 | I11 | PH2;
			AT91C_BASE_PIOA->PIO_SODR = PH1 | I21 | I20;
			current_state = 0;
			times_delay = 1;
			break;
		case 3:
			direction = LEFT;
			break;
		case 4:
			if (--times_delay < SPEED_MIN)
				times_delay = SPEED_MIN;
			break;
		case 6:
			direction = RIGHT;
			break;
		}

	}
}
