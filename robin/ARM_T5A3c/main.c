//********************************************************************
// ARM_T5A3c									Version 02.04.2012 hpw
//********************************************************************
#include	"include/AT91SAM7S64.h"			// Definition von ARM7 typischen Registern etc.
#include	"include/serial.h"
#include	"include/display.h"

#define		Taste1	AT91C_PIO_PA1
#define		Taste2	AT91C_PIO_PA0
#define		Taste3	AT91C_PIO_PA31

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
// main()
//**************************************************************
int main(void){

}
