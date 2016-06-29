//********************************************************************
// ARM_T5A3a									Version 02.04.2012 hpw
//********************************************************************
#include "include/AT91SAM7S64.h"
#include "include/display.h"
#include "include/cToolMod.h"

//#define 	CONTROL_REGISTER 			AT91C_ADC_CR
//#define 	CHANNEL_ENABLE_REGISTER		AT91C_ADC_CHER
//#define		STATUS_REGISTER				AT91C_ADC_SR
//#define		CH4							AT91C_ADC_CDR4


//**************************************************************
// main()
//**************************************************************
int main(){
	display_init();

	//Enables the 4th channel
	//AT91C_BASE_ADC->ADC_CHER |= (1 << 4);
	AT91C_BASE_ADC->ADC_CHER |= AT91C_ADC_CH4;
	while (1) {

		//Startet die Wandlung muss jedes mal aufgerufen werden
		AT91C_BASE_ADC->ADC_CR |= AT91C_ADC_START;
		//Check if data is ready: Returns 1 if data is rdy
		//Check DReady register
		if ((AT91C_BASE_ADC->ADC_SR & (1 << 16)) > 0) {
		//Check EOC4 register
//		if ((AT91C_BASE_ADC->ADC_SR & AT91C_ADC_EOC4) > 0) {
			unsigned int data = AT91C_BASE_ADC->ADC_CDR4; //CDR4 Register
			display_set_cursor(0,0);
			display_printf("Data %d", data);
			unsigned int data2 = AT91C_BASE_ADC->ADC_LCDR; //Latest AD-Wandlung
			display_set_cursor(0,1);
			display_printf("Data %d", data2);

		}
		delay5ms(100);
	}
	while(1);
}
