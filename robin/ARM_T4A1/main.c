//********************************************************************
// ARM_T4A1										Version 02.04.2012 hpw
//********************************************************************
#include	"include/AT91SAM7S64.h"					// Definition von ARM7 typischen Registern etc.

#define		PH2			AT91C_PIO_PA23				// Alle Ports die f�r den Motor verwendet werden
#define		PH1			AT91C_PIO_PA13
#define		I21			AT91C_PIO_PA20
#define		I11			AT91C_PIO_PA14
#define		I20			AT91C_PIO_PA16
#define		I10			AT91C_PIO_PA15
#define		MOTOR_MASK	(PH2|PH1|I21|I11|I20|I10)

//**************************************************************
// delay2ms(uiK) verz�gert um ca. uiK * 2ms
//**************************************************************
void delay2ms(unsigned int uiK){
	volatile unsigned int uiI;

	while (uiK--)							// folgende for-Schleife wird uiK-mal aufgerufen
		for (uiI=0; uiI<3200; uiI++);		//  for-Schleife wird 3200-mal durchlaufen. Dies
											//  verursacht eine Zeitverz�gerung von ca. 2ms.
}

//**************************************************************
// main l��t den Schrittmotor durchgehend nach rechts laufen
//**************************************************************
int main(){
	unsigned int cRECHTS[8]={				// Tabelle f�r Halbschrittbetrieb
												// Schritt 1
												// Schritt 2
												// Schritt 3
												// Schritt 4
												// Schritt 5
												// Schritt 6
												// Schritt 7
												// Schritt 8
	};

	AT91C_BASE_PIOA->PIO_OER=MOTOR_MASK;	// Freigabe der Motor-Port-Pins
	AT91C_BASE_PIOA->PIO_OWER=MOTOR_MASK;	// Freigabe des schreibenden/lesenden
											// Zugriffs auf Motor-Port-Pins �ber
											// Register PIO_ODSR
	AT91C_BASE_PIOA->PIO_PPUDR=MOTOR_MASK;	// Ausschalten der Pull-up-Widerst�nde

											// Ab hier: Schrittausgabe an die Ports

}
