#ifndef gpio_h_
#define gpio_h_

#define LEDBLUE GPIO_PIN_2
#define LEDRED GPIO_PIN_1
#define LEDGREEN GPIO_PIN_3
#define PORTF GPIO_PORTF_BASE
#define SW1 GPIO_PIN_4

#define SW2 GPIO_PIN_0
extern void LED_Status(void);
extern void SW_Init(void);
extern void SW_Isr(void);
extern unsigned char SW_Status(void);
#endif
