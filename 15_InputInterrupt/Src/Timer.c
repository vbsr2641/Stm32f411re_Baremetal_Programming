/*Inclusion of header for micro-controller*/
#include "stm32f4xx.h"

/*Inclusion of header for timer module*/
#include "Timer.h"

/*RM: Mask value to enable clock access to TIM2 via APB1 Bus and assign to APB1ENR register*/
#define TIM2EN (1U<<0)

/*RM: Mask value to enable timer 2/3 via control register 1 and assign to CR1 register*/
#define CR1_CEN (1U<<0)

/*RM: Mask value to set the TIM2 in out put toggle compare mode using 6, 5, 4 bits (0, 1, 1) and assign to CCMR1 register */
#define OC_TOGGLE ((1U<<5)|(1U<<4))

/*RM: Mask value to enable TIM2 Channel 1 in out put compare mode using bit 0 and assign to CCER register */
#define CCER_CC1E (1U<<0)

/*RM: Mask value to enable GPIOA 5 pin and assign to register AHB1ENR*/
#define GPIOAEN (1U<<0)

/*RM: Mask value to set GPIOA AFRL5 to bit 23 = 0, 22 = 0, 21 = 0, 20 = 1 (AF01 = 0001) to enable TIM2 CH1*/
#define AFR5_TIM (1U<<20)

/*RM: Mask value to set GPIOA AFRL6 to bit 27 = 0, 26 = 0, 25 = 1, 24 = 0 (AF02 = 0010) to enable TIM3 CH1*/
#define AFR6_TIM (1U<<25)

/*RM: Mask value to enable clock access to TIM3 via APB1 Bus and assign to APB1ENR register*/
#define TIM3EN (1U<<1)

/*RM: Mask value to enable TIM3 Channel 1 in input capture mode using bit 0 and assign to CCER register */
#define CCER_CC1S (1U<<0)

void tim2_1hz_init(void)
{
	/*Enable clock access to timer 2*/
	RCC->APB1ENR |= TIM2EN;

	/*Set pre-scaler value*/
	TIM2->PSC = 1600 - 1; //16 000 000 / 16 00 = 10 000

	/*Set auto-reload value*/
	TIM2->ARR = 10000 - 1; // 10 000 / 10 000 = 1

	/*Clear counter*/
	TIM2->CNT = 0;

	/*Enable timer 2*/
	TIM2->CR1 |= CR1_CEN;

}

void tim2_pa5_output_compare(void)
{
	/*Enable clock access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/*Set PA5 mode to alternate function*/
    GPIOA->MODER &= ~(1U<<10);
    GPIOA->MODER |= (1U<<11);

	/*Set PA5 alternate function type to TIM2 CH1(Timer 2 channel 1)(AF01)*/
    GPIOA->AFR[0] |= AFR5_TIM;

	/*Enable clock access to timer 2*/
	RCC->APB1ENR |= TIM2EN;

	/*Set pre-scaler value*/
	TIM2->PSC = 1600 - 1; //16 000 000 / 16 00 = 10 000

	/*Set auto-reload value*/
	TIM2->ARR = 10000 - 1; // 10 000 / 10 000 = 1

    /*Set output compare toggle mode*/
	TIM2->CCMR1 = OC_TOGGLE;

	/*Enable TIM2 CH1 in compare mode*/
	TIM2->CCER |= CCER_CC1E;

	/*Clear counter*/
	TIM2->CNT = 0;

	/*Enable timer 2*/
	TIM2->CR1 |= CR1_CEN;

}

void tim3_pa6_input_capture(void)
{

	/*Enable clock access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/*Set PA6 mode to alternate function*/
	GPIOA->MODER &= ~(1 << 12);
	GPIOA->MODER |= (1U << 13);

	/*Set PA6 alternate function type to TIM3 CH1(Timer 3 channel 1)(AF02)*/
	GPIOA->AFR[0] |= AFR6_TIM;

	/*Enable clock access to Timer 3*/
	RCC->APB1ENR |= TIM3EN;

	/*Set pre-scaler value*/
	TIM3->PSC = 16000 - 1;

    /*Set Timer 3 CH1 to input capture mode*/
	TIM3->CCMR1 = CCER_CC1S;

	/*Set Timer 3 CH1 to capture every rising edge by default*/
    TIM3->CCER = CCER_CC1E;

	/*Enable Timer 3*/
    TIM3->CR1 = CR1_CEN;
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
