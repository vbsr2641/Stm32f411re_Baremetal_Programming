#include "stm32f4xx.h"
#include "Timer.h"

/*RM: Mask value to enable clock access to TIM2 via APB1 Bus and assign to APB1ENR register*/
#define TIM2EN (1U<<0)

/*RM: Mask value to enable timer 2 via control register 1 and assign to CR1 register*/
#define CR1_CEN (1U<<0)

/*RM: Mask value to enable the interrupt in timer and assign to DIER register of timer*/
#define DIER_UIE (1U<<0)


void tim2_1hz_init(void)
{
	/*Enable clock access to timer 2*/
	RCC->APB1ENR |= TIM2EN;

	/*Set prescaler value*/
	TIM2->PSC = 1600 - 1; //16 000 000 / 16 00 = 10 000

	/*Set auto-reload value*/
	TIM2->ARR = 10000 - 1; // 10 000 / 10 000 = 1

	/*Clear counter*/
	TIM2->CNT = 0;

	/*Enable timer 2*/
	TIM2->CR1 |= CR1_CEN;

}

void tim2_1hz_interrupt_init(void)
{
	/*Enable clock access to timer 2*/
	RCC->APB1ENR |= TIM2EN;

	/*Set prescaler value*/
	TIM2->PSC = 1600 - 1; //16 000 000 / 16 00 = 10 000

	/*Set auto-reload value*/
	TIM2->ARR = 10000 - 1; // 10 000 / 10 000 = 1

	/*Clear counter*/
	TIM2->CNT = 0;

	/*Enable timer 2*/
	TIM2->CR1 |= CR1_CEN;

	/*Enable Timer interrupt*/
	TIM2->DIER |= DIER_UIE;

	/*Enable Timer interrupt in NVIC*/
	NVIC_EnableIRQ(TIM2_IRQn);
}

/*Here Timer 2 is selected for timeout feature.
 *Timer 2 is connected to APB1 bus
 *1600 - 1 is assigned as pre-scaler value for 1 second time out
 *1 hz is nothing but 1 second
 *10000 is used as Auto reload value
 *Here subtraction of 1 is performed as count starts from 0 to 9999 is nothing but 10000 counts
 *Counter is cleared by resetting CNT register using 0
 *Counter of Timer1 is enabled using CR1(Control Register) register
 *Here the clock frequency of timer is 16MHz. we are reducing clock speed to 10KHz by using 1600 as pre-scaler.
 *Here as per the formula T = (Pre-scaler + 1).(Auto reload value + 1) / fclk is used.
 *16 MHz is the fclk, T is 1 second.
 *The product of (Pre-scaler + 1).(Auto reload value + 1) should give 16 MHz hence 1600 and 10000hz is choosen
 * */
