#ifndef UART_H_
#define UART_H_

extern void UART0_Put_Str(char *Str);
extern void UART0_Init(void);

extern void UART0_Putint(int val);
extern void UART0_PutstrInt(char *str);
#endif
