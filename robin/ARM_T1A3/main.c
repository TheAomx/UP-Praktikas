//********************************************************************
// ARM_T1A1										Version 02.04.2012 hpw
//********************************************************************
#include	"include/AT91SAM7S64.h"			// Definition von ARM7 typischen Registern etc.
#include 	"include/cToolMod.h"

#define		LED1	AT91C_PIO_PA30			// Parallel Input Output Control Pin 30
#define		LED2	AT91C_PIO_PA2 			// Parallel Input Output Control Pin 2
#define		LED3	AT91C_PIO_PA28			// Parallel Input Output Control Pin 28
#define		LED4	AT91C_PIO_PA17			// Parallel Input Output Control Pin 30
#define		LED5	AT91C_PIO_PA18			// Parallel Input Output Control Pin 30




//**************************************************************
// main l��t LED am Port P30 f�r ca. 1 Minute mit 1 Hz blinken
//**************************************************************
int main(){
	unsigned char ucB=120;					// lokale Variable ucB
	unsigned int mask = (LED1 | LED2 | LED3 | LED4 | LED5);

	AT91C_BASE_PIOA->PIO_OER = mask;		// Freigabe des LED-Port-Pins
	AT91C_BASE_PIOA->PIO_OWER = mask;		// Register: Schreib-Freigabe des Output Write Enable Register
	AT91C_BASE_PIOA->PIO_PPUDR = mask;		// Ausschalten des Pull-up-Widerstandes

	AT91C_BASE_PIOA->PIO_ODSR &= ~mask;

	AT91C_BASE_PIOA->PIO_ODSR = (LED1 | LED3 | LED5);
	while (ucB--){
		AT91C_BASE_PIOA->PIO_ODSR ^= mask;	// LED1 ein
		delay5ms(100);						// Verz�gerung von 500ms
	}

	while (1);								// Endlosschleife nach eigentlichem
											//  Programm: niemals vergessen!
}
