/*
 * display.h
 *
 *  Created on: 18.04.2016
 *      Author: Aomx
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

void delay1us(unsigned int uiK);
void display_init(void);
void display_set_cursor(unsigned char x, unsigned char y);
void display_printf(char *string,...);
void display_putchar(unsigned char data);
void display_putstr(char *p);
void display_clear();

#endif /* DISPLAY_H_ */
