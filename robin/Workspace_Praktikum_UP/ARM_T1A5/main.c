/*
 * main.c
 *
 *  Created on: 27.04.2016
 *      Author: Robin
 */

#include	"include/AT91SAM7S64.h"
#include 	"include/cToolMod.h"
#include 	"include/display.h"


int main(){
	AT91C_BASE_PMC->PMC_PCER = (1<<AT91C_ID_PIOA);
	display_init();
	while(1) {
		unsigned int a = getTaste();
		display_set_cursor(0, 0);
		display_printf("%d", a);
	}

	while(1){};
}
