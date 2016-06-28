/*
 * cToolMod.c
 *
 *  Created on: 27.04.2016
 *      Author: hannes
 */
#include "cToolMod.h"
#include "include/AT91SAM7S64.h"			// Definition von ARM7 typischen Registern etc.

//**************************************************************
// delay5ms(uiK) verzögert um ca. uiK * 5ms
//**************************************************************
void delay5ms(unsigned int uiK) {
	volatile unsigned int uiI;

	while (uiK--)				// folgende for-Schleife wird uiK-mal aufgerufen
		for (uiI = 0; uiI < 8192; uiI++)
			;		//  for-Schleife wird 8192-mal durchlaufen. Dies
					//  verursacht eine Zeitverzögerung von ca. 5ms.
}

//**************************************************************
// delay5ms(uiK) verzögert um ca. uiK * 1ms
//**************************************************************
void delay1ms(unsigned int uiK) {
	volatile unsigned int uiI;
	while (uiK--) // folgende for-Schleife wird uiK-mal aufgerufen
		for (uiI = 0; uiI < 1638; uiI++)
			;
}

#define BLINK_DELAY(hz)(unsigned int)(((1.0/hz)*1000)/2)
#define BLINK_ITERATION(time_sec, delay) (unsigned int)((time_sec*1000)/(delay*2))

void blink(unsigned int frequence, unsigned int blink_time, unsigned to_blink,
		unsigned to_blink_gegen) {

	unsigned int i;
	unsigned int delay = BLINK_DELAY(frequence);
	unsigned int mask = (to_blink | to_blink_gegen);
	// Enable direkten zugriff auf output data register
	AT91C_BASE_PIOA->PIO_OWER = (to_blink | to_blink_gegen);

	AT91C_BASE_PIOA->PIO_CODR = to_blink;
	AT91C_BASE_PIOA->PIO_SODR = to_blink_gegen;
	delay1ms(delay);

	for (i = 0; i < (BLINK_ITERATION(blink_time, delay) * 2) - 1; i++) {
		AT91C_BASE_PIOA->PIO_ODSR ^= mask;
		delay1ms(delay);
	}
}

void running_LED(unsigned int leds[5]) {

	enum DIRECTION {
		LEFT, RIGHT
	};

	int i = 0;
	char direction = RIGHT;
	while (1) {
		AT91C_BASE_PIOA->PIO_SODR = leds[i];	// LED aus
		delay5ms(3);	// Pause
		AT91C_BASE_PIOA->PIO_CODR = leds[i];	// LED an
		delay5ms(3);	// Pause

		if (!(i % 4))
			direction = LEFT;
		if (!i)
			direction = RIGHT;

		(direction == RIGHT) ? i++ : i--;
	}
}

#define PUSH1 AT91C_PIO_PA1
#define PUSH2 AT91C_PIO_PA0
#define PUSH3 AT91C_PIO_PA31

unsigned char GetTaste() {
	AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_PIOA);
	unsigned int status = AT91C_BASE_PIOA->PIO_PDSR;
	return (!(status & PUSH1) | !(status & PUSH2) << 1 | !(status & PUSH3) << 2);
}

