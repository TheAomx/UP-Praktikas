//********************************************************************
// ARM_T4A1										Version 02.04.2012 hpw
//********************************************************************
#include	"include/AT91SAM7S64.h"					// Definition von ARM7 typischen Registern etc.
#include "display.h"

#define		PH2			AT91C_PIO_PA23				// Alle Ports die für den Motor verwendet werden
#define		PH1			AT91C_PIO_PA13
#define		I21			AT91C_PIO_PA20
#define		I11			AT91C_PIO_PA14
#define		I20			AT91C_PIO_PA16
#define		I10			AT91C_PIO_PA15
#define		MOTOR_MASK	(PH2|PH1|I21|I11|I20|I10)
#define DELTA_SPEED 1
enum Direction {
	LEFT = -1, RIGHT = 1
};

#define ARRAY_LENGTH(X) (sizeof(X)/sizeof(X[0]))
typedef unsigned int uint_t;

//**************************************************************
// delay2ms(uiK) verzögert um ca. uiK * 2ms
//**************************************************************
void delay_ms(unsigned int uiK) {
	volatile unsigned int uiI;

	while (uiK--)				// folgende for-Schleife wird uiK-mal aufgerufen
		for (uiI = 0; uiI < 1600; uiI++)
			;		//  for-Schleife wird 1600-mal durchlaufen. Dies
					//  verursacht eine Zeitverzögerung von ca. 1ms.
}

//**************************************************************
// main läßt den Schrittmotor durchgehend nach rechts laufen

#define BUTTON1 AT91C_PIO_PA1
#define BUTTON2 AT91C_PIO_PA0
#define BUTTON3 AT91C_PIO_PA31

unsigned int const MOTOR_STEPS[8] = {		// Tabelle für Halbschrittbetrieb
		0x0080C000,										// Schritt 1
				0x00800000,										// Schritt 2
				0x00110000,										// Schritt 3
				0x00000000,										// Schritt 4
				0x0000E000,										// Schritt 5
				0x00002000,										// Schritt 6
				0x00912000,										// Schritt 7
				0x00802000,										// Schritt 8
		};

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

void motor_move(int motor_speed, enum Direction motor_direction) {

	uint_t i = (motor_direction == RIGHT) ? 0 : ARRAY_LENGTH(MOTOR_STEPS) - 1;
	for (uint_t j = 0; j < ARRAY_LENGTH(MOTOR_STEPS);
			i += motor_direction, j++) {
		uint_t current_state = AT91C_BASE_PIOA->PIO_ODSR;
		uint_t not_mask = ~MOTOR_MASK;
		uint_t next_state = current_state & not_mask;
		next_state |= MOTOR_STEPS[i];
		AT91C_BASE_PIOA->PIO_ODSR = next_state;
		delay_ms(motor_speed);
	}
}

int get_motor_speed(int adjustment) {
	static int motor_speed = 2;
	int new_speed = motor_speed + adjustment;
	if (new_speed < 2) {
		motor_speed = 2;
	} else if (new_speed > 250) {
		motor_speed = 250;
	} else {
		motor_speed = new_speed;
	}
	return motor_speed;
}

int main() {
	AT91C_BASE_PIOA->PIO_OER = MOTOR_MASK;	// Freigabe der Motor-Port-Pins
	AT91C_BASE_PIOA->PIO_OWER = MOTOR_MASK;	// Freigabe des schreibenden/lesenden
											// Zugriffs auf Motor-Port-Pins über
											// Register PIO_ODSR
	AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_PIOA);
	AT91C_BASE_PIOA->PIO_PPUDR = MOTOR_MASK;// Ausschalten der Pull-up-Widerstände

	// Ab hier: Schrittausgabe an die Ports
	int running = 1;
	enum Direction current_direction = RIGHT;
	display_init();
	while (1) {
		unsigned char button_state = get_button_state();

		switch (button_state) {
		case 1:
			get_motor_speed(DELTA_SPEED);
			break;
		case 2:
			running = !running;
			break;
		case 3:
			current_direction = LEFT;
			break;
		case 4:
			get_motor_speed(-DELTA_SPEED);
			break;
		case 6:
			current_direction = RIGHT;
		}
		display_printf("Speed: %d", get_motor_speed(0));
		display_set_cursor(0, 1);
		display_putstr(((current_direction == LEFT) ? "LEFT" : "RIGHT"));
		if (running){
			motor_move(get_motor_speed(0), current_direction);
		}

		display_clear();
	}

}
