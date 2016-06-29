//********************************************************************
// ARM_T5A3a									Version 02.04.2012 hpw
//********************************************************************
#include "include/AT91SAM7S64.h"
#include "display.h"

#define LED1 AT91C_PIO_PA30
#define LED2 AT91C_PIO_PA2
#define LED3 AT91C_PIO_PA28
#define LED4 AT91C_PIO_PA17
#define LED5 AT91C_PIO_PA18

#define ARRAY_LENGTH(X) (sizeof(X)/sizeof(X[0]))
#define FREQ 18750
#define PWM_CHANNEL 2

typedef unsigned int uint_t;

void delay_ms(unsigned int milliseconds){
	volatile unsigned int i;

	while (milliseconds--)
		for (i=0; i<1638; i++);
}

uint_t all_leds[] = {
	LED1, LED2, LED3, LED4, LED5
};

void enable_led(uint_t led) {
	AT91C_BASE_PIOA->PIO_OER =  led;							// Ausgang freigeben
	AT91C_BASE_PIOA->PIO_OWER = led;	// Freigabe des schreibenden/lesenden Zugriffs
	AT91C_BASE_PIOA->PIO_PPUDR = led;						// Pull-Up ausschalten
}

void led_on(uint_t led) {
	AT91C_BASE_PIOA->PIO_SODR = led;
}

void led_off(uint_t led) {
	AT91C_BASE_PIOA->PIO_CODR = led;
}

static void invert_pio (uint_t pio_address) {
	uint_t current_state_pio = AT91C_BASE_PIOA->PIO_ODSR & pio_address;

	if (current_state_pio) {
		AT91C_BASE_PIOA->PIO_ODSR =  AT91C_BASE_PIOA->PIO_ODSR & (~pio_address);
	}
	else {
		AT91C_BASE_PIOA->PIO_ODSR =  AT91C_BASE_PIOA->PIO_ODSR | pio_address;
	}

}
uint_t get_bitfield_for_leds(uint_t *leds, uint_t num_leds) {
	uint_t bitfield = 0;

	for (uint_t i = 0; i < num_leds; i++) {
		bitfield |= leds[i];
	}

	return bitfield;
}

//**************************************************************
// main()
//**************************************************************
int main(){
	for (uint_t i = 0; i < ARRAY_LENGTH(all_leds); i++) {
		enable_led(all_leds[i]);
		led_off(all_leds[i]);
	}

	display_init();


	AT91C_BASE_ADC->ADC_CHER |= 1 << 4;
	AT91C_BASE_ADC->ADC_MR = AT91C_ADC_PRESCAL | AT91C_ADC_STARTUP | AT91C_ADC_SHTIM;

	while (1) {
		AT91C_BASE_ADC->ADC_CR |= 1 << 1;
		if ((AT91C_BASE_ADC->ADC_SR & (1 << 16))>0) {
			// daten ready
			uint_t data = AT91C_BASE_ADC->ADC_CDR4;
			display_printf("%d", data);
		}
#if 1
		else {
			display_putstr("daten not ready");
		}
#endif
		delay_ms(100);
		display_clear();
	}


	while (1);
}
