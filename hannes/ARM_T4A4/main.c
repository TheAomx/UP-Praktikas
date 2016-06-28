//********************************************************************
// ARM_T4A4										Version 02.04.2012 hpw
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

#define PUSH1 AT91C_PIO_PA1
#define PUSH2 AT91C_PIO_PA0
#define PUSH3 AT91C_PIO_PA31

#define STATE_MAX 3 	// 0 ...7 array indexes

#define STEPS_360 400.0
#define TIME_FRRQ 187.5
#define ROUND_TIME_DEFAULT 15.0

#define ROUND_TIME_MAX 20.0 	// times 2ms delay max
#define ROUND_TIME_MIN 1.0 	// times 2ms delay min

enum {
	LEFT, RIGHT
};

#define T_STEP(step_min, frequ, t_round) (unsigned int) ((t_round / step_min) * 1000) / (1 / frequ);

static volatile unsigned int round_time = ROUND_TIME_DEFAULT;
static volatile unsigned int direction = LEFT;

//**************************************************************
// delay5ms(uiK) verzögert um ca. uiK * 5ms
//**************************************************************
void delay5ms(unsigned int uiK) {
	volatile unsigned int uiI;

	while (uiK--)				// folgende for-Schleife wird uiK-mal aufgerufen
		for (uiI = 0; uiI < 8192; uiI++)
			;		//  for-Schleife wird 8192-mal durchlaufen. Dies
					//  verursacht eine Zeitverzögerung von ca. 5ms.
}

//**************************************************************
// Interrupt-Service-Routine PIOA
//**************************************************************
__attribute__ ((used)) void pioa_isr(void) {

	volatile unsigned int dummy;// Lokale dummy-Variable als volatile deklariert
	static unsigned char on = 0;					// Variable Start/Stop

	if (on) {										// Motor aus
		AT91C_BASE_TC1->TC_CCR = AT91C_TC_CLKDIS;	// CLK aus
		on = 0;
	}												// Start/Stop auf 0
	else {											// Motor an
		AT91C_BASE_TC1->TC_CCR = AT91C_TC_CLKEN;	// CLK ein
		AT91C_BASE_TC1->TC_CCR = AT91C_TC_SWTRG;	// Zurücksetzen des Zählers
		on = 1;
	}
	//round_time = ROUND_TIME_MIN;
	//AT91C_BASE_TC1->TC_RC = T_STEP(STEPS_360, TIME_FRRQ, round_time)
	delay5ms(50);								// Tastenprellen abfangen
	dummy = AT91C_BASE_PIOA->PIO_ISR;			// Bestätigung Interrupt Request
}

__attribute__ ((used)) void pioa_t2_isr(void) {
	volatile unsigned int dummy;// Lokale dummy-Variable als volatile deklariert
	if (++round_time > ROUND_TIME_MAX)
		round_time = ROUND_TIME_MAX;
	AT91C_BASE_TC1->TC_RC = T_STEP(STEPS_360, TIME_FRRQ, round_time)
	delay5ms(50);								// Tastenprellen abfangen
	dummy = AT91C_BASE_PIOA->PIO_ISR;			// Bestätigung Interrupt Request
}

__attribute__ ((used)) void pioa_t3_isr(void) {
	volatile unsigned int dummy;// Lokale dummy-Variable als volatile deklariert
	if (direction == RIGHT)
		direction = LEFT;
	else if (direction == LEFT)
		direction = RIGHT;
	delay5ms(50);								// Tastenprellen abfangen
	dummy = AT91C_BASE_PIOA->PIO_ISR;			// Bestätigung Interrupt Request
}

__attribute__ ((used)) void timer1_isr(void) {
	volatile unsigned int dummy;			// Lokale dummy-Variable
	static volatile int current_state = 0;			// Laufvariable

	unsigned int transition[4] = { 	// Tabelle für Halbschrittbetrieb
			I10 | I11, 				// Schritt 1|2, 5|6
			I21 | I20 | PH2,		// Schritt 2|3, 6|7
			I21 | I20,				// Schritt 3|4, 7|8
			I10 | I11 | PH1, 		// Schritt 4|5, 8|1
	};

	if (direction == LEFT && --current_state < 0)
		current_state = STATE_MAX;

	AT91C_BASE_PIOA->PIO_ODSR ^= transition[current_state];

	if (direction == RIGHT && ++current_state > STATE_MAX)
		current_state = 0;

	AT91C_BASE_TC1->TC_CCR = AT91C_TC_SWTRG;	// Zurücksetzen des Zählers
	dummy = AT91C_BASE_TC1->TC_SR;				// Bestätigung Interrupt Request
}

//**************************************************************
// main
//**************************************************************
int main() {
	AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_PIOA);	// PIO-Takt einschalten
	AT91C_BASE_PIOA->PIO_OER = MOTOR_MASK;		// Freigabe der Motor-Port-Pins
	AT91C_BASE_PIOA->PIO_OWER = MOTOR_MASK;	// Freigabe des schreibenden/lesenden
	// Zugriffs auf Motor-Port-Pins über
	// Register PIO_ODSR
	AT91C_BASE_PIOA->PIO_PPUDR = MOTOR_MASK;// Ausschalten der Pull-up-Widerstände

	// Set initial State 1
	AT91C_BASE_PIOA->PIO_CODR = I10 | I11 | PH2;
	AT91C_BASE_PIOA->PIO_SODR = PH1 | I21 | I20;

	timer_ir_init(1, 4, 3, timer1_isr);		// Timer1-IR initialisieren
	// Max. Zählwert TC_RC
	AT91C_BASE_TC1->TC_RC = T_STEP(STEPS_360, TIME_FRRQ, round_time)

	AT91C_BASE_TC1->TC_CCR = AT91C_TC_SWTRG;	// Zurücksetzen des Zählers

	pioa_ir_init(PUSH1, 4, 3, pioa_isr);		// Port-IR initialisieren
	pioa_ir_init(PUSH2, 4, 3, pioa_t2_isr);		// Port-IR initialisieren
	pioa_ir_init(PUSH3, 4, 3, pioa_t3_isr);		// Port-IR initialisieren

	while (1)
		;								// Endlosschleife
}
