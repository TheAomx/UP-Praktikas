//********************************************************************
// ARM_T4A3										Version 02.04.2012 hpw
//********************************************************************
#include	"include/AT91SAM7S64.h"			// Definition von ARM7 typischen Registern etc.
#include	"include/interrupt.h"			// IR-Routinen

#define 	SPEAKER		AT91C_PIO_PA18


//**************************************************************
// Interrupt-Service-Routine Timer1
//**************************************************************
__attribute__ ((used)) void timer0_isr (void){
	volatile unsigned int dummy;			// Lokale dummy-Variable
	static unsigned char i=0;				// Laufvariable



	//	AT91C_BASE_TC1->TC_CCR=AT91C_TC_SWTRG;	// Manuelles zurücksetzen des Zählers
	dummy = AT91C_BASE_TC0->TC_SR;			// Bestätigung Interrupt Request
}

//**************************************************************
// Interrupt-Service-Routine Timer1
//**************************************************************
__attribute__ ((used)) void timer1_isr (void){
	volatile unsigned int dummy;			// Lokale dummy-Variable
	static unsigned char i=0;				// Laufvariable

	AT91C_BASE_PIOA->PIO_ODSR ^= SPEAKER;


	//	AT91C_BASE_TC1->TC_CCR=AT91C_TC_SWTRG;	// Manuelles zurücksetzen des Zählers
	dummy = AT91C_BASE_TC1->TC_SR;			// Bestätigung Interrupt Request
}

//**************************************************************
// main initialisiert Timer 1 für Schrittmoterdrehungen
//**************************************************************
int main(){
	AT91C_BASE_PIOA->PIO_OER = SPEAKER;	// Freigabe der Motor-Port-Pins
	AT91C_BASE_PIOA->PIO_OWER = SPEAKER;	// Freigabe des schreibenden/lesenden
											// Zugriffs auf Motor-Port-Pins über
											// Register PIO_ODSR
	AT91C_BASE_PIOA->PIO_PPUDR=SPEAKER;	// Ausschalten der Pull-up-Widerstände

	//AT91C_TC_WAVESEL_UP_AUTO für selbstängides timerzurücksetzen
	//timer_ir_init(1, 4, 1, timer0_isr); Timer manuell setzen
	timer_ir_init(1, 4, 1 | AT91C_TC_WAVESEL_UP_AUTO, timer0_isr);
	timer_ir_init(1, 4, 1 | AT91C_TC_WAVESEL_UP_AUTO, timer1_isr);		// Timer1-IR initialisieren
	AT91C_BASE_TC0->TC_RC = 3000;		//Dauer Max. Zählwert TC_RC  (1ms * 3Mhz)
	AT91C_BASE_TC0->TC_CCR = AT91C_TC_SWTRG;
	AT91C_BASE_TC1->TC_RC = 3409;		// Ton
	AT91C_BASE_TC1->TC_CCR = AT91C_TC_SWTRG;	// Zurücksetzen des Zählers

	while (1);								// Endlosschleife
}
