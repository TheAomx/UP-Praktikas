//********************************************************************
// ARM_T4A4										Version 02.04.2012 hpw
//********************************************************************
#include	"include/AT91SAM7S64.h"			// Definition von ARM7 typischen Registern etc.
#include	"include/interrupt.h"			// IR-Routinen
#include "display.h"

#define		PH2			AT91C_PIO_PA23		// Alle Ports die für den Motor verwendet werden
#define		PH1			AT91C_PIO_PA13
#define		I21			AT91C_PIO_PA20
#define		I11			AT91C_PIO_PA14
#define		I20			AT91C_PIO_PA16
#define		I10			AT91C_PIO_PA15
#define		MOTOR_MASK	(PH2|PH1|I21|I11|I20|I10)

typedef unsigned int uint_t;

#define BUTTON1 AT91C_PIO_PA1
#define BUTTON2 AT91C_PIO_PA0
#define BUTTON3 AT91C_PIO_PA31


#define MIN_SPEED 250
#define MAX_SPEED 8000
#define SPEED_STEPS 50
#define DELTA_SPEED (MAX_SPEED-MIN_SPEED) / SPEED_STEPS
#define INITIAL_SPEED 469

enum Direction {
	LEFT = -1, RIGHT = 1
};

uint_t const MOTOR_STEPS[8] = {
	// Tabelle für Halbschrittbetrieb
	0x0080C000,										// Schritt 1
	0x00800000,										// Schritt 2
	0x00110000,										// Schritt 3
	0x00000000,										// Schritt 4
	0x0000E000,										// Schritt 5
	0x00002000,										// Schritt 6
	0x00912000,										// Schritt 7
	0x00802000,										// Schritt 8
};

enum Direction direction = RIGHT;

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

int get_motor_speed(int adjustment) {
	static int motor_speed = INITIAL_SPEED;
	int new_speed = motor_speed + adjustment;
	if (new_speed < MIN_SPEED) {
		motor_speed = MIN_SPEED;
	} else if (new_speed > MAX_SPEED) {
		motor_speed = MAX_SPEED;
	} else {
		motor_speed = new_speed;
	}
	return motor_speed;
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
	static unsigned char on=0;				// Variable Start/Stop
	unsigned char button_state = get_button_state();

	switch (button_state) {
	case 1:
		AT91C_BASE_TC1->TC_RC = get_motor_speed(DELTA_SPEED);
		break;
	case 2:
		if (on) {									// Motor aus
			AT91C_BASE_TC1->TC_CCR = AT91C_TC_CLKDIS;	// CLK aus
			on = 0;
		}else {									// Motor an
			AT91C_BASE_TC1->TC_CCR = AT91C_TC_CLKEN;	// CLK ein
			AT91C_BASE_TC1->TC_CCR = AT91C_TC_SWTRG;// Zurücksetzen des Zählers
			on = 1;
		}									// Start/Stop auf 1
		break;
	case 3:
		direction = LEFT;
		break;
	case 4:
		AT91C_BASE_TC1->TC_RC = get_motor_speed(-DELTA_SPEED);
		break;
	case 6:
		direction = RIGHT;
	}

	dummy = AT91C_BASE_PIOA->PIO_ISR;		// Bestätigung Interrupt Request
}

void motor_move(enum Direction motor_direction) {
	static int index = 0;
	int tmp;
	tmp = index + motor_direction;
	if (tmp < 0) {
		index = ARRAY_LENGTH(MOTOR_STEPS) - 1;
	} else if (tmp >= ARRAY_LENGTH(MOTOR_STEPS)) {
		index = 0;
	} else {
		index = tmp;
	}

	uint_t current_state = AT91C_BASE_PIOA->PIO_ODSR;

	uint_t not_mask = ~MOTOR_MASK;
	uint_t next_state = current_state & not_mask;
	next_state |= MOTOR_STEPS[index];
	AT91C_BASE_PIOA->PIO_ODSR = next_state;
}


//**************************************************************
// Interrupt-Service-Routine Timer1
//**************************************************************
__attribute__ ((used)) void timer1_isr (void){
	volatile unsigned int dummy;			// Lokale dummy-Variable

	motor_move(direction);

	AT91C_BASE_TC1->TC_CCR=AT91C_TC_SWTRG;	// Zurücksetzen des Zählers
	dummy = AT91C_BASE_TC1->TC_SR;			// Bestätigung Interrupt Request
}

//**************************************************************
// main
//**************************************************************
int main(){
	AT91C_BASE_PMC->PMC_PCER=(1<<AT91C_ID_PIOA);	// PIO-Takt einschalten
	AT91C_BASE_PIOA->PIO_OER=MOTOR_MASK;	// Freigabe der Motor-Port-Pins
	AT91C_BASE_PIOA->PIO_OWER=MOTOR_MASK;	// Freigabe des schreibenden/lesenden
											// Zugriffs auf Motor-Port-Pins über
											// Register PIO_ODSR
	AT91C_BASE_PIOA->PIO_PPUDR=MOTOR_MASK;	// Ausschalten der Pull-up-Widerstände

	timer_ir_init(1, 4, 3, timer1_isr);		// Timer1-IR initialisieren
	AT91C_BASE_TC1->TC_RC = get_motor_speed(0);		// Max. Zählwert TC_RC
	AT91C_BASE_TC1->TC_CCR=AT91C_TC_SWTRG;	// Zurücksetzen des Zählers

	pioa_ir_init(BUTTON1, 4, 3, pioa_isr);		// Port-IR initialisieren
	pioa_ir_init(BUTTON2, 4, 3, pioa_isr);
	pioa_ir_init(BUTTON3, 4, 3, pioa_isr);
	display_init();

	while (1) {
		display_printf("Speed: %d", get_motor_speed(0));
		display_set_cursor(0, 1);
		display_putstr(((direction == LEFT) ? "LEFT" : "RIGHT"));
		delay_ms(50);
		display_clear();
	}
}
