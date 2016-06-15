//********************************************************************
// ARM_T3A1										Version 02.04.2012 hpw
//********************************************************************

#include	"include/AT91SAM7S64.h"
#include	"include/display.h"
#include	"include/i2c_tools.h"

typedef unsigned char uchar_t;

#define ACK_CONSTANT  0
#define NACK_CONSTANT 1

#define		PH2			AT91C_PIO_PA23				// Alle Ports die für den Motor verwendet werden
#define		PH1			AT91C_PIO_PA13
#define		I21			AT91C_PIO_PA20
#define		I11			AT91C_PIO_PA14
#define		I20			AT91C_PIO_PA16
#define		I10			AT91C_PIO_PA15
#define		MOTOR_MASK	(PH2|PH1|I21|I11|I20|I10)


void delay_ms(unsigned int uiK){
	volatile unsigned int uiI;

	while (uiK--)
		for (uiI=0; uiI<1638; uiI++);

}

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

signed int read_current_temperature () {
	uchar_t ucTemp1, ucTemp2;
	uchar_t slave_address = 0x9F;		// binary coded: 1001 1111
	uchar_t ret;
	signed int temperature = -1;

	ret = i2c_start(slave_address);

	if (ret == 0) {
		ucTemp1 = i2c_rbyte(ACK_CONSTANT);  // Erstes Byte einlesen
		ucTemp2 = i2c_rbyte(NACK_CONSTANT); // Zweites Byte einlesen
		temperature = tempwert(ucTemp1, ucTemp2);
		i2c_stop();
	}

	return temperature;
}

typedef void (*read_callback) (signed int current_temperature);

void read_temperatures (signed int *buffer, signed int num_elements, unsigned int delay, read_callback cb) {
	uchar_t slave_address = 0x9F;		// binary coded: 1001 1111
	uchar_t ucTemp1, ucTemp2;

	for (signed int i = 0; i < num_elements; i++) {
		while (i2c_start(slave_address) == 1) {
			display_clear();
			display_putstr("start failed!");
			delay_ms(500);
		}

		ucTemp1 = i2c_rbyte(ACK_CONSTANT);  // Erstes Byte einlesen
		ucTemp2 = i2c_rbyte(NACK_CONSTANT); // Zweites Byte einlesen
		buffer[i] = tempwert(ucTemp1, ucTemp2);
		cb(buffer[i]);
#ifndef USE_TWI
		i2c_stop();
#endif
		delay_ms(delay);
	}
#ifdef USE_TWI
	i2c_stop();
#endif
}

void print_current_temperature (signed int current_temperature) {
	display_set_cursor(0,0);
	print_temp(current_temperature);
}

#define NUM_TEMP_VALUES 64

signed int calc_mean_temperature(signed int *measurements, signed int num_measurements) {
	signed int mean_temperature = 0;

	for (signed int i = 0; i < num_measurements; i++) {
		mean_temperature += measurements[i];
	}

	mean_temperature = mean_temperature >> 6;
	return mean_temperature;
}

void versuch_3 () {
	signed int temperature, mean_temperature;

	while (1) {
		display_clear();
		temperature = read_current_temperature();
		mean_temperature = calc_mean_temperature(&temperature, 1);
		print_temp(temperature);
		display_set_cursor(0, 1);
		if (mean_temperature != 0) {
			print_temp(mean_temperature);
		}
		delay_ms(500);
	}
}

void versuch_4 () {
	unsigned int delay = 100;
	signed int mean_temperature;
	signed int temperatures [NUM_TEMP_VALUES];

	while (1) {
			display_clear();
			read_temperatures(temperatures, NUM_TEMP_VALUES, delay, print_current_temperature);
			mean_temperature = calc_mean_temperature(temperatures, NUM_TEMP_VALUES);
			print_temp(temperatures[0]);
			display_set_cursor(0, 1);
			if (mean_temperature != 0) {
				print_temp(mean_temperature);
			}
	}
}

void versuch_5 () {
	uchar_t slave_address = 0x9F;		// binary coded: 1001 1111

	i2c_start(slave_address);
}

int main(){
	display_init();


#ifdef USE_TWI
	twi_init();
#endif

#ifdef USE_TWI
	display_putstr("using twi");
#else
	display_putstr("not using twi");
#endif

	delay_ms(1000);

#if 0
	versuch_3();
#endif
#if 0
	versuch_4();
#endif
#if 0
	versuch_5();
#endif


	display_printf("%x", MOTOR_MASK);
	while (1);
}
