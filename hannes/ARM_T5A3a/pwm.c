//********************************************************************
// pwm.c										Version 02.04.2012 hpw
//********************************************************************
#include "include/AT91SAM7S64.h"
#include "include/pwm.h"

#define	MCK		24000000							// Master Clock

//#define CPRE	(0 << 0)							// Channel Pre-scaler
														//	 0:MCK		 1:MCK/2	 2:MCK/4	 3:MCK/8
														//	 4:MCK/16	 5:MCK/32	 6:MCK/64	 7:MCK/128
														//	 8:MCK/256	 9:MCK/512	10:MCK/1024
														//	11:CLKA		12:CLKB
#define	CPOL	(0 << 9)							// Channel Polarity
														// 0: The output waveform starts at a low level
														// 1: The output waveform starts at a high level
#define	CALG	(0 << 8)							// Channel Alignment
														// 0: The period is left aligned
														// 1: The period is center aligned
#define	CPD		(0 << 10)							// Channel Update Period
														// 0: Writing to the PWM_CUPDx will modify the duty cycle at the next period start event.
														// 1: Writing to the PWM_CUPDx will modify the period at the next period start event

//**************************************************************
// pwm_init
// Initialisiert Puls-Weiten-Modulations-Controller-Modul
// Übergabeparameter:  PWM_NR	Channel-Nr. des PWMC 0..3
//                     PORT		Zugehöriger Port-Pin
//                     FRQ		Frequenz 0.36Hz .. 120kHz
// Rückgabeparameter:  PWM_NR	Nr. des initialisierten PWMC
//								im Fehlerfall	PWM_NR:	-1
//												PORT:	-2
//												FRQ:	-3
//**************************************************************
signed char pwm_init (unsigned char PWM_NR, unsigned int PORT, float FRQ){
	AT91S_PWMC_CH *PWM_POINTER;						// Pointer PWM Channel
	unsigned int CPRE;								// PWM Prescaler
	unsigned int CPRD;								// PWM Periode

	switch(PWM_NR){									// PWM-Channel auswählen:
		case 0:
			PWM_POINTER = AT91C_BASE_PWMC_CH0;			// Pointer für PWMC0 zuweisen
			switch(PORT){
				case AT91C_PA0_PWM0:
					AT91C_BASE_PIOA->PIO_ASR=PORT;			// PWM-Modul als Pheripherie A auswählen
					break;
				case AT91C_PA11_PWM0:
					AT91C_BASE_PIOA->PIO_BSR=PORT;			// PWM-Modul als Pheripherie B auswählen
					break;
				case AT91C_PA23_PWM0:
					AT91C_BASE_PIOA->PIO_BSR=PORT;			// PWM-Modul als Pheripherie B auswählen
					break;
				default: return -2;}						// Mit Fehler zurück
			break;
		case 1:
			PWM_POINTER = AT91C_BASE_PWMC_CH1;			// Pointer für PWMC1 zuweisen
			switch(PORT){
				case AT91C_PA1_PWM1:
					AT91C_BASE_PIOA->PIO_ASR=PORT;			// PWM-Modul als Pheripherie A auswählen
					break;
				case AT91C_PA12_PWM1:
					AT91C_BASE_PIOA->PIO_BSR=PORT;			// PWM-Modul als Pheripherie B auswählen
					break;
				case AT91C_PA24_PWM1:
					AT91C_BASE_PIOA->PIO_BSR=PORT;			// PWM-Modul als Pheripherie B auswählen
					break;
				default: return -2;}						// Mit Fehler zurück
			break;
		case 2:
			PWM_POINTER = AT91C_BASE_PWMC_CH2;			// Pointer für PWMC2 zuweisen
			switch(PORT){
				case AT91C_PA2_PWM2:
					AT91C_BASE_PIOA->PIO_ASR=PORT;			// PWM-Modul als Pheripherie A auswählen
					break;
				case AT91C_PA13_PWM2:
					AT91C_BASE_PIOA->PIO_BSR=PORT;			// PWM-Modul als Pheripherie B auswählen
					break;
				case AT91C_PA25_PWM2:
					AT91C_BASE_PIOA->PIO_BSR=PORT;			// PWM-Modul als Pheripherie B auswählen
					break;
				default: return -2;}						// Mit Fehler zurück
			break;
		case 3:
			PWM_POINTER = AT91C_BASE_PWMC_CH3;			// Pointer für PWMC3 zuweisen
			switch(PORT){
				case AT91C_PA7_PWM3:
					AT91C_BASE_PIOA->PIO_BSR=PORT;			// PWM-Modul als Pheripherie B auswählen
					break;
				case AT91C_PA14_PWM3:
					AT91C_BASE_PIOA->PIO_BSR=PORT;			// PWM-Modul als Pheripherie B auswählen
					break;
				default: return -2;}						// Mit Fehler zurück
			break;
		default: return -1;}							// Mit Fehler zurück
	AT91C_BASE_PIOA->PIO_PDR=PORT;					// PIO-Modus auf Pheriperie
	AT91C_BASE_PIOA->PIO_PPUDR=PORT;				// Pull-up-Widerstände aus
	if (FRQ>120000)  return -3;						// Frequenz zu hoch -> mit Fehler zurück
	else{											// Periode und Prescaler berechnen
		CPRD = MCK / FRQ;								// Periode = Masterclock / Frequenz
		CPRE = 0;										// Prescaler auf 0
		while (CPRD & 0xFFFF0000){						// Überprüfung ob Periode > 16Bit
			CPRD = CPRD >> 1;							// Periode / 2
			CPRE++;										// Prescaler + 1
			if (CPRE>0x0A) return -3;}}					// Frequenz zu niedrig -> mit Fehler zurück
	AT91C_BASE_PMC->PMC_PCER = (1<<AT91C_ID_PWMC);	// Clock für PWMC freigeben
	PWM_POINTER->PWMC_CMR = (CPD|CPOL|CALG|CPRE);	// Modus einstellen
	PWM_POINTER->PWMC_CPRDR = CPRD;					// Frequenz einstellen
	return (signed char) PWM_NR;					// PWMC-Nr. zurück
}

