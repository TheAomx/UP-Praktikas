/*
 * cToolMod.

 *
 *  Created on: 27.06.2016
 *      Author: Robin
 */

#include "cToolMod.h"
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
// delay1ms(uiK) verzögert um ca. uiK * 1ms
//**************************************************************
void delay1ms(unsigned int uiK){
	volatile unsigned int uiI;

	while (uiK--)							// folgende for-Schleife wird uiK-mal aufgerufen
		for (uiI=0; uiI<1638; uiI++);		//  for-Schleife wird 1638-mal durchlaufen. Dies
											//  verursacht eine Zeitverzögerung von ca. 1ms.
}


unsigned int getTaste() {
	unsigned int reg = AT91C_BASE_PIOA->PIO_PDSR;
	return !(reg & BUTTON1) |
			!(reg & BUTTON2) << 1 |
					!(reg & BUTTON3) << 2;
}
