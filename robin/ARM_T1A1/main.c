//********************************************************************
// ARM_T1A1										Version 02.04.2012 hpw
//********************************************************************
#include	"include/AT91SAM7S64.h"			// Definition von ARM7 typischen Registern etc.

#define		LED1	AT91C_PIO_PA30			// Parallel Input Output Control Pin 30


//**************************************************************
// delay5ms(uiK) verzögert um ca. uiK * 5ms
//**************************************************************
void delay5ms(unsigned int uiK){
	volatile unsigned int uiI;

	while (uiK--)							// folgende for-Schleife wird uiK-mal aufgerufen
		for (uiI=0; uiI<8192; uiI++);		//  for-Schleife wird 8192-mal durchlaufen. Dies
											//  verursacht eine Zeitverzögerung von ca. 5ms.
}

//**************************************************************
// main läßt LED am Port P30 für ca. 1 Minute mit 1 Hz blinken
//**************************************************************
int main(){
	unsigned char ucB=60;					// lokale Variable ucB

	AT91C_BASE_PIOA->PIO_OER = LED1;		// Freigabe des LED-Port-Pins
	AT91C_BASE_PIOA->PIO_PPUDR = LED1;		// Ausschalten des Pull-up-Widerstandes

	while (ucB--){
		AT91C_BASE_PIOA->PIO_SODR = LED1;	// LED1 ein
		delay5ms(100);						// Verzögerung von 500ms
		AT91C_BASE_PIOA->PIO_CODR = LED1;	// LED1 aus
		delay5ms(100);						// Verzögerung von 500ms
	}

	while (1);								// Endlosschleife nach eigentlichem
											//  Programm: niemals vergessen!
}
