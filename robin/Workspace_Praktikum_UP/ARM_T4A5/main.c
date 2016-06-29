//********************************************************************
// ARM_T4A4										Version 02.04.2012 hpw
//********************************************************************
#include	"include/AT91SAM7S64.h"			// Definition von ARM7 typischen Registern etc.
#include	"include/interrupt.h"			// IR-Routinen
#include 	"include/cToolMod.h"

#define		PH2			AT91C_PIO_PA23		// Alle Ports die f�r den Motor verwendet werden
#define		PH1			AT91C_PIO_PA13
#define		I21			AT91C_PIO_PA20
#define		I11			AT91C_PIO_PA14
#define		I20			AT91C_PIO_PA16
#define		I10			AT91C_PIO_PA15
#define		MOTOR_MASK	(PH2|PH1|I21|I11|I20|I10)

#define		T1			AT91C_PIO_PA1		// Deklaration der Taste1
#define		T2			AT91C_PIO_PA0		// Parallel Input Output Control Pin
#define		T3			AT91C_PIO_PA31		// Parallel Input Output Control Pin


int global_LR = 1;
int global_delay = 20;

//**************************************************************
// Interrupt-Service-Routine PIOA
//**************************************************************
__attribute__ ((used)) void pioa_isr (void){

	volatile unsigned int dummy;			// Lokale dummy-Variable als volatile deklariert
	static unsigned char on=0;				// Variable Start/Stop



	switch (getTaste()) {
	case 2:
		if(on){											// Motor aus
			AT91C_BASE_TC1->TC_CCR=AT91C_TC_CLKDIS;	// CLK aus
			on=0;									// Start/Stop auf 0
		} else										// Motor an
		{											// Start/Stop auf 1
			AT91C_BASE_TC1->TC_CCR=AT91C_TC_CLKEN;	// CLK ein
			AT91C_BASE_TC1->TC_CCR=AT91C_TC_SWTRG;	// Zur�cksetzen des Z�hlers
			on=1;
		}
		break;
	case 1:
		global_LR = -1;
		break;
	case 4:
		global_LR = 1;
		break;
	case 3:
		global_delay--;
		break;
	case 6:
		global_delay++;
		break;
	default:
		break;
	}


	delay5ms(50);							// Tastenprellen abfangen

	dummy = AT91C_BASE_PIOA->PIO_ISR;		// Best�tigung Interrupt Request
}

//**************************************************************
// Interrupt-Service-Routine Timer1
//**************************************************************
__attribute__ ((used)) void timer1_isr (void){
	volatile unsigned int dummy;			// Lokale dummy-Variable
	static unsigned char i=0;				// Laufvariable
	const unsigned int cRECHTS[8]={
			(PH2 | I10 | I11),
			(PH2),
			(I20 | I21),
			0,
			(PH1 | I10 | I11),
			(PH1),
			(PH2 | I20 | I21 | PH1),
			(PH1 | PH2)
	};

	// Ausgabe der Schnittstellensignale f�r die Schrittmotorsteuerung
	AT91C_BASE_PIOA->PIO_ODSR = cRECHTS[i % 8];
	//i++;
	i += global_LR;
	delay5ms(global_delay);

	//AT91C_BASE_TC1->TC_CCR=AT91C_TC_SWTRG;	// Zur�cksetzen des Z�hlers
	dummy = AT91C_BASE_TC1->TC_SR;			// Best�tigung Interrupt Request
}

//**************************************************************
// main
//**************************************************************
int main(){
	AT91C_BASE_PMC->PMC_PCER=(1<<AT91C_ID_PIOA);	// PIO-Takt einschalten

	AT91C_BASE_PIOA->PIO_OER=MOTOR_MASK;	// Freigabe der Motor-Port-Pins
	AT91C_BASE_PIOA->PIO_OWER=MOTOR_MASK;	// Freigabe des schreibenden/lesenden
	// Zugriffs auf Motor-Port-Pins �ber
	// Register PIO_ODSR
	AT91C_BASE_PIOA->PIO_PPUDR=MOTOR_MASK;	// Ausschalten der Pull-up-Widerst�nde

	//timer_ir_init(1, 4, 3, timer1_isr);		// Timer1-IR initialisieren
	timer_ir_init(1, 4, 3 | AT91C_TC_WAVESEL_UP_AUTO, timer1_isr);
	AT91C_BASE_TC1->TC_RC  = 7031;			// Max. Z�hlwert TC_RC
	AT91C_BASE_TC1->TC_CCR=AT91C_TC_SWTRG;	// Zur�cksetzen des Z�hlers

	pioa_ir_init((T1|T2|T3), 4, 3, pioa_isr);		// Port-IR initialisieren

	while (1);								// Endlosschleife
}
