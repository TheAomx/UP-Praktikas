/*
 * cToolMod.c
 *
 *  Created on: 18.04.2016
 *      Author: Aomx
 */

#include "cToolMod.h"

void delay_ms(unsigned int milliseconds){
	volatile unsigned int i;

	while (milliseconds--)
		for (i=0; i<1638; i++);
}

void delay_s(unsigned int seconds) {
	delay_ms(1000 * seconds);
}
