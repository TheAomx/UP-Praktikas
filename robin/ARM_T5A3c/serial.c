/******************************************************************************/
/*  This file has been a part of the uVision/ARM development tools            */
/*  Copyright KEIL ELEKTRONIK GmbH 2002-2004                                  */
/******************************************************************************/
/*                                                                            */
/*  SERIAL.C:  Low Level Serial Routines                                      */
/*                                                                            */
/******************************************************************************/

//	Martin Thomas :
//	- slightly modified for the WinARM example
//	- changed function names to avoid conflict with "stdio"
//	  (so far no syscalls for the newlib in this example)
//	- Keil seems to "reject" the AT91lib*.h. Don't know
//	  why since the layer is "thin". Maybe will change this to
//	"lib Style" in later version...

//	Hans-Peter Wiesmath :
//	- modified for ARM7-Board used in our Lab

#include "include/AT91SAM7S64.h"				/* AT91SAMT7S64 definitions */
												// hpw: modified the path

#define	MCK	24000000							// hpw: clockrate used on ARM7-Board
#define BR	115200								/* Baud Rate */
#define BRD	(MCK/16/BR)							/* Baud Rate Divisor */

AT91S_USART * pUSART = AT91C_BASE_US0;			/* Global Pointer to USART0 */

void usart0_init (void){						/* Initialize Serial Interface */
	AT91C_BASE_PMC->PMC_PCER=(1<<AT91C_ID_US0);	// hpw: enable USART0-clock

	*AT91C_PIOA_PDR =	AT91C_PA5_RXD0 |		/* Enable RxD0 Pin */
						AT91C_PA6_TXD0;			/* Enalbe TxD0 Pin */

	pUSART->US_CR = AT91C_US_RSTRX |			/* Reset Receiver      */
					AT91C_US_RSTTX |			/* Reset Transmitter   */
					AT91C_US_RXDIS |			/* Receiver Disable    */
					AT91C_US_TXDIS;				/* Transmitter Disable */

	pUSART->US_MR = AT91C_US_USMODE_NORMAL |	/* Normal Mode */
					AT91C_US_CLKS_CLOCK    |	/* Clock = MCK */
					AT91C_US_CHRL_8_BITS   |	/* 8-bit Data  */
					AT91C_US_PAR_NONE      |	/* No Parity   */
					AT91C_US_NBSTOP_1_BIT;		/* 1 Stop Bit  */

	pUSART->US_BRGR = BRD;						/* Baud Rate Divisor */

	pUSART->US_CR = AT91C_US_RXEN  |			/* Receiver Enable     */
					AT91C_US_TXEN;				/* Transmitter Enable  */
}

int usart0_putc(int ch){
	while (!(pUSART->US_CSR & AT91C_US_TXRDY));	/* Wait for Empty Tx Buffer */
	return (pUSART->US_THR = ch);				/* Transmit Character */
}

int usart0_putchar (int ch){					/* Write Character to Serial Port */
	if (ch == '\n') usart0_putc( '\r' );		/* Check for LF, Output CR */
	return usart0_putc( ch );					/* Transmit Character */
}

int usart0_puts ( char* s ){
	while ( *s ) usart0_putchar( *s++ );
	return 0;
}

int usart0_kbhit( void ){						/* returns true if character in receive buffer */
	if ( pUSART->US_CSR & AT91C_US_RXRDY) return 1;
	else return 0;
}

int usart0_getc ( void ){						/* Read Character from Serial Port */
	while (!(pUSART->US_CSR & AT91C_US_RXRDY));	/* Wait for Full Rx Buffer */
	return (pUSART->US_RHR);					/* Read Character */
}
