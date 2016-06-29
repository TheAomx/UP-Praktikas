//********************************************************************
// ARM_T3A4										Version 02.04.2012 hpw
//********************************************************************
#include	"include/AT91SAM7S64.h"
#include	"include/display.h"
#include	"include/twi_tools.h"


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
/** 1. Schiebt das MSByte an die letzten 8 Bit des 32 bit großen signed int
 *  2. Schiebt das LSByte an die vorletzten 8 Bit des signed int
 *  3. Schiebt um 23 Stellen nach rechts damit 9 Bits überbleiben
 *
 *  Die ersten 8Bits bestimmen die Vorkommastelle und das letzte Bit die Nachkomma-
 *  Stelle (0 oder 0.5) der Temperatur
 *
 */
signed int tempwert(unsigned char MSByte, unsigned char LSByte){
	signed int siTemp;

	siTemp = ((MSByte<<24)+(LSByte<<16))>>23;
	return siTemp;
}

/** Union stellt ein gemeinsamen Speicherbeich dar.
 *  Der Speicherplatz bedarf einer Union wird über die größte
 *  Komponente bestimmt. In diesem Falle 32Bit für das signed int.
 *
 *  Dementspreched werden über DATA.uc[2] und DATA.uc[3] die letzten
 *  und vorletzten 8 Bit der Union addressiert. Wenn nun DATA.si um
 *  23 Stellen nach rechts geschoben wird, werden dabei die zuvor beschriebenen
 *  32bit des Unions verschoben. Sodass am Ende eine 9 Bit Große Zahl zurückgegenen wird
 *  wie bei tempwert
 *
 */
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

	display_init();							// Initialisiere Display
	twi_init();

	int i;
	while(1) {
		signed int avg = 0;
		for(i = 0; i < 64; i++) {
			// Setzen der Startbedingung und warten bis slave antwortet mit ACK0
			while(i2c_start(Adresse+Read));
			ucTemp1 = i2c_rbyte(0);					// Erstes Byte einlesen
			ucTemp2 = i2c_rbyte(1);					// Zweites Byte einlesen
			i2c_stop();								// I2C wieder freigeben

			// Ausgabe der gelesenen Werte

			display_set_cursor(0, 0);
			print_temp(tempwert(ucTemp1, ucTemp2));
			avg += tempwert(ucTemp1, ucTemp2);
			delay5ms(100);							// Verzögerung 100 ms
		}
		display_set_cursor(0, 1);
		print_temp(avg >> 6);
	}
	while(1);								// Endlosschleife
}
