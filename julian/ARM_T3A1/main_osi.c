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

void set_tos_temperature () {
	unsigned char WRITE_ADDRESS = 0x9E;			// slave adresse binär: 1001 1111
	unsigned char SET_TOS   = 0x03;
	// temperature binär: 0001 0100 00000000
	unsigned char byte_1 = 0x14;
	unsigned char byte_2 = 0x00;

	i2c_start(WRITE_ADDRESS);						// Führt die Startbedingung aus
	i2c_wbyte(SET_TOS);
	i2c_wbyte(byte_1);
	i2c_wbyte(byte_2);
	i2c_stop();
}

void set_thyst_temperature () {
	unsigned char WRITE_ADDRESS = 0x9E;			// slave adresse binär: 1001 1111
	unsigned char SET_THYST = 0x02;
	// temperature binär: 0001 1001 00000000
	unsigned char byte_1 = 0x14;
	unsigned char byte_2 = 0x00;

	i2c_start(WRITE_ADDRESS);						// Führt die Startbedingung aus
	i2c_wbyte(SET_THYST);
	i2c_wbyte(byte_1);
	i2c_wbyte(byte_2);
	i2c_stop();
}

void setup_configuration_register () {
	unsigned char WRITE_ADDRESS = 0x9E;			// slave adresse binär: 1001 1111
	unsigned char pointer_byte = 0x01;
	unsigned char configuration = 0x04;			// 0000 0100

	i2c_start(WRITE_ADDRESS);						// Führt die Startbedingung aus
	i2c_wbyte(pointer_byte);
	i2c_wbyte(configuration);
	i2c_stop();

}

void setup_temperature_read_mode() {
	unsigned char WRITE_ADDRESS = 0x9E;			// slave adresse binär: 1001 1111
	unsigned char pointer_byte = 0x00;

	i2c_start(WRITE_ADDRESS);						// Führt die Startbedingung aus
	i2c_wbyte(pointer_byte);
	i2c_stop();
}

//**************************************************************
// main()
//**************************************************************
int main(){
	unsigned char ucTemp1, ucTemp2;			// Deklaration der Variablen
	unsigned char READ_ADDRESS = 0x9F;			// slave adresse binär: 1001 1111

	display_init();							// Initialisiere Display

	setup_configuration_register();
	set_thyst_temperature();
	set_tos_temperature();

	setup_temperature_read_mode();

	while (1) {
		i2c_start(READ_ADDRESS);				// Führt die Startbedingung aus
		ucTemp1 = i2c_rbyte(1);					// Erstes Byte einlesen
		ucTemp2 = i2c_rbyte(0);					// Zweites Byte einlesen
		i2c_stop();								// I2C wieder freigeben

		signed int temp = tempwert(ucTemp1, ucTemp2);
		print_temp(temp);

		delay5ms(20);							// Verzögerung 100 ms
		display_clear();
	}

	while(1);								// Endlosschleife
}
