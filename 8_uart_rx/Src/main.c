/* Include generic header file for the micro-controller then go this file add the name of the controller STM32F411xE to symbols of the properties of the project */
#include "stm32f4xx.h"

/* Include stdint header to use data types */
#include<stdint.h>

/* Include stdio header to use printf */
#include<stdio.h>

#include "uart.h"

/* RM: Mask value to enable the bit 0 of AHBENR register */
#define GPIOAEN 	(1U<<0)

/* RM: Mask to value to enable bit 5 of register Output data register of GPIO */
#define GPIOA_5 	(1U<<5)

/* Use user readable name for GPIOA_5 */
#define LED_PIN 	GPIOA_5

/* Variable to store read data */
char key;

int main(void)
{

	/* RM: Enable Clock access to GPIOA */
	RCC->AHB1ENR |= GPIOAEN;

	/* RM: Set PA5 as output pin */
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &= ~(1U<<11);

	/* Initialize the USART Transmission */
	uart2_rxtx_init();

	while(1)
	{
		key = uart2_read();

		if(key == '1')
		{
			GPIOA->ODR |= LED_PIN;
		}
		else
		{
			GPIOA->ODR &= ~LED_PIN;
		}

	}

	return 0;
}

