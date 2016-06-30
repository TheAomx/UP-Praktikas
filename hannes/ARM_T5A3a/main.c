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

#define LED_ALL (LED1 | LED2 | LED3 | LED4 | LED5)

//**************************************************************
// main()
//**************************************************************
int main() {
//	unsigned int leds[] = { LED1, LED2, LED3, LED4, LED5 };
	AT91C_BASE_PIOA->PIO_OER = LED_ALL;	// Ausgang freigeben
	AT91C_BASE_PIOA->PIO_PPUDR = LED_ALL;	// Pull-Up ausschalten
	AT91C_BASE_PIOA->PIO_CODR = LED_ALL;	// Alle LED aus// Alle LED aus

	pwm_init(PWM_CHANNEL, LED2, 10.0f);

	pwm_set_duty(PWM_CHANNEL, 50);

	while (1)
		;
}
