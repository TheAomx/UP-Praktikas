//********************************************************************
// ARM_T4A4										Version 02.04.2012 hpw
//********************************************************************
#include	"include/AT91SAM7S64.h"			// Definition von ARM7 typischen Registern etc.
#include	"include/interrupt.h"			// IR-Routinen
#include "display.h"

#define SPEAKER_PIN AT91C_PIO_PA18

typedef unsigned int uint_t;

#define BUTTON1 AT91C_PIO_PA1
#define BUTTON2 AT91C_PIO_PA0
#define BUTTON3 AT91C_PIO_PA31

#define ARRAY_LENGTH(X) (sizeof(X)/sizeof(X[0]))


//**************************************************************
// delay5ms(uiK) verzögert um ca. uiK * 5ms
//**************************************************************
void delay_ms(unsigned int uiK) {
	volatile unsigned int uiI;

	while (uiK--)				// folgende for-Schleife wird uiK-mal aufgerufen
		for (uiI = 0; uiI < 1600; uiI++)
			;		//  for-Schleife wird 1600-mal durchlaufen. Dies
					//  verursacht eine Zeitverzögerung von ca. 1ms.
}

unsigned char is_button_pressed(unsigned int button) {
	return !(AT91C_BASE_PIOA->PIO_PDSR & button);
}
unsigned char get_button_state() {
	unsigned char retValue = 0;

	if (is_button_pressed(BUTTON1)) {
		retValue += 1;
	}

	if (is_button_pressed(BUTTON2)) {
		retValue += 2;
	}

	if (is_button_pressed(BUTTON3)) {
		retValue += 4;
	}

	return retValue;
}


//**************************************************************
// Interrupt-Service-Routine PIOA
//**************************************************************
__attribute__ ((used)) void pioa_isr (void){

	volatile unsigned int dummy;			// Lokale dummy-Variable als volatile deklariert

	dummy = AT91C_BASE_PIOA->PIO_ISR;		// Bestätigung Interrupt Request
}


//**************************************************************
// Interrupt-Service-Routine Timer1
//**************************************************************
__attribute__ ((used)) void timer1_isr (void){
	volatile unsigned int dummy;			// Lokale dummy-Variable

	uint_t current_state_speaker = AT91C_BASE_PIOA->PIO_ODSR & SPEAKER_PIN;
	uint_t current_state = AT91C_BASE_PIOA->PIO_ODSR & (~SPEAKER_PIN);
	current_state |= ~current_state_speaker;
	AT91C_BASE_PIOA->PIO_ODSR = current_state;

	AT91C_BASE_TC1->TC_CCR=AT91C_TC_SWTRG;	// Zurücksetzen des Zählers
	dummy = AT91C_BASE_TC1->TC_SR;			// Bestätigung Interrupt Request
}

//**************************************************************
// main
//**************************************************************
int main(){
	AT91C_BASE_PMC->PMC_PCER=(1<<AT91C_ID_PIOA);	// PIO-Takt einschalten
	AT91C_BASE_PIOA->PIO_OER=SPEAKER_PIN;	// Freigabe der Motor-Port-Pins
	AT91C_BASE_PIOA->PIO_OWER=SPEAKER_PIN;	// Freigabe des schreibenden/lesenden
											// Zugriffs auf Motor-Port-Pins über
											// Register PIO_ODSR
	AT91C_BASE_PIOA->PIO_PPUDR=SPEAKER_PIN;	// Ausschalten der Pull-up-Widerstände

	timer_ir_init(1, 4, 1, timer1_isr);		// Timer1-IR initialisieren
	AT91C_BASE_TC1->TC_RC = 6818;		// Max. Zählwert TC_RC
	AT91C_BASE_TC1->TC_CCR=AT91C_TC_SWTRG;	// Zurücksetzen des Zählers
/*
	pioa_ir_init(BUTTON1, 4, 3, pioa_isr);		// Port-IR initialisieren
	pioa_ir_init(BUTTON2, 4, 3, pioa_isr);
	pioa_ir_init(BUTTON3, 4, 3, pioa_isr);
*/
	display_init();

	while (1) {

	}
}
