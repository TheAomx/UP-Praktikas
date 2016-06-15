//*----------------------------------------------------------------------------
//*         ATMEL Microcontroller Software Support  -  ROUSSET  -
//*----------------------------------------------------------------------------
//* The software is delivered "AS IS" without warranty or condition of any
//* kind, either express, implied or statutory. This includes without
//* limitation any warranty or condition with respect to merchantability or
//* fitness for any particular purpose, or against the infringements of
//* intellectual property rights of others.
//*----------------------------------------------------------------------------
//* File Name           : Cstartup_SAM7.c
//* Object              : Low level initializations written in C for GCC Tools
//* Creation            : 12/Jun/04
//* 1.2   28/Feb/05 JPP : LIB change AT91C_WDTC_WDDIS & PLL
//* 1.3   21/Mar/05 JPP : Change PLL Wait time
//*----------------------------------------------------------------------------

// Include the AT91SAM7S64 file description
#include "AT91SAM7S64.h"

// The following functions must be write in ARM mode this function called directly
// by exception vector
extern void AT91F_Spurious_handler(void);
extern void AT91F_Default_IRQ_handler(void);
extern void AT91F_Default_FIQ_handler(void);

//*----------------------------------------------------------------------------
//* \fn    AT91F_LowLevelInit
//* \brief This function performs very low level HW initialization
//*        this function can be use a Stack, depending the compilation
//*        optimization mode
//*----------------------------------------------------------------------------
void AT91F_LowLevelInit( void)
{  int i;
  // Disable Watchdog
  AT91C_BASE_WDTC->WDTC_WDMR= AT91C_WDTC_WDDIS;
  // Set MCK at 24 MHz
  // Enabling the Main Oscillator:
  AT91C_BASE_PMC->PMC_MOR = ( (AT91C_CKGR_OSCOUNT) & (0x10 << 8)) | AT91C_CKGR_MOSCEN ;
  // Wait the startup time
  while(!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MOSCS));
  // CPU-Clock: MAIN_CLK / 4 (3 MHz)
  AT91C_BASE_PMC->PMC_MCKR = AT91C_PMC_CSS_MAIN_CLK | AT91C_PMC_PRES_CLK_4;
  // wait until MCKRDY in PMC_SR is set
  while(!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY));
  // setting PLL (96 MHz)
  AT91C_BASE_PMC->PMC_PLLR = ((AT91C_CKGR_DIV      & 8) |
                              (AT91C_CKGR_PLLCOUNT & (60 << 8)) |
                              (AT91C_CKGR_MUL      & (63 << 16)));
  // Wait the startup time
  while(!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_LOCK));
  // CPU-Clock: PLL / 4 (24 MHz)
  AT91C_BASE_PMC->PMC_MCKR = AT91C_PMC_CSS_PLL_CLK | AT91C_PMC_PRES_CLK_4;
  // wait until MCKRDY in PMC_SR is set
  while(!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY));
  //  Set Flash Waite state
  //  if MCK = 24MHz I have 36 Cycle for 1.5 usecond ( flied MC_FMR->FMCN)
  AT91C_BASE_MC->MC_FMR = ((AT91C_MC_FMCN)&(36<<16)) | AT91C_MC_FWS_0FWS ;
  // Set up the default interrupts handler vectors
  AT91C_BASE_AIC->AIC_SVR[0] = (int) AT91F_Default_FIQ_handler ;
  for (i=1;i<31; i++)
  {
	  AT91C_BASE_AIC->AIC_SVR[i] = (int) AT91F_Default_IRQ_handler ;
  }
  AT91C_BASE_AIC->AIC_SPU  = (int) AT91F_Spurious_handler ;
}
