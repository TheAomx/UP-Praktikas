//********************************************************************
// interrupt.c									Version 02.04.2012 hpw
//********************************************************************
#include "include/AT91SAM7S64.h"
#include "include/interrupt.h"

//**************************************************************
// pioa_ir_init
// Initialisiert Interrupts des PIOA-Controllers
// Übergabeparameter:  PORT     Auswahl des(r) Pins
//                     PRIOR    Priorität   0 (niedrigste) .. 7 (höchste)
//                     SRCTYPE  Sensivität  0  Low-Level
//                                          1  Fallende Flanke
//                                          2  High-Level
//                                          3  Steigende Flanke
//                     ADR_ISR  Adresse der ISR-Routine
//**************************************************************
void pioa_ir_init (unsigned int PORT, unsigned int PRIOR, unsigned int SRCTYPE, void (*ADR_ISR) (void) ){
																// Konfiguration Advanced Interrupt Controllers (AIC):
	AT91C_BASE_AIC->AIC_IDCR = (1<<AT91C_ID_PIOA);					// PIOA-Interrupts sperren
	AT91C_BASE_AIC->AIC_SVR[AT91C_ID_PIOA]= (unsigned int) ADR_ISR;	// Adresse der ISR zuweisen
	AT91C_BASE_AIC->AIC_SMR[AT91C_ID_PIOA]= (SRCTYPE<<5)|PRIOR;		// Type und Priorität auswählen
	AT91C_BASE_AIC->AIC_ICCR = (1<<AT91C_ID_PIOA);					// ev. IR-Flag löschen
	AT91C_BASE_AIC->AIC_IECR = (1<<AT91C_ID_PIOA);					// PIOA-Interrupts freigeben

	AT91C_BASE_PIOA->PIO_IER = PORT;							// PIO-Pin(s) für Interrupts freigeben
}

//**************************************************************
// timer_ir_init
// Initialisiert Interrupts des Timers 0..2
// Übergabeparameter:  TC_NR    Nr. des Timers  0..2
//                     PRIOR    Priorität       0 (niedrigste) .. 7 (höchste)
//                     TCCLKS   Modus           0  TimerClock1 = MCK/2    =>  12,0    MHz
//                                              1  TimerClock2 = MCK/8    =>   3,0    MHz
//                                              2  TimerClock3 = MCK/32   => 750,0    kHz
//                                              3  TimerClock4 = MCK/128  => 187,5    kHz
//                                              4  TimerClock5 = MCK/1024 =>  23,4375 kHz
//                     ADR_ISR  Adresse der ISR-Routine
//**************************************************************
void timer_ir_init (unsigned int TC_NR, unsigned int PRIOR, unsigned int TCCLKS, void (*ADR_ISR) (void) ){
	AT91S_TC *TC_POINTER;										// Pointer TimerCounter Channel
	unsigned int IRQ_ID;										// Timer-IRQ-ID
	volatile unsigned int dummy;								// Dummy-Variable

	switch(TC_NR){												// Timer auswählen:
		case 0:
			TC_POINTER = AT91C_BASE_TC0;							// Pointer für Timer0 zuweisen
			IRQ_ID = AT91C_ID_TC0;									// IRQ_ID des Timers0 zuweisen
			break;
		case 1:
			TC_POINTER = AT91C_BASE_TC1;							// Pointer für Timer0 zuweisen
			IRQ_ID = AT91C_ID_TC1;									// IRQ_ID des Timers0 zuweisen
			break;
		case 2:
			TC_POINTER = AT91C_BASE_TC2;							// Pointer für Timer0 zuweisen
			IRQ_ID = AT91C_ID_TC2;									// IRQ_ID des Timers0 zuweisen
			break;
		default:
			TC_POINTER = AT91C_BASE_TC0;							// Pointer für Timer0 zuweisen
			IRQ_ID = AT91C_ID_TC0;									// IRQ_ID des Timers0 zuweisen
			break;}

	AT91C_BASE_PMC->PMC_PCER = (1<<IRQ_ID);						// Clock für den Timer freigeben

																// Konfiguration Timer Counter (TC):
	TC_POINTER->TC_CCR = AT91C_TC_CLKDIS;							// Clock des Timers sperren
	TC_POINTER->TC_IDR = 0xFF;										// Alle Timer-Interrupts sperren
	dummy = TC_POINTER->TC_SR;										// Status Bit löschen
	TC_POINTER->TC_CMR = TCCLKS;									// Timer-Modus einstellen
	TC_POINTER->TC_CCR = AT91C_TC_CLKEN;							// Timer-Clock freigeben
    TC_POINTER->TC_IER  = AT91C_TC_CPCS;							// Timer-Interrupts für 'RC Compare' freischalten

																// Konfiguration Advanced Interrupt Controllers (AIC):
    AT91C_BASE_AIC->AIC_IDCR = (1<<IRQ_ID);							// TimerX-Interrupts sperren
    AT91C_BASE_AIC->AIC_SVR[IRQ_ID]=(unsigned int) ADR_ISR;			// Adresse der ISR zuweisen
    AT91C_BASE_AIC->AIC_SMR[IRQ_ID]=AT91C_AIC_SRCTYPE_EXT_HIGH_LEVEL|PRIOR;	// Type und Priorität auswählen
    AT91C_BASE_AIC->AIC_ICCR = (1<<IRQ_ID);							// ev. IR-Flag löschen
	AT91C_BASE_AIC->AIC_IECR = (1<<IRQ_ID);							// TimerX-Interrupts freigeben
}
