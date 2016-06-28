//********************************************************************
// ARM_T5A2										Version 02.04.2012 hpw
//********************************************************************
#include "include/interrupt.h"				// IR-Routinen
#include "include/AT91SAM7S64.h"			// Definition von ARM7 typischen Registern etc.

#define SPEAKER_PORT AT91C_PIO_PA18

unsigned int uiDauer, uiFrequenz; // Globale Variable f�r Tondauer und Frequenz

//**************************************************************
// Interrupt-Service-Routine Timer0 (Tondauer)
//**************************************************************
__attribute__ ((used)) void timer0_isr(void) {
	volatile unsigned int dummy;			// Lokale dummy-Variable
	if (uiDauer > 0)
		uiDauer--;
	AT91C_BASE_TC0->TC_CCR = AT91C_TC_SWTRG;	// Zur�cksetzen des Z�hlers
	dummy = AT91C_BASE_TC0->TC_SR;			// Best�tigung Interrupt Request
}

//**************************************************************
// Interrupt-Service-Routine Timer1 (Tonh�he)
//**************************************************************
__attribute__ ((used)) void timer1_isr(void) {
	volatile unsigned int dummy;				// Lokale dummy-Variable
	if (uiFrequenz != 0)
		AT91C_BASE_PIOA->PIO_ODSR ^= SPEAKER_PORT;
	AT91C_BASE_TC1->TC_CCR = AT91C_TC_SWTRG;	// Zur�cksetzen des Z�hlers
	dummy = AT91C_BASE_TC1->TC_SR;				// Best�tigung Interrupt Request
}

