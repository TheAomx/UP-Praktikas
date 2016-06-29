//***********************************************************************
// twi_tools.h                                     Version hpw 06.05.2014
//***********************************************************************

#ifndef TWI_TOOLS_H_
#define TWI_TOOLS_H_

void twi_init(void);
unsigned char i2c_start(unsigned char Adresse);
unsigned char i2c_rbyte(unsigned char ACK);
unsigned char i2c_wbyte(unsigned char byte);
void i2c_stop(void);

#endif /* TWI_TOOLS_H_ */
