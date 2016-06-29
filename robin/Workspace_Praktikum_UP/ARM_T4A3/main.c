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
	unsigned int cRECHTS[8]= {
				(PH2 | I10 | I11),
				(PH2),
				(I20 | I21),
				0,
				(PH1 | I10 | I11),
				(PH1),
				(PH2 | I20 | I21 | PH1),
				(PH1 | PH2)
		};
	AT91C_BASE_PIOA->PIO_ODSR = cRECHTS[i % 8];
	i++;
	// Ausgabe der Schnittstellensignale für die Schrittmotorsteuerung


	// auskommentiert wegen AT91C_TC_WAVESEL_UP_AUTO
	//	AT91C_BASE_TC1->TC_CCR=AT91sC_TC_SWTRG;	// Zurücksetzen des Zählers
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

	//AT91C_TC_WAVESEL_UP_AUTO für selbstängides timerzurücksetzen
	timer_ir_init(1, 4, 3 | AT91C_TC_WAVESEL_UP_AUTO, timer1_isr);		// Timer1-IR initialisieren
	AT91C_BASE_TC1->TC_RC = 7031;		// Max. Zählwert TC_RC  (15s/400Schritte)*187,5kHz
	AT91C_BASE_TC1->TC_CCR=AT91C_TC_SWTRG;	// Zurücksetzen des Zählers

	while (1);								// Endlosschleife
}
