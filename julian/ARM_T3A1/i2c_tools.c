//********************************************************************
// i2c_tools.c								Version 15.05.2014_std hpw
//********************************************************************
#include	"include/AT91SAM7S64.h"
#include "include/i2c_tools.h"

#define		SDA		AT91C_PIO_PA3					// SDA auf PA3
#define		SCL		AT91C_PIO_PA4					// SCL auf PA4


#ifndef USE_TWI
static void i2c_wbit(unsigned char bit);

//**************************************************************
// i2c_stop
// Generiert eine Stoppbedingung auf dem I2C Bus
//**************************************************************
void i2c_stop(void){
	AT91C_BASE_PIOA->PIO_CODR = SDA;				// SDA -> 0
	AT91C_BASE_PIOA->PIO_SODR = SCL;				// SCL -> 1
	AT91C_BASE_PIOA->PIO_SODR = SDA;				// SDA -> 1
}

//**************************************************************
// i2c_rbit
// Ein Bit von der Datenleitung lesen
//**************************************************************
unsigned char i2c_rbit(void){
	unsigned char bit;								// Variablendeklaration

	AT91C_BASE_PIOA->PIO_SODR = SCL;				// SCL -> 1
	if (AT91C_BASE_PIOA->PIO_PDSR & SDA) bit=1;		// Pegel von SDA lesen
	else bit=0;										//	und bit zuweisen
	AT91C_BASE_PIOA->PIO_CODR = SCL;				// SCL -> 0
	return bit;										// Eingelesenes Bit zurück
}


//**************************************************************
// i2c_rbyte
// Ein Byte vom I2C Bus lesen
// Übergabewert:	ACK = 0:	Es soll anschließend ein weiteres Byte gelesen werden
//					ACK = 1:	Im Anschluß Beendigung der Datenübertragung
// Rückgabewert:	gibt ausgelesenes Byte zurück
//**************************************************************
unsigned char i2c_rbyte(unsigned char ack) {
	unsigned char byte = 0;
	signed char i;

	for(i=7;i>=0;i--) {
		byte |= i2c_rbit() << i;
	}

	i2c_wbit(ack);

	AT91C_BASE_PIOA->PIO_SODR = SDA;

	return byte;
}

//**************************************************************
// i2c_wbit
// Ein Bit auf die Datenleitung legen
//**************************************************************
void i2c_wbit(unsigned char bit){
	if(bit) AT91C_BASE_PIOA->PIO_SODR = SDA;		// Ausgabe von bit: High
	else AT91C_BASE_PIOA->PIO_CODR = SDA;			//                  Low
	AT91C_BASE_PIOA->PIO_SODR = SCL;				// SCL -> 1
	{	asm("NOP"); asm("NOP"); asm("NOP");			// ca. 600ns warten
		asm("NOP"); asm("NOP"); asm("NOP");
		asm("NOP"); asm("NOP"); asm("NOP");			// (14/24MHz = 583,33ns)
		asm("NOP"); asm("NOP"); asm("NOP");
		asm("NOP"); asm("NOP");
	}
	AT91C_BASE_PIOA->PIO_CODR = SCL;				// SCL -> 0
}

//**************************************************************
// i2c_wbyte
// Ein Byte auf den I2C Bus schreiben
// Übergabewert:	Byte, welches geschrieben werden soll
// Rückgabewert:	ACK
//**************************************************************
unsigned char i2c_wbyte(unsigned char byte){
	signed char i;									// Variablendeklaration

	for(i=7;i>=0;i--){								// Jedes Bit seriell senden
		i2c_wbit(((1<<i) & byte)>>i);				// Bit aus dem Byte lesen + senden
	}
	if (i2c_rbit()){								// falls keine Rückmeldung vom Slave (ACK=1):
		i2c_stop();									//  Fehler	-> Ende des Protokolls
		return 1;									// 			-> zurück mit ACK = 1
	}
	return 0;										// zurück mit ACK = 0
}

