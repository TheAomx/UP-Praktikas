//********************************************************************
// ARM_T1A14									Version 02.04.2012 hpw
//********************************************************************
#include	"include/AT91SAM7S64.h"			// Definition von ARM7 typischen Registern etc.
#include 	"include/cToolMod.h"
#include 	"include/display.h"
#include 	<stdio.h>

#define		LED1	AT91C_PIO_PA30			// Parallel Input Output Control Pin 30
#define		LED2	AT91C_PIO_PA2 			// Parallel Input Output Control Pin 2
#define		LED3	AT91C_PIO_PA28			// Parallel Input Output Control Pin 28
#define		LED4	AT91C_PIO_PA17			// Parallel Input Output Control Pin 30
#define		LED5	AT91C_PIO_PA18			// Parallel Input Output Control Pin 30




//**************************************************************
// main läßt LED am Port P30 für ca. 1 Minute mit 1 Hz blinken
//**************************************************************
int main(){
	unsigned char ucB=0;					// lokale Variable ucB
	char buf[10];
	display_init();


	do {
		ucB++;

		sprintf(buf,"%x", ucB);
//		AT91C_BASE_PIOA->PIO_ODSR ^= mask;	// LED1 ein
		display_set_cursor(0,0);
		display_putstr(buf);
		display_set_cursor(0,1);
		display_putchar(ucB);
		delay5ms(10);						// Verzögerung von 500ms

	}while(ucB < 255);

	while (1);								// Endlosschleife nach eigentlichem
											//  Programm: niemals vergessen!
}
