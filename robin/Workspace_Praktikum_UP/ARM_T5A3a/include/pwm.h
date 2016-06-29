//********************************************************************
// pwm.h										Version 02.04.2012 hpw
//********************************************************************
#ifndef pwm_h_
#define pwm_h_

signed char pwm_init (unsigned char PWM_NR, unsigned int PORT, float FRQ);
signed char pwm_set_duty (unsigned char PWM_NR,unsigned int DUTY);

#endif
