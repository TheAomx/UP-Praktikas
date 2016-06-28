//********************************************************************
// ARM_T4A3										Version 02.04.2012 hpw
//********************************************************************
#include	"include/AT91SAM7S64.h"			// Definition von ARM7 typischen Registern etc.
#include	"include/interrupt.h"			// IR-Routinen

#define		PH2			AT91C_PIO_PA23		// Alle Ports die für den Motor verwendet werden
#define		PH1			AT91C_PIO_PA13
#define		I21			AT91C_PIO_PA20
#define		I11			AT91C_PIO_PA14
#define		I20			AT91C_PIO_PA16
#define		I10			AT91C_PIO_PA15
#define		MOTOR_MASK	(PH2|PH1|I21|I11|I20|I10)


//**************************************************************
// Interrupt-Service-Routine Timer1
//**************************************************************
__attribute__ ((used)) void timer1_isr (void){
	volatile unsigned int dummy;			// Lokale dummy-Variable
	static unsigned char i=0;				// Laufvariable
	const unsigned int cRECHTS[8]={			// Tabelle für Halbschrittbetrieb
												// Schritt 1
												// Schritt 2
												// Schritt 3
												// Schritt 4
												// Schritt 5
												// Schritt 6
												// Schritt 7
												// Schritt 8
	};

	// Ausgabe der Schnittstellensignale für die Schrittmotorsteuerung

	AT91C_BASE_TC1->TC_CCR=AT91C_TC_SWTRG;	// Zurücksetzen des Zählers
	dummy = AT91C_BASE_TC1->TC_SR;			// Bestätigung Interrupt Request
}

//**************************************************************
// main initialisiert Timer 1 für Schrittmoterdrehungen
//**************************************************************
int main(){
	AT91C_BASE_PIOA->PIO_OER=MOTOR_MASK;	// Freigabe der Motor-Port-Pins
	AT91C_BASE_PIOA->PIO_OWER=MOTOR_MASK;	// Freigabe des schreibenden/lesenden
											// Zugriffs auf Motor-Port-Pins über
											// Register PIO_ODSR
	AT91C_BASE_PIOA->PIO_PPUDR=MOTOR_MASK;	// Ausschalten der Pull-up-Widerstände

	timer_ir_init(1, 4, 3, timer1_isr);		// Timer1-IR initialisieren
	AT91C_BASE_TC1->TC_RC  = 		;		// Max. Zählwert TC_RC
	AT91C_BASE_TC1->TC_CCR=AT91C_TC_SWTRG;	// Zurücksetzen des Zählers

	while (1);								// Endlosschleife
}
