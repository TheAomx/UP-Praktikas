/*
 * main.c
 *
 *  Created on: 18.04.2016
 *      Author: Aomx
 */

#include "include/AT91SAM7S64.h"							// Register-Deklarationen
#include "display.h"

#define BUTTON1 AT91C_PIO_PA1
#define BUTTON2 AT91C_PIO_PA0
#define BUTTON3 AT91C_PIO_PA31

void delay_ms(unsigned int milliseconds){
	volatile unsigned int i;

	while (milliseconds--)
		for (i=0; i<1638; i++);
}

void delay_s(unsigned int seconds) {
	delay_ms(1000 * seconds);
}

unsigned char is_button_pressed(unsigned int button) {
	return !(AT91C_BASE_PIOA->PIO_PDSR & button);
}

unsigned char get_button_state() {
	unsigned char retValue = 0;

	if (is_button_pressed(BUTTON1)) {
		retValue += 1;
	}

	if (is_button_pressed(BUTTON2)) {
		retValue += 2;
	}

	if (is_button_pressed(BUTTON3)) {
		retValue += 4;
	}

	return retValue;
}


int main (void) {
#if 1
	display_init();
	// pio takt aktivieren
	AT91C_BASE_PMC->PMC_PCER = (1<<AT91C_ID_PIOA);

	while (1) {
		display_clear();
		display_printf("%d", get_button_state());
		delay_ms(200);
	}
#endif
#if 0
	unsigned char ucJ = 0;
	display_init();
	do {
		display_clear();
		display_printf("0x%x", ucJ);
		display_set_cursor(0,1);
		display_putchar(ucJ);
		delay_ms(500);
		ucJ++;
	} while (ucJ != 0);

	while (1);
#endif
}
