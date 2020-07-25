#include "../include.h"
#include "ADC.h"

 static void ADCisr(void);
extern configADC(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);
    ADCHardwareOversampleConfigure(ADC0_BASE,64);

    ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE,1, 0, ADC_CTL_CH0)   ;
    ADCSequenceStepConfigure(ADC0_BASE,1, 1, ADC_CTL_CH0)   ;
    ADCSequenceStepConfigure(ADC0_BASE,1, 2, ADC_CTL_CH0)   ;
    ADCSequenceStepConfigure(ADC0_BASE,1, 3 ,ADC_CTL_CH0|ADC_CTL_IE)   ;
    ADCSequenceEnable(ADC0_BASE,1);
    ADCIntRegister(ADC0_BASE, 1, &ADCisr);
    ADCIntEnable(ADC0_BASE, 1);
}

static void ADCisr(void){
    ADCIntClear(ADC0_BASE, 1);
    uint8_t data[4];
    ADCSequenceDataGet(ADC0_BASE, 1, (uint32_t *)&data);
    volatile  float volta;
    volta = (float)(((data[0]+data[1]+data[2]+data[3])/4)*5/4096);

}
