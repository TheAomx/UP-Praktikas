#ifndef serial_h_
#define serial_h_

void usart0_init (void);
int usart0_putc(int ch);
int usart0_putchar (int ch);

void usart0_puts(char *s);

int usart0_kbhit( void );
int usart0_getc ( void );

#endif
