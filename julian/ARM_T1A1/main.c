//********************************************************************
// ARM_T1A1										Version 02.04.2012 hpw
//********************************************************************
#include	"include/AT91SAM7S64.h"			// Definition von ARM7 typischen Registern etc.

#define LED1 AT91C_PIO_PA30
#define LED2 AT91C_PIO_PA2
#define LED3 AT91C_PIO_PA28
#define LED4 AT91C_PIO_PA17
#define LED5 AT91C_PIO_PA18

#define TYPICAL

#define ARRAY_LENGTH(X) (sizeof(X)/sizeof(X[0]))
typedef unsigned int uint_t;

uint_t all_leds[] = {
	LED1, LED2, LED3, LED4, LED5
};


//**************************************************************
// delay5ms(uiK) verzögert um ca. uiK * 5ms
//**************************************************************
void delay5ms(unsigned int uiK){
	volatile unsigned int uiI;

	while (uiK--)							// folgende for-Schleife wird uiK-mal aufgerufen
		for (uiI=0; uiI<8192; uiI++);		//  for-Schleife wird 8192-mal durchlaufen. Dies
											//  verursacht eine Zeitverzögerung von ca. 5ms.
}

void delay_ms(unsigned int milliseconds){
	volatile unsigned int i;

	while (milliseconds--)
		for (i=0; i<1638; i++);
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


//**************************************************************
// main läßt LED am Port P30 für ca. 1 Minute mit 1 Hz blinken
//**************************************************************
int main(){
	unsigned char ucB=60;					// lokale Variable ucB

	for (uint_t i = 0; i < ARRAY_LENGTH(all_leds); i++) {
		enable_led(all_leds[i]);
	}

#if 0
	while (ucB--){
		uint_t leds [] =  {LED1, LED3, LED5};
		blink_leds(leds, ARRAY_LENGTH(leds), 500);
	}
#endif

#if 1
	uint_t odd_leds[] = {
				LED1, LED3, LED5
	};

	uint_t even_leds[] = {
			LED2, LED4
	};

	AT91C_BASE_PIOA->PIO_OWER = get_bitfield_for_leds(all_leds, ARRAY_LENGTH(all_leds));

	while (ucB--){
		AT91C_BASE_PIOA->PIO_ODSR = get_bitfield_for_leds(odd_leds, ARRAY_LENGTH(odd_leds));
		delay_ms(500);
		AT91C_BASE_PIOA->PIO_ODSR = get_bitfield_for_leds(even_leds, ARRAY_LENGTH(even_leds));
		delay_ms(500);
	}

	AT91C_BASE_PIOA->PIO_CODR = get_bitfield_for_leds(all_leds, ARRAY_LENGTH(all_leds));
	AT91C_BASE_PIOA->PIO_OWDR = get_bitfield_for_leds(all_leds, ARRAY_LENGTH(all_leds));
#endif

	while (1);								// Endlosschleife nach eigentlichem
											//  Programm: niemals vergessen!
}
