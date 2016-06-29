//********************************************************************
// display.h									Version 02.04.2012 hpw
//********************************************************************

#ifndef DISPLAY_H_
#define DISPLAY_H_

#define LCD_PORT	AT91C_BASE_PIOA

#define	LCD_RS		AT91C_PIO_PA29
#define	LCD_RW		AT91C_PIO_PA27
#define LCD_E		AT91C_PIO_PA7

#define LCD_D0		AT91C_PIO_PA8
#define LCD_D1		AT91C_PIO_PA9
#define LCD_D2		AT91C_PIO_PA10
#define LCD_D3		AT91C_PIO_PA11
#define LCD_D4		AT91C_PIO_PA12
#define LCD_D5		AT91C_PIO_PA26
#define LCD_D6		AT91C_PIO_PA25
#define LCD_D7		AT91C_PIO_PA24

#define LCD_SBUS	(LCD_RS | LCD_RW | LCD_E)
#define LCD_DBUS	(LCD_D7 | LCD_D6 | LCD_D5 | LCD_D4 | LCD_D3 | LCD_D2 | LCD_D1 | LCD_D0)
#define LCD_BUS		(LCD_DBUS | LCD_SBUS)

extern void display_init(void);
extern void display_putchar(unsigned char c);
extern void display_putstr(char *p);
extern void display_set_cursor(unsigned char x, unsigned char y);
extern void display_printf(char *s, ...);

#endif /*DISPLAY_H_*/
