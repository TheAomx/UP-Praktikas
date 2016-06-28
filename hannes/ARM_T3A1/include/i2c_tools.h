//********************************************************************
// i2c_tools.c								Version 15.05.2014_std hpw
//********************************************************************

#ifndef I2C_TOOLS_H_
#define I2C_TOOLS_H_

unsigned char i2c_start(unsigned char Adresse);
unsigned char i2c_wbyte(unsigned char Byte);
unsigned char i2c_rbyte(unsigned char ACK);
void i2c_stop(void);

#endif /*I2C_TOOLS_H_*/
