//********************************************************************
// ARM_T3A1										Version 02.04.2012 hpw
//********************************************************************
#include	"include/AT91SAM7S64.h"
#include	"include/display.h"
#include	"include/i2c_tools.h"

//**************************************************************
// delay5ms(uiK) verzögert um ca. uiK * 5ms
//**************************************************************
void delay5ms(unsigned int uiK){
	volatile unsigned int uiI;

	while (uiK--)							// folgende for-Schleife wird uiK-mal aufgerufen
		for (uiI=0; uiI<8192; uiI++);		//  for-Schleife wird 8192-mal durchlaufen. Dies
											//  verursacht eine Zeitverzögerung von ca. 5ms.
}

//**************************************************************
// tempwert(MSByte,LSByte) gibt siTemp zurück
//**************************************************************
signed int tempwert(unsigned char MSByte, unsigned char LSByte){
	signed int siTemp;

	siTemp = ((MSByte<<24)+(LSByte<<16))>>23;
	return siTemp;
}

//*Alternativ
//	union{
//		signed int si;
//		unsigned char uc[4];
//	} DATA;
//
//	DATA.uc[2] = LSByte;
//	DATA.uc[3] = MSByte;
//	return (DATA.si>>23);
//}

//**************************************************************
// print_tempwert(cTemp) gibt Übergabewert auf LCD aus
//**************************************************************
void print_temp(signed int cTemp){			// Funktion zur Ausgabe des Temperaturwertes
	unsigned int ucL;

	if(cTemp < 0){							// Fallunterscheidung cTemp=negativ?
		display_putchar('-');
		ucL = (unsigned int) -cTemp;}
	else{
		display_putchar('+');
		ucL = (unsigned int) cTemp;}

	if((ucL>>1)<10)display_putchar(' ');	// Formartierte Ausgabe
	display_printf("%d",ucL>>1);			// Ausgabe als unsigned int
	display_putchar(',');
	if (cTemp & 1) display_putchar('5');	// Ausgabe der Nachkommastelle
	else display_putchar('0');
	display_putchar(0xDF);					// Ausgabe der Massangabe
	display_putchar('C');
}

//**************************************************************
// main()
//**************************************************************
int main(){
	#define Adresse 0x9E			// evtl 9F für schreiben?
	#define	Read	1
	#define Write	0
	unsigned char ucTemp1,ucTemp2;			// Deklaration der Variablen
//	unsigned char Adresse = 0b10011111;

	display_init();							// Initialisiere Display

	while(1) {
		while(i2c_start(Adresse+Read));		// Führt die Startbedingung aus
//		display_printf("%d", ACK);
		ucTemp1 = i2c_rbyte(0);					// Erstes Byte einlesen
		ucTemp2 = i2c_rbyte(1);					// Zweites Byte einlesen
		i2c_stop();									// I2C wieder freigeben

		display_set_cursor(0, 0);
		display_printf("%x   %x  ", ucTemp1, ucTemp2);
		display_set_cursor(0, 1);
		print_temp(tempwert(ucTemp1, ucTemp2));											// Programm zur Verarbeitung der
													// eingelesenen Werte
		delay5ms(100);							// Verzögerung 100 ms
	}


	while(1);								// Endlosschleife
}
