//********************************************************************
// interrupt.h									Version 02.04.2012 hpw
//********************************************************************
#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include "AT91SAM7S64.h"
#include "interrupt.h"

void pioa_ir_init (unsigned int PORT, unsigned int PRIOR, unsigned int SRCTYPE, void (*ADR_ISR) (void) );
void timer_ir_init (unsigned int TC_NR, unsigned int PRIOR, unsigned int TCCLKS, void (*ADR_ISR) (void) );

#endif /*INTERRUPT_H_*/
