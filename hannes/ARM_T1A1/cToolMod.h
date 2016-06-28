/*
 * cToolMod.h
 *
 *  Created on: 27.04.2016
 *      Author: hannes
 */

#ifndef CTOOLMOD_H_
#define CTOOLMOD_H_

void delay5ms(unsigned int uiK);
void delay1ms(unsigned int uiK);
void blink(unsigned int frequence, unsigned int blinck_time, unsigned to_blinck,
		unsigned to_blink_gegen);
void running_LED(unsigned int leds[5]);
unsigned char GetTaste();

#endif /* CTOOLMOD_H_ */