//**************************************************************
// i2c_start
// Startbedingung auf dem I2C-Bus generieren
// Übergabewert:	Adresse des I2C Bausteines inkl. RW
//					A6	A5	A4	A3	A3	A1	A0	RW
//					A[6..0] :	Bausteinadresse
//					RW = 0  :	Nächter Zugriff schreibend
//					RW = 1  :	Nächter Zugriff lesend
// Rückgabewert:	ACK = 0 :	Slave hat erfolgreich quittiert
//					ACK = 1 :	kein Slave hat geantwortet
//**************************************************************
unsigned char i2c_start(unsigned char sladr){
	AT91C_BASE_PMC->PMC_PCER = (1<<AT91C_ID_PIOA);	// PIO-Takt einschalten
	AT91C_BASE_PIOA->PIO_MDER = (SDA|SCL);			// Pins auf 'Open Drain'
	AT91C_BASE_PIOA->PIO_PPUDR = (SDA|SCL);			// Pull-Ups aus
	AT91C_BASE_PIOA->PIO_SODR = (SDA|SCL);			// SDA+SCL -> 1
	AT91C_BASE_PIOA->PIO_OER = (SDA|SCL);			// SDA, SCL aktivieren
	AT91C_BASE_PIOA->PIO_CODR = SDA;				// SDA -> 0
	AT91C_BASE_PIOA->PIO_CODR = SCL;				// SCL -> 0
	if (!i2c_wbyte(sladr)) return 0;				// falls Slave antwortet -> raus, ACK = 0
	return 1;										// Slave hat nicht geantwortet -> ACK = 1
}

#endif

#ifdef USE_TWI
//**********************************************************
// twi_init
// TWI-Controller-Modul initialisieren
//**********************************************************
void twi_init(void){
	AT91C_BASE_PIOA->PIO_PDR=(AT91C_PA3_TWD|AT91C_PA4_TWCK);	// PIO-Modus auf Pheriperie
	AT91C_BASE_PIOA->PIO_ASR=(AT91C_PA3_TWD|AT91C_PA4_TWCK);	// TWIContoller-Modul als Pheripherie A auswählen
	AT91C_BASE_PIOA->PIO_MDER=(AT91C_PA3_TWD|AT91C_PA4_TWCK);	// Pins als 'open drain'
	AT91C_BASE_PIOA->PIO_PPUDR=(AT91C_PA3_TWD|AT91C_PA4_TWCK);	// Pull-up-Widerstände aus

	AT91C_BASE_PMC->PMC_PCER=(1<<AT91C_ID_TWI);					// TWI-Clock einschalten

	AT91C_BASE_TWI->TWI_CR = AT91C_TWI_SWRST;					// TWI-Controller-Modul Reset


	// TWI Clock Waveform Generator Register; MCK = 24.0MHz
//	AT91C_BASE_TWI->TWI_CWGR=0x0002EAEA;	// LOW: ~12.8kHz, CKDIV=2, CLDIV=234, CHDIV=234
	AT91C_BASE_TWI->TWI_CWGR=0x00007575;	// SM :   100kHz, CKDIV=0, CLDIV=117, CHDIV=117
//	AT91C_BASE_TWI->TWI_CWGR=0x00001B1B;	// FM :   400kHz, CKDIV=0, CLDIV= 27, CHDIV=027
//	AT91C_BASE_TWI->TWI_CWGR=0x00000909;	// FM+:   1.0MHz, CKDIV=0, CLDIV=  9, CHDIV=  9
//	AT91C_BASE_TWI->TWI_CWGR=0x00000101;	// HS :   3.4MHz, CKDIV=0, CLDIV=  1, CHDIV=  1
}

