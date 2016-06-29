//********************************************************************
// ARM_T5A3a									Version 02.04.2012 hpw
//********************************************************************
#include "include/AT91SAM7S64.h"
#include "include/pwm.h"

#define		LED2	AT91C_PIO_PA2			// Parallel Input Output Control Pin 2


#define 	PWM_CHANNEL 2
//**************************************************************
// main()
//**************************************************************
int main(){

	pwm_init(PWM_CHANNEL, LED2, 2.0f);
	pwm_set_duty(PWM_CHANNEL, 25);

	while(1);

}
