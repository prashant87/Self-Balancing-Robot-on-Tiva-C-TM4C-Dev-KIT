#ifndef SYSTICK_SYSTICK_H_
#define SYSTICK_SYSTICK_H_

extern void SystickInit(void);
void Systick_ISR(void);
bool SystickFlagReturn(void);
void SystickFlagSet(bool Flag);

#endif /* SYSTICK_SYSTICK_H_ */
