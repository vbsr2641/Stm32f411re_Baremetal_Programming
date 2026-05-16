/* Include generic header file for the micro-controller then go this file add the name of the controller STM32F411xE to symbols of the properties of the project */
#include "stm32f4xx.h"

/* Include stdint header to use data types */
#include<stdint.h>

/* Include stdio header to use printf */
#include<stdio.h>

/* Inclusion of uart header file */
#include "uart.h"

/*Inclusion of systick header file*/
#include "systick.h"

/*Inclusion of Timer header file*/
#include "Timer.h"

/*Inclusion of external interrupt header file*/
#include "exti.h"

/* RM: Macro to define Mask value to enable bit0 of AHB1ENR register */
#define GPIOAEN    (1U<<0)

/* RM: Macro to define Mask value to enable bit5 of ODR register */
#define PIN5       (1U<<5)

/* RM: Macro to define PIN5 as user friendly name */
#define LED_PIN    PIN5

/*Prototype of function exti_callback*/
static void exti_callback(void);

int main(void)
{
	/* RM: Clock enable register of AHB1 bus for portA(GPIOA) where LED is connected in block diagram */
	RCC->AHB1ENR |= GPIOAEN;

	/* bit10 and bit11 need to be configured to make GPIOA port pin 5 as general purpose output mode where LED is a output peripheral */
	/* RM: Direction(Mode) register bit10 for portA(GPIOA) peripheral is set */
	GPIOA->MODER |= (1U<<10);

	/* RM: Direction(Mode) register bit11 for portA(GPIOA) peripheral is reset */
	GPIOA->MODER &=~ (1U<<11);

	/* Initialize the EXTI Interrupt Port C pin 13 */
	pc13_exti_init();

	/* Initialize the USART Transmission */
	uart2_tx_init();

	while(1)
	{


	}

	return 0;
}

static void exti_callback(void)
{
	/*Print the statement when interrupt occurred*/
	printf("BTN pressed...\n\r");

	/*Toggle LED when interrupt occurred*/
	GPIOA->ODR ^= LED_PIN;
}

void EXTI15_10_IRQHandler(void)
{
	/*RM:Check if PR bit is set */
	if((EXTI->PR & LINE13) != 0)
	{
		/*Clear PR(Pending register) flag*/
		EXTI->PR |= LINE13;

		/*Invoke the exti_callback API to trigger transmission on every interrupt occured*/
		exti_callback();
	}

}

/*General Test to check whether driver is working or not.
 *
 *1. Use Realterm to check whether BTN pressed... sentence is printed through USART when push button is pressed.
 *2. Check whether LED is blinking when push button is pressed via external interrupt occurred.
 * */