//**********************************************************
// i2c_start
// Generiert eine Startbedingung auf dem I2C-Bus
// Übergabewert:	Adresse des I2C Bausteines inkl. RW
//					A6	A5	A4	A3	A3	A1	A0	RW
//					A[6..0] :	Bausteinadresse
//					RW = 0  :	Nächter Zugriff schreibend
//					RW = 1  :	Nächter Zugriff lesend
// Rückgabewert:	ACK :		Wird hier nicht unterstützt.
//								Aus Kompatibilitätsgründen
//								zu 'i2c_tools.c' wird immer
//								'0' zurückgegeben
//**********************************************************
unsigned char i2c_start(unsigned char sladr){
	unsigned int DADR  = (unsigned int) sladr & 0x000000FE;		// Slave Adresse ohne RW-Bit
	unsigned int MREAD = (unsigned int) sladr & 0x00000001;		// RW-Bit für 'Master read direction'

	AT91C_BASE_TWI->TWI_CR = AT91C_TWI_MSEN;					// TWI-Controller-Modul als Master freigeben
	while (!(AT91C_BASE_TWI->TWI_SR & AT91C_TWI_TXCOMP));		// Warten bis ev. Übertragung zu Ende
	AT91C_BASE_TWI->TWI_MMR = (DADR << 15) | (MREAD << 12);		// Master-Mode Register konfigurieren

	return 0;
}

//**********************************************************
// i2c_rbyte
// Liest ein Byte vom I2C Bus
// Übergabewert:	ACK = 0:	Es soll anschließend ein weiteres Byte gelesen werden
//					ACK = 1:	Im Anschluß Beendigung der Datenübertragung
// Rückgabewert:	Gelesenes Byte
//**********************************************************
unsigned char i2c_rbyte(unsigned char ACK){
	unsigned char byte;											// Variable für Rückgabewert
	static unsigned char s=0;									// Status-Bit für Start/Stop Condition

																// Entscheidung zur Sendung der Start/Stop-Bedingung
	if (!s && ACK){													// Insgesammt nur ein Byte lesen
		AT91C_BASE_TWI->TWI_CR=(AT91C_TWI_START|AT91C_TWI_STOP);		// Start+Stop gleichzeitig senden
	}
	if (!s && !ACK){												// Das erste einer Vielzahl von Byte lesen
		AT91C_BASE_TWI->TWI_CR=AT91C_TWI_START;							// Start-Bedingung senden
		s=1;															// Status-Bit setzen
	}
	if (s && ACK){													// Das letzte Byte einer Vielzahl lesen
		AT91C_BASE_TWI->TWI_CR=AT91C_TWI_STOP;							// Stop-Bedingung senden
		s=0;															// Status-Bit löschen
	}
	while (!(AT91C_BASE_TWI->TWI_SR & AT91C_TWI_RXRDY));		// Warten bis Byte übertragen
	byte = AT91C_BASE_TWI->TWI_RHR;								// Byte abholen
	if (ACK) while(!(AT91C_BASE_TWI->TWI_SR & AT91C_TWI_TXCOMP));	// Warten bis Übertragung zu Ende
	return byte;												// Gelesener Wert zurück
}

//**************************************************************
// i2c_wbyte
// Ein Byte auf den I2C Bus schreiben
// Übergabewert:	Byte, welches geschrieben werden soll
// Rückgabewert:	ACK
//**************************************************************
unsigned char i2c_wbyte(unsigned char byte){

	AT91C_BASE_TWI->TWI_THR = byte;									// Byte in Transmit-Hold-Register -> Starten der Übertragung
	while (!(AT91C_BASE_TWI->TWI_SR & AT91C_TWI_TXRDY));			// Warten bis Byte übertragen
	if (AT91C_BASE_TWI->TWI_SR & AT91C_TWI_NACK) return 1;			// ACK zurück
	else return 0;
}

//**********************************************************
// i2c_stop
// Beendet Datenübertragung und gibt den Bus frei
//**********************************************************
void i2c_stop(void){
	while (!(AT91C_BASE_TWI->TWI_SR & AT91C_TWI_TXCOMP));			// Warten bis Übertragung zu Ende
	AT91C_BASE_TWI->TWI_CR = AT91C_TWI_MSDIS;						// TWI-Controller-Modul als Master sperren
	return;
}
#endif
