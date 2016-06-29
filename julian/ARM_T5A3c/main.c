//********************************************************************
// ARM_T5A3c									Version 02.04.2012 hpw
//********************************************************************
#include	"include/AT91SAM7S64.h"			// Definition von ARM7 typischen Registern etc.
#include	"include/serial.h"
#include	"include/display.h"
#include    "include/interrupt.h"

#define		BUTTON1	AT91C_PIO_PA1
#define		BUTTON2	AT91C_PIO_PA0
#define		BUTTON3	AT91C_PIO_PA31

void delay_ms(unsigned int uiK){
	volatile unsigned int uiI;

	while (uiK--)
		for (uiI=0; uiI<1638; uiI++);

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

__attribute__ ((used)) void pioa_isr (void){

	volatile unsigned int dummy;			// Lokale dummy-Variable als volatile deklariert

	unsigned char button_state = get_button_state();

	switch (button_state) {
	case 1:
		usart0_puts("button 1 pressed\n");
		break;
	case 2:
		usart0_puts("button 2 pressed\n");
		break;
	case 3:
		break;
	case 4:
		usart0_puts("button 3 pressed\n");
		break;
	case 6:

		break;
	}

	dummy = AT91C_BASE_PIOA->PIO_ISR;		// Bestätigung Interrupt Request
}

//**************************************************************
// main()
//**************************************************************
int main(void){
	AT91C_BASE_PMC->PMC_PCER=(1<<AT91C_ID_PIOA);	// PIO-Takt einschalten

	usart0_init();
	display_init();

#if 0
	pioa_ir_init(BUTTON1, 4, 3, pioa_isr);
	pioa_ir_init(BUTTON2, 4, 3, pioa_isr);
	pioa_ir_init(BUTTON3, 4, 3, pioa_isr);

	while (1) {

	}
#endif
#if 1
	while (1) {
		if (usart0_kbhit()) {
			unsigned char read_char = usart0_getc();
			display_putchar(read_char);
			delay_ms(100);
			display_clear();
		}

	}
#endif
}
