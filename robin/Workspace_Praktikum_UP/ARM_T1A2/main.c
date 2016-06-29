//********************************************************************
// ARM_T1A1										Version 02.04.2012 hpw
//********************************************************************
#include	"include/AT91SAM7S64.h"			// Definition von ARM7 typischen Registern etc.

#define		LED1	AT91C_PIO_PA30			// Parallel Input Output Control Pin 30
#define		LED2	AT91C_PIO_PA2			// Parallel Input Output Control Pin 2
#define		LED3	AT91C_PIO_PA28			// Parallel Input Output Control Pin 28
#define		LED4	AT91C_PIO_PA17			// Parallel Input Output Control Pin 17
#define		LED5	AT91C_PIO_PA18			// Parallel Input Output Control Pin 18

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

	AT91C_BASE_PIOA->PIO_OER = (LED1 | LED2 | LED3 | LED4 | LED5);
	AT91C_BASE_PIOA->PIO_OWER = (LED1 | LED2 | LED3 | LED4 | LED5);
	AT91C_BASE_PIOA->PIO_PPUDR = (LED1 | LED2 | LED3 | LED4 | LED5);

	AT91C_BASE_PIOA->PIO_ODSR |= (LED1 | LED3 | LED5);
	while (ucB--){
		AT91C_BASE_PIOA->PIO_ODSR ^= (LED1 | LED2 | LED3 | LED4 | LED5);
		delay5ms(100);						// Verzögerung von 500ms
	}

	while (1);								// Endlosschleife nach eigentlichem
											//  Programm: niemals vergessen!
}
