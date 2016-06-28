//********************************************************************
// i2c_tools.h								Version 24.06.2014_stud cs
//********************************************************************

#ifndef I2C_TOOLS_H_
#define I2C_TOOLS_H_

unsigned char i2c_start(unsigned char Adresse);
unsigned char i2c_wbyte(unsigned char Byte);
unsigned char i2c_rbyte(unsigned char ACK);
void i2c_stop(void);

#endif /*I2C_TOOLS_H_*/
