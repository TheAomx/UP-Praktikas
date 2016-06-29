/*
 * main.c
 *
 *  Created on: 27.04.2016
 *      Author: Robin
 */

#include	"include/AT91SAM7S64.h"
#include 	"include/display.h"
#include 	"include/cToolMod.h"


int main(){
	unsigned char ucJ = 0;

	display_init();
	do {
		display_set_cursor(0, 0);
		display_printf("%x", ucJ);

		display_set_cursor(0, 1);
		display_putchar(ucJ);

		delay1us(50000);
	}while(++ucJ != 0);

	while (1);								// Endlosschleife nach eigentlichem
	//  Programm: niemals vergessen!
}