//**************************************************************
// pwm_set_duty
// Stellt Duty des PWM-Channels ein
// Übergabeparameter:  PWM_NR	Channel-Nr. des PWMC 0..3
//                     DUTY		'high' in Prozent
// Rückgabeparameter:  PWM_NR	Nr. des  PWMC
//								im Fehlerfall:	-1
//**************************************************************
signed char pwm_set_duty (unsigned char PWM_NR,unsigned int DUTY){
	AT91S_PWMC_CH *PWM_POINTER;						// Pointer PWM Channel
	unsigned int CH_ID;								// Channel-ID
	unsigned int CPRD;								// PWM Periode

	switch(PWM_NR){									// PWM-Channel auswählen:
		case 0:
			PWM_POINTER = AT91C_BASE_PWMC_CH0;			// Pointer für PWMC0 zuweisen
			CH_ID = AT91C_PWMC_CHID0; break;			// Channel-ID-zuweisen
		case 1:
			PWM_POINTER = AT91C_BASE_PWMC_CH1;			// Pointer für PWMC1 zuweisen
			CH_ID = AT91C_PWMC_CHID1; break;			// Channel-ID-zuweisen
		case 2:
			PWM_POINTER = AT91C_BASE_PWMC_CH2;			// Pointer für PWMC2 zuweisen
			CH_ID = AT91C_PWMC_CHID2; break;			// Channel-ID-zuweisen
		case 3:
			PWM_POINTER = AT91C_BASE_PWMC_CH3;			// Pointer für PWMC3 zuweisen
			CH_ID = AT91C_PWMC_CHID3; break;			// Channel-ID-zuweisen
		default: return -1;}							// Mit Fehler zurück
	CPRD = PWM_POINTER->PWMC_CPRDR & 0x0000FFFF;	// aktuelle Periode auslesen
	if (DUTY){										// Duty > 0 ?
		DUTY = DUTY%100;								// Duty begrenzen auf unter 100
		PWM_POINTER->PWMC_CUPDR = CPRD*DUTY/100;		// Neue Duty einstellen
		AT91C_BASE_PWMC->PWMC_ENA = CH_ID;}				// PWM enabeln
	else AT91C_BASE_PWMC->PWMC_DIS = CH_ID;				// PWM2 disabeln
	return PWM_NR;									// PWMC-Nr. zurück
}
