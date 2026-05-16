#ifndef TIMER_H_
#define TIMER_H_

/*Prototype for 1 second time out initialization*/
void tim2_1hz_init(void);

/*Prototype for 1 second time out initialization in interrupt mode*/
void tim2_1hz_interrupt_init(void);

/*Mask value to check the status of update interrupt flag of TIMx_SR(Timer status register)*/
#define SR_UIF (1U<<0)

#endif /* TIMER_H_ */
