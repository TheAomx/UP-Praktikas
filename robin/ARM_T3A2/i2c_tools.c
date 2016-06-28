//********************************************************************
// i2c_tools.c								Version 24.06.2014_stud cs
//********************************************************************
#include	"include/AT91SAM7S64.h"

#define		SDA		AT91C_PIO_PA3					// SDA auf PA3
#define		SCL		AT91C_PIO_PA4					// SCL auf PA4

//**************************************************************
// i2c_stop
// Generiert eine Stoppbedingung auf dem I2C Bus
//**************************************************************
void i2c_stop(void){
	AT91C_BASE_PIOA->PIO_CODR = SDA;				// SDA -> 0
	AT91C_BASE_PIOA->PIO_SODR = SCL;				// SCL -> 1
	{	asm("NOP"); asm("NOP"); asm("NOP");			// Stop condition setup time
		asm("NOP"); asm("NOP"); asm("NOP");
		asm("NOP"); asm("NOP"); asm("NOP");
		asm("NOP"); asm("NOP"); asm("NOP");
		asm("NOP"); asm("NOP"); asm("NOP");
		asm("NOP"); asm("NOP"); asm("NOP");
	}
	AT91C_BASE_PIOA->PIO_SODR = SDA;				// SDA -> 1
}

//**************************************************************
// i2c_rbit
// Ein Bit von der Datenleitung lesen
//**************************************************************
unsigned char i2c_rbit(void){
	unsigned char bit;								// Variablendeklaration

	AT91C_BASE_PIOA->PIO_SODR = SCL;				// SCL -> 1
	{	asm("NOP"); asm("NOP"); asm("NOP");
		asm("NOP"); asm("NOP"); asm("NOP");
		asm("NOP"); asm("NOP"); asm("NOP");
		asm("NOP"); asm("NOP"); asm("NOP");
		asm("NOP"); asm("NOP"); asm("NOP");
	}
	if (AT91C_BASE_PIOA->PIO_PDSR & SDA) bit=1;		// Pegel von SDA lesen
	else bit=0;										// und bit zuweisen
	AT91C_BASE_PIOA->PIO_CODR = SCL;				// SCL -> 0
	return bit;										// Eingelesenes Bit zur�ck
}

//**************************************************************
// i2c_wbit
// Ein Bit auf die Datenleitung legen
//**************************************************************
void i2c_wbit(unsigned char bit){
	if(bit) AT91C_BASE_PIOA->PIO_SODR = SDA;		// Ausgabe von bit: High
	else AT91C_BASE_PIOA->PIO_CODR = SDA;			//                  Low
	{	asm("NOP"); asm("NOP"); asm("NOP");			// Setup time
		asm("NOP"); asm("NOP"); asm("NOP");
		asm("NOP"); asm("NOP"); asm("NOP");
	}
	AT91C_BASE_PIOA->PIO_SODR = SCL;				// SCL -> 1
	{	asm("NOP"); asm("NOP"); asm("NOP");			// Hold time and clock high time
		asm("NOP"); asm("NOP"); asm("NOP");
		asm("NOP"); asm("NOP"); asm("NOP");
		asm("NOP"); asm("NOP"); asm("NOP");
		asm("NOP"); asm("NOP"); asm("NOP");
		asm("NOP"); asm("NOP"); asm("NOP");
		asm("NOP"); asm("NOP"); asm("NOP");
		asm("NOP"); asm("NOP"); asm("NOP");
	}
	AT91C_BASE_PIOA->PIO_CODR = SCL;				// SCL -> 0
}

//**************************************************************
// i2c_wbyte
// Ein Byte auf den I2C Bus schreiben
// �bergabewert:	Byte, welches geschrieben werden soll
// R�ckgabewert:	ACK
//**************************************************************
unsigned char i2c_wbyte(unsigned char byte){
	signed char i;									// Variablendeklaration

	for(i=7;i>=0;i--){								// Jedes Bit seriell senden
		i2c_wbit(((1<<i) & byte)>>i);				// Bit aus dem Byte lesen + senden
	}
	AT91C_BASE_PIOA->PIO_SODR = SDA;				// SDA -> 1 - Leitung f�r ACK freigeben
	if (i2c_rbit()){								// falls keine R�ckmeldung vom Slave (ACK=1):
		i2c_stop();									//  Fehler	-> Ende des Protokolls
		return 1;									// 			-> zur�ck mit ACK = 1
	}
	return 0;										// zur�ck mit ACK = 0
}

//**************************************************************
// i2c_start
// Startbedingung auf dem I2C-Bus generieren
// �bergabewert:	Adresse des I2C Bausteines inkl. RW
//					A6	A5	A4	A3	A3	A1	A0	RW
//					A[6..0] :	Bausteinadresse
//					RW = 0  :	N�chter Zugriff schreibend
//					RW = 1  :	N�chter Zugriff lesend
// R�ckgabewert:	ACK = 0 :	Slave hat erfolgreich quittiert
//					ACK = 1 :	kein Slave hat geantwortet
//**************************************************************
unsigned char i2c_start(unsigned char sladr){
	AT91C_BASE_PMC->PMC_PCER = (1<<AT91C_ID_PIOA);	// PIO-Takt einschalten
	AT91C_BASE_PIOA->PIO_MDER = (SDA|SCL);			// Pins auf 'Open Drain'
	AT91C_BASE_PIOA->PIO_PPUDR = (SDA|SCL);			// Pull-Ups aus
	AT91C_BASE_PIOA->PIO_SODR = (SDA|SCL);			// SDA+SCL -> 1
	AT91C_BASE_PIOA->PIO_OER = (SDA|SCL);			// SDA, SCL aktivieren
	{	asm("NOP"); asm("NOP"); asm("NOP");			// Start condition setup time
		asm("NOP"); asm("NOP"); asm("NOP");
		asm("NOP"); asm("NOP"); asm("NOP");
		asm("NOP"); asm("NOP"); asm("NOP");
	}
	AT91C_BASE_PIOA->PIO_CODR = SDA;				// Start condition hold time
	{	asm("NOP"); asm("NOP"); asm("NOP");
		asm("NOP"); asm("NOP"); asm("NOP");
		asm("NOP"); asm("NOP"); asm("NOP");
		asm("NOP"); asm("NOP"); asm("NOP");
	}
	AT91C_BASE_PIOA->PIO_CODR = SCL;				// SCL -> 0
	return i2c_wbyte(sladr);						// ACK zur�ck
}

//**************************************************************
// i2c_rbyte
// Ein Byte vom I2C Bus lesen
// �bergabewert:	ACK = 0:	Es soll anschlie�end ein weiteres Byte gelesen werden
//					ACK = 1:	Im Anschlu� Beendigung der Daten�bertragung
// R�ckgabewert:	gibt ausgelesenes Byte zur�ck
//**************************************************************
unsigned char i2c_rbyte(unsigned char ACK){
	unsigned char byte = 0;
	signed char i;									// Variablendeklaration

	for(i=7;i>=0;i--){								// Jedes Bit seriell senden
			byte |=(i2c_rbit()<<i);				// Bit aus dem Byte lesen + senden
	}
	i2c_wbit(ACK);
	AT91C_BASE_PIOA->PIO_SODR = SDA;				// SDA -> 1 - Leitung f�r ACK freigeben

	return byte;
}
