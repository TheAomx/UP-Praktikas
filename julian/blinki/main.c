#include "include/AT91SAM7S64.h"							// Register-Deklarationen
#include "cToolMod.h"
#include "display.h"
#define TYPICAL
//#define POINTER
// #define DIRECT

#define LED1 AT91C_PIO_PA30
#define LED2 AT91C_PIO_PA2
#define LED3 AT91C_PIO_PA28
#define LED4 AT91C_PIO_PA17
#define LED5 AT91C_PIO_PA18

#define BUTTON1 AT91C_PIO_PA1
#define BUTTON2 AT91C_PIO_PA0
#define BUTTON3 AT91C_PIO_PA31

#define ARRAY_LENGTH(X) (sizeof(X)/sizeof(X[0]))
typedef unsigned int uint_t;
typedef void (*blinker_mode_callback_t) (void);

struct blinker_mode_t {
	char *name;
	blinker_mode_callback_t function;
};

typedef struct blinker_mode_t blinker_mode_t;

uint_t all_leds[] = {
	LED1, LED2, LED3, LED4, LED5
};

void all_led_blinker_mode();
void odd_even_blinker_mode();

void odd_even_blinker_direct_mode();
void line_blinker_mode_forward();
void line_blinker_mode_backward();

blinker_mode_t blinker_modes [] = {
			{"All blinker", all_led_blinker_mode},
			{"Odd even blinker", odd_even_blinker_mode},
			{"Odd even blinker direct", odd_even_blinker_direct_mode},
			{"forward blinker", line_blinker_mode_forward},
			{"backward blinker", line_blinker_mode_backward},
};

uint_t current_blink_mode = 0;

unsigned char is_button_pressed(unsigned int button) {
	return !(AT91C_BASE_PIOA->PIO_PDSR & button);
}

//**************************************************************
// pioa_ir_init
// Initialisiert Interrupts des PIOA-Controllers
// Übergabeparameter:  PORT     Auswahl des(r) Pins
//                     PRIOR    Priorität   0 (niedrigste) .. 7 (höchste)
//                     SRCTYPE  Sensivität  0  Low-Level
//                                          1  Fallende Flanke
//                                          2  High-Level
//                                          3  Steigende Flanke
//                     ADR_ISR  Adresse der ISR-Routine
//**************************************************************
void pioa_ir_init (unsigned int PORT, unsigned int PRIOR, unsigned int SRCTYPE, void (*ADR_ISR) (void) ){
																// Konfiguration Advanced Interrupt Controllers (AIC):
	AT91C_BASE_AIC->AIC_IDCR = (1<<AT91C_ID_PIOA);					// PIOA-Interrupts sperren
	AT91C_BASE_AIC->AIC_SVR[AT91C_ID_PIOA]= (unsigned int) ADR_ISR;	// Adresse der ISR zuweisen
	AT91C_BASE_AIC->AIC_SMR[AT91C_ID_PIOA]= (SRCTYPE<<5)|PRIOR;		// Type und Priorität auswählen
	AT91C_BASE_AIC->AIC_ICCR = (1<<AT91C_ID_PIOA);					// ev. IR-Flag löschen
	AT91C_BASE_AIC->AIC_IECR = (1<<AT91C_ID_PIOA);					// PIOA-Interrupts freigeben

	AT91C_BASE_PIOA->PIO_IER = PORT;							// PIO-Pin(s) für Interrupts freigeben
}

//**************************************************************
// Interrupt-Service-Routine PIOA
//**************************************************************
__attribute__ ((used)) void pioa_isr (void){

	volatile unsigned int dummy;			// Lokale dummy-Variable als volatile deklariert

	if (is_button_pressed(BUTTON1)) {
		current_blink_mode = (current_blink_mode + 1) % ARRAY_LENGTH(blinker_modes);
	}

	dummy = AT91C_BASE_PIOA->PIO_ISR;		// Bestätigung Interrupt Request
}

void delay () {
	delay_ms(100);
}

void enable_led(uint_t led) {
	AT91C_BASE_PIOA->PIO_OER = led;							// Ausgang freigeben
	AT91C_BASE_PIOA->PIO_PPUDR = led;						// Pull-Up ausschalten
}

void led_on_pointer(uint_t led) {
	volatile unsigned int *SODR = (unsigned int *) 0xFFFFF430;	// Zeigerdefiniton 'Set Output Data Register'
	*SODR = led;												// LED an
}

