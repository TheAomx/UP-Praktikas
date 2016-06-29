//********************************************************************
// ARM_T5A3c									Version 02.04.2012 hpw
//********************************************************************
#include	"include/AT91SAM7S64.h"			// Definition von ARM7 typischen Registern etc.
#include	"include/serial.h"
#include	"include/display.h"
#include 	"include/interrupt.h"
#include 	"include/cToolMod.h"

#define		Taste1	AT91C_PIO_PA1
#define		Taste2	AT91C_PIO_PA0
#define		Taste3	AT91C_PIO_PA31


__attribute__ ((used)) void pioa_isr (void){

	volatile unsigned int dummy;			// Lokale dummy-Variable als volatile deklariert

	switch(getTaste()) {
		case 1:
			usart0_puts("Taste 1");
		break;
		case 2:
			usart0_puts("Taste 2");
		break;
		case 4:
			usart0_puts("Taste 3");
		break;
	}
	display_set_cursor(0,0);
	display_printf("%d", getTaste());
	delay5ms(50);							// Tastenprellen abfangen
	dummy = AT91C_BASE_PIOA->PIO_ISR;		// Bestätigung Interrupt Request
}
//**************************************************************
// main()
//**************************************************************
int main(void){
	AT91C_BASE_PMC->PMC_PCER =(1<<AT91C_ID_PIOA);
	display_init();
	usart0_init();
	pioa_ir_init((Taste1|Taste2|Taste3), 4, 3, pioa_isr);
	while(1) {
//		while(!usart0_kbhit());
//		display_set_cursor(0,0);
//		int c = usart0_getc();
//		display_putchar(c);
//		delay5ms(10);
	}
	while(1);

}
