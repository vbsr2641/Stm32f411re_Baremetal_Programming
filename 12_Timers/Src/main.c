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

/* RM: Macro to define Mask value to enable bit0 of AHB1ENR register */
#define GPIOAEN    (1U<<0)

/* RM: Macro to define Mask value to enable bit5 of ODR register */
#define PIN5       (1U<<5)

/* RM: Macro to define PIN5 as user friendly name */
#define LED_PIN    PIN5

int main(void)
{

	/* RM: Clock enable register of AHB1 bus for portA(GPIOA) where LED is connected in block diagram */
	RCC->AHB1ENR |= GPIOAEN;

	/* bit10 and bit11 need to be configured to make GPIOA port pin 5 as general purpose output mode where LED is a output peripheral */
	/* RM: Direction(Mode) register bit10 for portA(GPIOA) peripheral is set */
	GPIOA->MODER |= (1U<<10);

	/* RM: Direction(Mode) register bit11 for portA(GPIOA) peripheral is reset */
	GPIOA->MODER &=~ (1U<<11);

	/* Initialize the USART Transmission */
	uart2_tx_init();

	/* Initialize the Timer 2 Module */
	tim2_1hz_init();

	while(1)
	{
		/*Loop to wait until UIF flag is set*/
		while(!(TIM2->SR & SR_UIF)){};

		/*Clear the UIF flag when after UIF is set*/
		TIM2->SR &= ~SR_UIF;

		/* print sentence after every second*/
		printf("A Second Passed !!!\n\r");

		/*LED is toggled every 1 second*/
		GPIOA->ODR ^= LED_PIN;

	}

	return 0;
}

/*General Test to check whether driver is working or not.
 *
 * 1. Test1: LED is toggled every 1 second by providing 1000 to function systickDelayMS.
 *
 * 2. Test2: "A Second Passed!!!" sentence every one second through UART
 *
 * */