void led_off_pointer(uint_t led) {
	volatile unsigned int *CODR = (unsigned int *) 0xFFFFF434;	// Zeigerdefiniton 'Clear Output Data Register'
	*CODR = led;													// LED an
}

void led_on_direct(uint_t led) {
	*((volatile unsigned int *) 0xFFFFF430) = led;		// LED an
}

void led_off_direct(uint_t led) {
	*((volatile unsigned int *) 0xFFFFF434) = led;		// LED aus
}

void led_on_base (uint_t led) {
	AT91C_BASE_PIOA->PIO_SODR = led;	// LED an
}

void led_off_base(uint_t led) {
	AT91C_BASE_PIOA->PIO_CODR = led; 	// LED aus
}

void led_on (uint_t led) {
#ifdef TYPICAL
	led_on_base(led);
#endif
#ifdef POINTER
	led_on_pointer(led);
#endif
#ifdef DIRECT
	led_on_direct(led);
#endif
}

void led_off (uint_t led) {
#ifdef TYPICAL
	led_off_base(led);
#endif
#ifdef POINTER
	led_off_pointer(led);
#endif
#ifdef DIRECT
	led_off_direct(led);
#endif
}

void blink_led(uint_t led, uint_t _delay) {
	led_on(led);
	delay_ms(_delay);
	led_off(led);
	delay_ms(_delay);
}

uint_t get_bitfield_for_leds(uint_t *leds, uint_t num_leds) {
	uint_t bitfield = 0;

	for (uint_t i = 0; i < num_leds; i++) {
		bitfield |= leds[i];
	}

	return bitfield;
}

void blink_leds(uint_t *leds, uint_t num_leds, uint_t _delay) {
	blink_led(get_bitfield_for_leds(leds, num_leds), _delay);
}

enum BlinkLineDirection {
	FORWARD, BACKWARD
};

void blink_all_in_line(uint_t *leds, uint_t num_leds, enum BlinkLineDirection direction) {
	if (direction == FORWARD) {
		for (uint_t i = 0; i < num_leds; i++) {
			blink_led(leds[i], 100);
		}
	}
	else if (direction == BACKWARD) {
		for (int i = num_leds-1; i >= 0; i--) {
			blink_led(leds[i], 100);
		}
	}
}

void all_led_blinker_mode () {
	blink_leds(all_leds, ARRAY_LENGTH(all_leds), 100);
}

void odd_even_blinker_mode () {
	uint_t odd_leds[] = {
			LED1, LED3, LED5
	};

	uint_t even_leds[] = {
			LED2, LED4
	};

	blink_leds(odd_leds, ARRAY_LENGTH(odd_leds), 1000);
	blink_leds(even_leds, ARRAY_LENGTH(even_leds), 1000);
}

void odd_even_blinker_direct_mode() {
	uint_t odd_leds[] = {
				LED1, LED3, LED5
	};

	uint_t even_leds[] = {
			LED2, LED4
	};

	AT91C_BASE_PIOA->PIO_OWER = get_bitfield_for_leds(all_leds, ARRAY_LENGTH(all_leds));

	AT91C_BASE_PIOA->PIO_ODSR = get_bitfield_for_leds(odd_leds, ARRAY_LENGTH(odd_leds));
	delay_ms(500);
	AT91C_BASE_PIOA->PIO_ODSR = get_bitfield_for_leds(even_leds, ARRAY_LENGTH(even_leds));
	delay_ms(500);

	AT91C_BASE_PIOA->PIO_CODR = get_bitfield_for_leds(all_leds, ARRAY_LENGTH(all_leds));
	AT91C_BASE_PIOA->PIO_OWDR = get_bitfield_for_leds(all_leds, ARRAY_LENGTH(all_leds));
}

void line_blinker_mode_forward () {
	blink_all_in_line(all_leds, ARRAY_LENGTH(all_leds), FORWARD);
}

void line_blinker_mode_backward () {
	blink_all_in_line(all_leds, ARRAY_LENGTH(all_leds), BACKWARD);
}

// main: Hauptprogramm
int main(void){
	for (uint_t i = 0; i < ARRAY_LENGTH(all_leds); i++) {
		enable_led(all_leds[i]);
	}

	display_init();

	// pio takt aktivieren
	AT91C_BASE_PMC->PMC_PCER = (1<<AT91C_ID_PIOA);

	pioa_ir_init(BUTTON1, 4, 3, pioa_isr);		// Port-IR initialisieren

	while (1) {
		display_clear();
		display_putstr(blinker_modes[current_blink_mode].name);
		blinker_modes[current_blink_mode].function();
		delay_ms(50);
	}
}
