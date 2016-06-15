//********************************************************************
// ARM_T5A3a									Version 02.04.2012 hpw
//********************************************************************
#include "include/AT91SAM7S64.h"
#include "include/pwm.h"
#include "include/interrupt.h"

#define LED1 AT91C_PIO_PA30
#define LED2 AT91C_PIO_PA2
#define LED3 AT91C_PIO_PA28
#define LED4 AT91C_PIO_PA17
#define LED5 AT91C_PIO_PA18

#define ARRAY_LENGTH(X) (sizeof(X)/sizeof(X[0]))
#define FREQ 18750
#define PWM_CHANNEL 2

typedef unsigned int uint_t;

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


__attribute__ ((used)) void timer0_isr (void){
	volatile unsigned int dummy;

	static uint_t counter = 0;
#if 0
	if (counter < (FREQ/3)) {
		led_on(LED2);
	}
	else {
		led_off(LED2);
		if (counter > FREQ) {
			counter = 0;
		}
	}
#endif
	invert_pio(LED2);

	counter++;

	AT91C_BASE_TC0->TC_CCR=AT91C_TC_SWTRG;	// Zurücksetzen des Zählers
	dummy = AT91C_BASE_TC0->TC_SR;			// Bestätigung Interrupt Request
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

	led_on(LED5);
#if 0
	timer_ir_init(0, 5, 3, timer0_isr);
	AT91C_BASE_TC0->TC_RC = FREQ;			// max. Zählerwert für TC0
	AT91C_BASE_TC0->TC_CCR=AT91C_TC_SWTRG;	// Zurücksetzen des Zählers Timer0
#endif
	//led_off(get_bitfield_for_leds(all_leds,ARRAY_LENGTH(all_leds)));
	signed char ret = pwm_init(PWM_CHANNEL, LED2, 1.0f);
	if (ret !=0 ){
		if(ret==-1){
			led_on(LED1);
		}else if(ret==-2){
			led_on(LED1|LED2);
		}else if(ret==-3){
			led_on(LED1|LED2|LED3);
		}
	}

	ret = pwm_set_duty(PWM_CHANNEL, 1);
	if (ret == -1) {
		led_on(get_bitfield_for_leds(all_leds, ARRAY_LENGTH(all_leds)));
	}

	while (1);
}
