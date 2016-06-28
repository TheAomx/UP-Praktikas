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

#define STATE_MAX 3 	// 0 ...7 array indexes
#define SPEED_MAX 20 	// times 2ms delay max
#define SPEED_MIN 1 	// times 2ms delay min

// 15s pro umdrehung
// 400 steops pro umdrehung
// 15/400 = 37,5ms/pro umdrehung
// taktfrequenz des timers = 187,5
// Eine Taktperiode dauert 0.0053 ms = 1/187,5 ( 1ms(p) = 1khz),
// TC_RC =
#define STEPS_360 400.0
#define TIME_FRRQ 187.5
#define ROUND_TIME 10.0

#define T_STEP(step_min, frequ, t_round) (unsigned int) ((t_round / step_min) * 1000) / (1 / frequ);
enum {
	LEFT, RIGHT
};

//**************************************************************
// Interrupt-Service-Routine Timer1
//**************************************************************
__attribute__ ((used)) void timer1_isr(void) {
	volatile unsigned int dummy;			// Lokale dummy-Variable
	static volatile unsigned int current_state = 0, direction = RIGHT;// Laufvariable
	const unsigned  int transition[4] = { 	// Tabelle für Halbschrittbetrieb
			I10 | I11, 			// Schritt 1|2,  5|6
			I21 | I20 | PH2,	// Schritt 2|3, 6|7
			I21 | I20,			// Schritt 3|4,  7|8
			I10 | I11 | PH1, 	// Schritt 4|5,  8|1
	};
	if (direction == LEFT && --current_state < 0)
		current_state = STATE_MAX;

	AT91C_BASE_PIOA->PIO_ODSR ^= transition[current_state];

	if (direction == RIGHT && ++current_state > STATE_MAX)
		current_state = 0;

	AT91C_BASE_TC1->TC_CCR = AT91C_TC_SWTRG;	// Zurücksetzen des Zählers
	dummy = AT91C_BASE_TC1->TC_SR;			// Bestätigung Interrupt Request
}



//**************************************************************
// main initialisiert Timer 1 für Schrittmoterdrehungen
//**************************************************************
int main() {
	AT91C_BASE_PIOA->PIO_OER = MOTOR_MASK;	// Freigabe der Motor-Port-Pins
	AT91C_BASE_PIOA->PIO_OWER = MOTOR_MASK;	// Freigabe des schreibenden/lesenden
											// Zugriffs auf Motor-Port-Pins über
											// Register PIO_ODSR
	AT91C_BASE_PIOA->PIO_PPUDR = MOTOR_MASK;// Ausschalten der Pull-up-Widerstände

	// Set initial State 1
	AT91C_BASE_PIOA->PIO_CODR = I10 | I11 | PH2;
	AT91C_BASE_PIOA->PIO_SODR = PH1 | I21 | I20;

	timer_ir_init(1, 4, 3, timer1_isr);		// Timer1-IR initialisieren
	AT91C_BASE_TC1->TC_RC =  T_STEP(STEPS_360, TIME_FRRQ,	ROUND_TIME);		// Max. Zählwert TC_RC
	AT91C_BASE_TC1->TC_CCR = AT91C_TC_SWTRG;	// Zurücksetzen des Zählers

	while (1)
		;								// Endlosschleife
}
