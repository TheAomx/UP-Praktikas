//********************************************************************
// ARM_T1A14									Version 02.04.2012 hpw
//********************************************************************
#include	"include/AT91SAM7S64.h"			// Definition von ARM7 typischen Registern etc.
#include 	"include/cToolMod.h"
#include 	"include/display.h"
#include 	<stdio.h>

#define		BUTTON1	AT91C_PIO_PA1			// Parallel Input Output Control Pin 30
#define		BUTTON2	AT91C_PIO_PA0 			// Parallel Input Output Control Pin 2
#define		BUTTON3	AT91C_PIO_PA31			// Parallel Input Output Control Pin 28




//**************************************************************
// main läßt LED am Port P30 für ca. 1 Minute mit 1 Hz blinken
//**************************************************************
int main(){
	AT91C_BASE_PMC->PMC_PCER = (1<<AT91C_ID_PIOA); // wichtig;

	unsigned char ucB=0;					// lokale Variable ucB
	char buf[10];
	display_init();

	while(1) {
		ucB++;
		unsigned int t = getTaste();
		sprintf(buf,"%u", t);
		display_set_cursor(0,0);
		display_putstr(buf);
		display_set_cursor(0,1);
		display_printf("%d", t);
	}

	while (1);								// Endlosschleife nach eigentlichem
											//  Programm: niemals vergessen!
}
