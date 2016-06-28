#include	"include/AT91SAM7S64.h"


#define		BUTTON1	AT91C_PIO_PA1			// Parallel Input Output Control Pin 30
#define		BUTTON2	AT91C_PIO_PA0 			// Parallel Input Output Control Pin 2
#define		BUTTON3	AT91C_PIO_PA31

void delay5ms(unsigned int uiK);

void delay1ms(unsigned int uiK);

unsigned int getTaste();
