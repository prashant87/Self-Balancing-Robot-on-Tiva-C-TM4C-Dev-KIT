#include <UART0Utils/UART0utils.h>
#include "../include.h"
#define MAXLEN  32
uint8_t  Buffer[4000];
uint8_t dataAval;
uint8_t dataindex=0;
extern bool flag;

typedef struct{
    uint8_t  *const buffer;
    int head;
    int tail;
    const int maxlen;
} CirculBuf_t;
//#define CIRCULBUF(x,y)
//CIRCULBUF(myBuffer,64);
int CirculBufPUSH(CirculBuf_t *c, uint8_t data);
int CirculBufPOP(CirculBuf_t *c);
uint8_t databuf[MAXLEN];
CirculBuf_t myBuffer = {
                 .buffer  = databuf,
                 .head =0 ,
                 .tail =0,
                 .maxlen = MAXLEN
};
int CirculBufPUSH(CirculBuf_t *c, uint8_t data)
{

    int next = c->head +1;
    if (next >= c-> maxlen) next =0; //
    if(next == c->tail) return 1;
    c->buffer[c->head]= data;
    c->head = next;
    return 0;
}
int CirculBufPOP(CirculBuf_t *c)
{
    if (c->head == c->tail) return -1;
    int next = c -> tail +1;
    if (next >= c->maxlen) next = 0;
    UARTCharPutNonBlocking(UART0_BASE, c->buffer[c->tail]);
    c->tail = next;
    return 0;
}
extern void UART0_Init(void)
{
        SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
        GPIOPinConfigure(GPIO_PA0_U0RX);
        GPIOPinConfigure(GPIO_PA1_U0TX);
        GPIOPinTypeUART(GPIO_PORTA_BASE, (GPIO_PIN_0 | GPIO_PIN_1));
        UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
        IntMasterEnable(); //enable processor interrupts
        IntEnable(INT_UART0); //enable the UART interrupt
        UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT|UART_INT_TX); //only enable RX and TX interrupts
//
}
extern void UART0_IntHandler(void)
{
    uint32_t ui32Status;
    ui32Status = UARTIntStatus(UART0_BASE, true);
    UARTIntClear(UART0_BASE, ui32Status);
    if(ui32Status==UART_INT_RX)
    {
       flag =1;
    }
//    if(ui32Status==UART_INT_TX)
//    {
//        CirculBufPOP(&myBuffer);
//
//    }

}
extern void UART0_PutstrInt(char *str)
{

    while(*str != 0x00)
    {
        CirculBufPUSH(&myBuffer,*(str++));
    }

    UARTTxIntModeSet(UART0_BASE, UART_TXINT_MODE_EOT);
    UARTIntEnable(UART0_BASE, UART_INT_TX);
//    UARTCharPut(UART0_BASE, 'a');
    CirculBufPOP(&myBuffer );
}
extern void UART0_Put_Str(char *str)
{
    while(*str != 0x00)
    {
    UARTCharPut(UART0_BASE, (char)(*str++));
    }
}
extern void UART0_Putint(int val)
{
//    sprintf((char*)Buffer,"%d ",val);
//    Buffer[0]='5';

//    UART0_Put_Str(Buffer);
}
