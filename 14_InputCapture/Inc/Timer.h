#ifndef TIMER_H_
#define TIMER_H_

/*Prototype for 1 second time out configuration*/
void tim2_1hz_init(void);

/*Prototype for configuration of output compare feature of Timer 2*/
void tim2_pa5_output_compare(void);

/*Prototype for configuration of input capture feature of Timer 3*/
void tim3_pa6_input_capture(void);

/*Mask value to check the status of update interrupt flag of TIMx_SR(Timer status register)*/
#define SR_UIF (1U<<0)

/*Mask value to check the status of Timer */
#define SR_CC1IF (1U<<0)

#endif /* TIMER_H_ */
