//********************************************************************
// ARM_T1A1										Version 02.04.2012 hpw
//********************************************************************
#include "include/AT91SAM7S64.h"			// Definition von ARM7 typischen Registern etc.
#include "display.h"

#define LED1 AT91C_PIO_PA30									// LED1 angeschlossen an Port-Pin 30
#define LED2 AT91C_PIO_PA2
#define LED3 AT91C_PIO_PA28
#define LED4 AT91C_PIO_PA17
#define LED5 AT91C_PIO_PA18

#define PUSH1 AT91C_PIO_PA1
#define PUSH2 AT91C_PIO_PA0
#define PUSH3 AT91C_PIO_PA31

#define LED_ALL (LED1 | LED2 | LED3 | LED4 | LED5)

//**************************************************************
// main läßt LED am Port P30 für ca. 1 Minute mit 1 Hz blinken
//**************************************************************

enum DIRECTION {
	LEFT, RIGHT
};

void delay1ms(unsigned int uiK) {
	volatile unsigned int uiI;
	while (uiK--) // folgende for-Schleife wird uiK-mal aufgerufen
		for (uiI = 0; uiI < 1638; uiI++)
			;
}

//#define a1_4_1
//#define a1_4_2
#define a1_5

#ifdef a1_4_1
void print_ucJ(unsigned int speed) {
	enum DIRECTION {
		FALSE = 0, TRUE
	};
	char hex[] = "0123456789ABCDEF";
	char toprint[] = "0x00";

	unsigned short i = 0, j = 0, done = FALSE;
	unsigned char ucJ = 0;

	display_init();
	while (!done) {
		(ucJ == 0xFF) ? done = TRUE : done;
		((ucJ + 1) % 16) ? i++ : (j++, i = 0);

		toprint[2] = hex[j];
		toprint[3] = hex[i];

		display_set_cursor(0, 0);
		display_putstr(toprint);
		delay1ms(speed);
		display_set_cursor(0, 1);
		display_putchar(ucJ);
		ucJ++;
	}
	display_set_cursor(0, 1);
	display_putstr("Done...");
}
#endif

#ifdef a1_4_2
void print_ucJ_using_printf(unsigned int speed) {
	enum DIRECTION {
		FALSE = 0, TRUE
	};
	char hex[] = "0123456789abcdef";
	char toprint[] = "0x00";

	unsigned short i = 0, j = 0, done = FALSE;

	unsigned char ucJ = 0;

	display_init();
	while (!done) {
		(ucJ == 0xFF) ? done = TRUE : done;
		((ucJ + 1) % 16) ? i++ : (j++, i = 0);

		toprint[2] = hex[j];
		toprint[3] = hex[i];
		display_set_cursor(0, 0);
		display_printf("0x%x", ucJ);

		//display_putstr(toprint);
		delay1ms(speed);
		display_set_cursor(0, 1);
		display_putchar(ucJ);
		ucJ++;
	}
	display_set_cursor(0, 1);
	display_putstr("Done...");
}
#endif

// main: Hauptprogramm
int main(void) {
#ifdef a1_4_1
	/*
	 * Size after:
	 * main.elf  :
	 * section            size   addr
	 * .text              3956      0
	 * .rodata              56   3956
	 * .comment             72      0
	 * .debug_aranges      160      0
	 * .debug_pubnames     267      0
	 * .debug_info        2870      0
	 * .debug_abbrev       798      0
	 * .debug_line         810      0
	 * .debug_frame        436      0
	 * .debug_str         1271      0
	 * .debug_loc          724      0
	 * .ARM.attributes      16      0
	 * total             11436 delta=36 bytes!
	 * */
	print_ucJ(50);
#endif

#ifdef a1_4_2
	/*
	 * Size after:
	 * main.elf  :
	 * section            size   addr
	 * .text              3964      0
	 * .rodata              64   3964
	 * .comment             72      0
	 * .debug_aranges      160      0
	 * .debug_pubnames     280      0
	 * .debug_info        2870      0
	 * .debug_abbrev       798      0
	 * .debug_line         810      0
	 * .debug_frame        436      0
	 * .debug_str         1284      0
	 * .debug_loc          724      0
	 * .ARM.attributes      16      0
	 * Total             11478
	 */
	print_ucJ_using_printf(50);
#endif

#ifdef a1_5
	AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_PIOA);
	unsigned int button_mask = PUSH1 | PUSH2 | PUSH3;
	display_init();
	display_set_cursor(0, 1);
	display_putstr("Test");
	AT91C_BASE_PIOA->PIO_PPUER = button_mask;
	while (1) {
		delay1ms(100);
		display_set_cursor(0, 0);
		display_printf("%x", GetTaste());

	}
#endif

}
