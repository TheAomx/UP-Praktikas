//********************************************************************
// ARM_T4A1										Version 02.04.2012 hpw
//********************************************************************
#include	"include/AT91SAM7S64.h"					// Definition von ARM7 typischen Registern etc.
#include 	"include/cToolMod.h"
#include 	"include/display.h"

#define		PH2			AT91C_PIO_PA23				// Alle Ports die für den Motor verwendet werden
#define		PH1			AT91C_PIO_PA13
#define		I21			AT91C_PIO_PA20
#define		I11			AT91C_PIO_PA14
#define		I20			AT91C_PIO_PA16
#define		I10			AT91C_PIO_PA15
#define		MOTOR_MASK	(PH2|PH1|I21|I11|I20|I10)

//**************************************************************
// delay2ms(uiK) verzögert um ca. uiK * 2ms
//**************************************************************
void delay2ms(unsigned int uiK){
	volatile unsigned int uiI;

	while (uiK--)							// folgende for-Schleife wird uiK-mal aufgerufen
		for (uiI=0; uiI<3200; uiI++);		//  for-Schleife wird 3200-mal durchlaufen. Dies
											//  verursacht eine Zeitverzögerung von ca. 2ms.
}

//**************************************************************
// main läßt den Schrittmotor durchgehend nach rechts laufen
//**************************************************************
int main(){
//	unsigned int cRECHTS[8]={		// Tabelle für Halbschrittbetrieb
//			0x0080C000,				// Schritt 1
//			0x00800000,				// Schritt 2
//			0x00110000,				// Schritt 3
//			0x00000000,				// Schritt 4
//			0x0000E000,				// Schritt 5
//			0x00002000,				// Schritt 6
//			0x00912000,				// Schritt 7
//			0x00802000				// Schritt 8
//	};

	unsigned int cRECHTS2[8]= {
			(PH2 | I10 | I11),
			(PH2),
			(I20 | I21),
			0,
			(PH1 | I10 | I11),
			(PH1),
			(PH2 | I20 | I21 | PH1),
			(PH1 | PH2)
	};
	display_init();
	AT91C_BASE_PMC->PMC_PCER = (1<<AT91C_ID_PIOA); // Button aktivieren
	AT91C_BASE_PIOA->PIO_OER=MOTOR_MASK;	// Freigabe der Motor-Port-Pins
	AT91C_BASE_PIOA->PIO_OWER=MOTOR_MASK;	// Freigabe des schreibenden/lesenden
											// Zugriffs auf Motor-Port-Pins über
											// Register PIO_ODSR
	AT91C_BASE_PIOA->PIO_PPUDR=MOTOR_MASK;	// Ausschalten der Pull-up-Widerstände
	unsigned char i = 0;
	signed char LR = -1;		//Bestimmt Drehrichtung -1 : L, 1: R
	unsigned char start = 1;
	unsigned int delay = 1;

	while(1) {
		AT91C_BASE_PIOA->PIO_ODSR = cRECHTS2[i % 8];
		i += LR * start;
		switch (getTaste()) {
			case 2:
				(start == 0) ? (start = 1) : (start = 0);
				break;
			case 1:
				(delay < 50) ? delay++ : (delay = 50);
				break;
			case 4:
				(delay > 0) ? delay-- : (delay = 1);
				break;
			case 3:
				LR = 1;
				break;
			case 6:
				LR = -1;
				break;
			default:
				break;
		}
		display_set_cursor(0, 0);
		display_printf("%d", delay);
		delay2ms(delay);
	}


											// Ab hier: Schrittausgabe an die Ports

}