//**************************************************************
//  main()
//**************************************************************
int main() {

//	// "Rondo alla Turca", Mozart
#define	BPM	120
#include "include/melodie.h"			// Deklaration der Tonfrequenz und Tondauer
	const unsigned int lied_f[] = { f_h1, f_a1, f_gis1, f_a1, f_c2, f_pause,
	f_d2, f_c2, f_h1, f_c2, f_e2, f_pause, f_f2, f_e2, f_dis2, f_e2,
	f_h2, f_a2, f_gis2, f_a2, f_h2, f_a2, f_gis2, f_a2, f_c3, f_a2,
	f_c3, f_g2, f_a2, f_h2, f_a2, f_g2, f_a2, f_g2, f_a2, f_h2, f_a2,
	f_g2, f_a2, f_h2, f_a2, f_g2, f_fis2, f_e2, f_pause, f_e2, f_f2,
	f_g2, f_pause, f_g2, f_a2, f_g2, f_f2, f_e2, f_d2, f_g1, f_e2, f_f2,
	f_g2, f_pause, f_g2, f_a2, f_g2, f_f2, f_e2, f_d2, f_pause, f_c2,
	f_d2, f_e2, f_pause, f_e2, f_f2, f_e2, f_d2, f_c2, f_h1, f_e1, f_c2,
	f_d2, f_e2, f_pause, f_e2, f_f2, f_e2, f_d2, f_c2, f_h1, f_pause,
	f_h1, f_a1, f_gis1, f_a1, f_c2, f_pause, f_d2, f_c2, f_h1, f_c2,
	f_e2, f_pause, f_f2, f_e2, f_dis2, f_e2, f_h2, f_a2, f_gis2, f_a2,
	f_h2, f_a2, f_gis2, f_a2, f_c3, f_a2, f_h2, f_c3, f_h2, f_a2,
	f_gis2, f_a2, f_e2, f_f2, f_d2, f_c2, f_c2, f_h1, f_a1, f_h1, f_a1,
	f_pause };
	const unsigned int lied_d[] = { d_n16, d_n16, d_n16, d_n16, d_n8, d_n8,
	d_n16, d_n16, d_n16, d_n16, d_n8, d_n8, d_n16, d_n16, d_n16, d_n16,
	d_n16, d_n16, d_n16, d_n16, d_n16, d_n16, d_n16, d_n16, d_n4, d_n8,
	d_n8, d_n32, d_n32, d_n16, d_n8, d_n8, d_n8, d_n32, d_n32, d_n16,
	d_n8, d_n8, d_n8, d_n8, d_n8, d_n8, d_n8, d_n8, d_n8, d_n8, d_n8,
	d_n16, d_n16, d_n8, d_n16, d_n16, d_n16, d_n16, d_n8, d_n8, d_n8,
	d_n8, d_n16, d_n16, d_n8, d_n16, d_n16, d_n16, d_n16, d_n8, d_n8,
	d_n8, d_n8, d_n16, d_n16, d_n8, d_n16, d_n16, d_n16, d_n16, d_n8,
	d_n8, d_n8, d_n8, d_n16, d_n16, d_n8, d_n16, d_n16, d_n16, d_n16,
	d_n8, d_n8, d_n16, d_n16, d_n16, d_n16, d_n8, d_n8, d_n16, d_n16,
	d_n16, d_n16, d_n8, d_n8, d_n16, d_n16, d_n16, d_n16, d_n16, d_n16,
	d_n16, d_n16, d_n16, d_n16, d_n16, d_n16, d_n4, d_n8, d_n8, d_n8,
	d_n8, d_n8, d_n8, d_n8, d_n8, d_n8, d_n8, d_n4, d_n16, d_n16, d_n16,
	d_n16, d_n4, d_ENDE };

//	// "Alle meine Entchen"
//	#define	BPM	240
//	#include "include/melodie.h"			// Deklaration der Tonfrequenz und Tondauer
//	const unsigned int lied_f[]  = {f_d1, f_e1, f_fis1, f_g1, f_a1, f_pause, f_a1, f_h1, f_pause, f_h1, f_pause, f_h1, f_pause, f_h1, f_a1, f_pause, f_h1, f_pause, f_h1, f_pause, f_h1, f_pause, f_h1, f_a1, f_pause, f_g1, f_pause, f_g1, f_pause, f_g1, f_pause, f_g1, f_fis1, f_pause, f_fis1, f_e1, f_pause, f_e1, f_pause, f_e1, f_pause, f_e1, f_d1, f_pause, f_a1};
//	const unsigned int lied_d[]  = {d_n4, d_n4, d_n4, d_n4, d_n2, d_n32, d_n2, d_n4, d_n32, d_n4, d_n32, d_n4, d_n32, d_n4, d_n2, d_n2, d_n4, d_n32, d_n4, d_n32, d_n4, d_n32, d_n4, d_n2, d_n2, d_n4, d_n32, d_n4, d_n32, d_n4, d_n32, d_n4, d_n2, d_n32, d_n2, d_n4, d_n32, d_n4, d_n32, d_n4, d_n32, d_n4, d_n2, d_n2, d_ENDE};

//	// "Fuchs, Du hast die Gans gestohlen...."
//	#define	BPM	240
//	#include "include/melodie.h"			// Deklaration der Tonfrequenz und Tondauer
//	const unsigned int lied_f[]  = {f_d1, f_e1, f_fis1, f_g1, f_a1, f_a1, f_a1, f_a1, f_h1, f_g1, f_d2, f_h1, f_a1, f_pause, f_h1, f_g1, f_d2, f_h1, f_a1, f_pause, f_a1, f_g1, f_g1, f_g1, f_g1, f_fis1, f_fis1, f_fis1, f_fis1, f_e1, f_fis1, f_e1, f_d1, f_pause, f_pause};
//	const unsigned int lied_d[]  = {d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n2, d_n2, d_n4, d_n4, d_n4, d_n4, d_n2, d_n2, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n2, d_n2, d_ENDE};

//	// "GREENSLEEVES"
//	#define	BPM	200
//	#include "include/melodie.h"			// Deklaration der Tonfrequenz und Tondauer
//	const unsigned int lied_f[]  = {f_a1, f_c2, f_d2, f_e2, f_e2, f_f2, f_e2, f_d2, f_h1, f_g1, f_g1, f_a1, f_h1, f_c2, f_a1, f_a1, f_a1, f_gis1, f_a1, f_h1, f_gis1, f_e1, f_a1, f_c2, f_d2, f_e2, f_e2, f_f2, f_e2, f_d2, f_h1, f_g1, f_g1, f_a1, f_h1, f_c2, f_c2, f_h1, f_a1, f_gis1, f_gis1, f_fis1, f_gis1, f_a1, f_a1, f_a1, f_a1, f_g2, f_g2, f_g2, f_g2, f_fis2, f_e2, f_d2, f_h1, f_g1, f_g1, f_a1, f_h1, f_c2, f_a1, f_a1, f_a1, f_gis1, f_a1, f_h1, f_gis1, f_e1, f_e1, f_g2, f_g2, f_g2, f_g2, f_fis2, f_e2, f_d2, f_h1, f_g1, f_g1, f_a1, f_h1, f_c2, f_c2, f_h1, f_a1, f_gis1, f_gis1, f_fis1, f_gis1, f_a1, f_a1, f_a1, f_a1};
//	const unsigned int lied_d[]  = {d_n4, d_n2, d_n4, d_n4, d_n8, d_n8, d_n4, d_n2, d_n4, d_n4, d_n8, d_n8, d_n4, d_n2, d_n4, d_n4, d_n8, d_n8, d_n4, d_n2, d_n4, d_n2, d_n4, d_n2, d_n4, d_n4, d_n8, d_n8, d_n4, d_n2, d_n4, d_n4, d_n8, d_n8, d_n4, d_n4, d_n8, d_n8, d_n4, d_n4, d_n8, d_n8, d_n4, d_n2, d_n4, d_n2, d_n4, d_n2, d_n4, d_n4, d_n8, d_n8, d_n4, d_n2, d_n4, d_n4, d_n8, d_n8, d_n4, d_n2, d_n4, d_n4, d_n8, d_n8, d_n4, d_n2, d_n4, d_n2, d_n4, d_n2, d_n4, d_n4, d_n8, d_n8, d_n4, d_n2, d_n4, d_n4, d_n8, d_n8, d_n4, d_n4, d_n8, d_n8, d_n4, d_n4, d_n8, d_n8, d_n4, d_n2, d_n4, d_n2, d_ENDE};

//	// "hopp, hopp, hopp, ..."
//	#define	BPM	240
//	#include "include/melodie.h"			// Deklaration der Tonfrequenz und Tondauer
//	const unsigned int lied_f[] = {f_g1, f_h1, f_d2, f_pause, f_d2, f_c2, f_h1, f_a1, f_g1, f_pause, f_pause, f_a1, f_a1, f_fis1, f_d1, f_d2, f_d2, f_h1, f_g1, f_a1, f_a1, f_fis1, f_d1, f_d2, f_d2, f_h1, f_g1, f_g1, f_a1, f_h1, f_c2, f_d2, f_pause, f_pause, f_d2, f_c2, f_h1, f_a1, f_g1, f_pause, f_pause};
//	const unsigned int lied_d[] = {d_n2, d_n2, d_n2, d_n2, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n2, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n2, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_ENDE};

// 	// "Lasst uns froh und munter sein ..."
//	#define	BPM	120
//	#include "include/melodie.h"			// Deklaration der Tonfrequenz und Tondauer
//	const unsigned int lied_f[] = {f_c2, f_c2, f_c2, f_d2, f_c2, f_b1, f_a1, f_a1, f_a1, f_b1, f_b1, f_b1, f_c2, f_b1, f_a1, f_g1, f_g1, f_g1, f_f1, f_g1, f_a1, f_b1, f_c2, f_d2, f_c2, f_d2, f_c2, f_f2, f_c2, f_c2, f_d2, f_c2, f_b1, f_a1, f_g1, f_c2, f_f2, f_c2, f_c2, f_d2, f_c2, f_b1, f_a1, f_g1, f_f1, f_pause};
//	const unsigned int lied_d[] = {d_n4, d_n4, d_n8, d_n8, d_n8, d_n8, d_n4, d_n4, d_n2, d_n4, d_n4, d_n8, d_n8, d_n8, d_n8, d_n4, d_n4, d_n2, d_n4, d_n4, d_n4, d_n4, d_n8, d_n8, d_n8, d_n8, d_n2, d_n4, d_n4, d_n8, d_n8, d_n8, d_n8, d_n4, d_n4, d_n2, d_n4, d_n4, d_n8, d_n8, d_n8, d_n8, d_n4, d_n4, d_n2, d_ENDE};

//	// "Ein M�nnlein steht im Walde ..."
//	#define	BPM	120
//	#include "include/melodie.h"			// Deklaration der Tonfrequenz und Tondauer
//	const unsigned int lied_f[] = {f_c1, f_f1, f_g1, f_a1, f_b1, f_c2, f_d2, f_b1, f_a1, f_g1, f_f1,  f_c1, f_f1, f_g1, f_a1, f_b1, f_c2, f_d2, f_b1, f_a1, f_g1, f_f1,  f_pause, f_c2,  f_b1, f_a1, f_c2, f_b1, f_a1, f_g1, f_c2, f_b1, f_a1, f_c2, f_b1, f_a1, f_g1, f_f1, f_g1, f_a1, f_b1, f_c2, f_d2, f_b1, f_a1, f_g1, f_f1, f_pause, f_pause};
//	const unsigned int lied_d[] = {d_n4, d_n4, d_n4, d_n4, d_n4, d_n2, d_n4, d_n4, d_n2, d_n2, d_n2p, d_n4, d_n4, d_n4, d_n4, d_n4, d_n2, d_n4, d_n4, d_n2, d_n2, d_n2p, d_n4,    d_n4p, d_n8, d_n4, d_n4, d_n4, d_n4, d_n2, d_n4p, d_n8, d_n4, d_n4, d_n4, d_n4, d_n2, d_n4, d_n4, d_n4, d_n4, d_n2, d_n4, d_n4, d_n2, d_n2, d_n2, d_n4, d_ENDE};

//	// "Et�de aus OP.No.3", Frederic Chopin
//	#define	BPM	120
//	#include "include/melodie.h"			// Deklaration der Tonfrequenz und Tondauer
//	const unsigned int lied_f[] = {f_pause, f_dis2, f_e2, f_h2, f_dis2, f_e2, f_h1, f_h1, f_h1, f_dis2, f_e2, f_h2, f_dis2, f_e2, f_h1, f_h2, f_a2, f_gis2, f_e2, f_e1, f_dis1, f_e1, f_fis1, f_fis1, f_gis1, f_gis1, f_fis1, f_gis1, f_gis1, f_a1, f_a1, f_gis1, f_cis2, f_h1, f_a1, f_gis1, f_dis1, f_e1, f_fis1, f_fis1, f_gis1, f_gis1, f_fis1, f_e1, f_gis1, f_a1, f_fis1, f_gis1, f_a1, f_h1, f_gis1, f_a1, f_cis2, f_fis1, f_a1, f_gis1, f_fis1, f_a1, f_gis1, f_fis1, f_h1, f_pause, f_e1, f_dis1, f_e1, f_fis1, f_fis1, f_gis1, f_gis1, f_fis1, f_gis1, f_pause, f_a1, f_a1, f_gis1, f_cis2, f_h1, f_a1, f_gis1, f_dis1, f_e1, f_fis1, f_fis1, f_gis1, f_gis1, f_fis1, f_e1, f_h1, f_cis2, f_cis2, f_h1, f_a1, f_h1, f_gis1, f_a1, f_dis2, f_e2, f_e2, f_dis2, f_cis2, f_dis2, f_c2, f_cis2, f_e2, f_fis2, f_dis2, f_e2, f_fis2, f_gis2, f_e2, f_fis2, f_gis2, f_pause, f_fis2, f_e2, f_cis2, f_dis2, f_e2, f_dis2, f_cis2, f_gis1, f_h1, f_cis2, f_h1, f_a1, f_e1, f_gis1, f_e1, f_fis1, f_gis1, f_pause};
//	const unsigned int lied_d[] = {d_n8, d_n8, d_n8, d_n4, d_n8, d_n8, d_n4, d_n2, d_n2, d_n8, d_n8, d_n4, d_n8, d_n8, d_n2, d_n8, d_n8, d_n8, d_n4, d_n4, d_n8, d_n8, d_n2, d_n8, d_n8, d_n8, d_n8, d_n2, d_n8, d_n8, d_n8, d_n8, d_n2, d_n8, d_n8, d_n8, d_n8, d_n8, d_n2, d_n8, d_n8, d_n8, d_n8, d_n2, d_n8, d_n8, d_n8, d_n8, d_n8, d_n8, d_n8, d_n8, d_n4, d_n2, d_n64, d_n8, d_n2, d_n64, d_n8, d_n8, d_n4, d_n4, d_n4, d_n8, d_n8, d_n2, d_n8, d_n8, d_n8, d_n8, d_n2, d_n8, d_n8, d_n8, d_n8, d_n2, d_n8, d_n8, d_n8, d_n8, d_n8, d_n2, d_n8, d_n8, d_n8, d_n8, d_n2, d_n8, d_n8, d_n8, d_n8, d_n8, d_n8, d_n8, d_n8, d_n8, d_n8, d_n8, d_n8, d_n8, d_n8, d_n8, d_n8, d_n8, d_n8, d_n8, d_n8, d_n8, d_n8, d_n8, d_n8, d_n2, d_n8, d_n8, d_n8, d_n8, d_n2, d_n8, d_n8, d_n8, d_n8, d_n2, d_n8, d_n8, d_n8, d_n8, d_n2, d_n2, d_n2, d_n2, d_ENDE};

//	// "Es tanzt ein Bibabutzelmann ..."
//	#define	BPM	120
//	#include "include/melodie.h"			// Deklaration der Tonfrequenz und Tondauer
//	const unsigned int lied_f[] = {f_d1, f_g1, f_g1, f_d2, f_d2, f_h1, f_h1, f_g1, f_g1, f_a1, f_a1, f_d1, f_d1, f_g1, f_pause, f_d1, f_g1, f_g1, f_d2, f_d2, f_h1, f_h1, f_g1, f_g1, f_a1, f_a1, f_d1, f_d1, f_g1, f_pause, f_h1, f_a1, f_h1, f_c2, f_a1, f_h1, f_c2, f_d2, f_h1, f_a1, f_h1, f_c2, f_a1, f_h1, f_c2, f_d2, f_d1, f_g1, f_g1, f_d2, f_d2, f_h1, f_h1, f_g1, f_g1, f_a1, f_a1, f_d1, f_d1, f_g1, f_pause, f_pause};
//	const unsigned int lied_d[] = {d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n2, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n2, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_n4, d_ENDE};

	unsigned int *p_freq, *p_dauer;			// Deklaration Zeiger

	p_freq = (unsigned int *) &lied_f;		// Zeiger auf Frequenz
	p_dauer = (unsigned int *) &lied_d;		// Zeiger auf Dauer

	AT91C_BASE_PIOA->PIO_OER = AT91C_PIO_PA18;// Freigabe des Lautsprecher-Port-Pins
	AT91C_BASE_PIOA->PIO_OWER = AT91C_PIO_PA18;	// Freigabe des schreibenden/lesenden Zugriffs
	AT91C_BASE_PIOA->PIO_PPUDR = AT91C_PIO_PA18;// Ausschalten des Pull-up-Widerstands

	timer_ir_init(0, 1, 1, timer0_isr);	// Timer0-IR initialisieren (Tondauer)
	timer_ir_init(1, 4, 1, timer1_isr);	// Timer1-IR initialisieren	(Tonh�he)

	AT91C_BASE_TC0->TC_RC = 3000;			// max. Z�hlerwert f�r TC0
	AT91C_BASE_TC0->TC_CCR = AT91C_TC_SWTRG;// Zur�cksetzen des Z�hlers Timer0

	do {
		uiDauer = *(p_dauer++);
		uiFrequenz = *(p_freq++);
		if (uiDauer == 0)
			break;
		AT91C_BASE_TC1->TC_RC = uiFrequenz;
		AT91C_BASE_TC1->TC_CCR = AT91C_TC_SWTRG;// Zur�cksetzen des Z�hlers Timer0
		AT91C_BASE_TC0->TC_CCR = AT91C_TC_SWTRG;// Zur�cksetzen des Z�hlers Timer0
		while (uiDauer != 0)
			;

	} while (1);
}
