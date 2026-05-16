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
volatile char key;

/* Prototype of Function uart_callback */
static void uart_callback(void);

int main(void)
{

	/* RM: Enable Clock access to GPIOA */
	RCC->AHB1ENR |= GPIOAEN;

	/* RM: Set PA5 as output pin */
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &= ~(1U<<11);

	/* Initialize the USART Reception */
	uart2_rx_interrupt_init();

	/* Initialize the USART Transmission */
	//uart2_tx_interrupt_init();

	while(1)
	{

	}

	return 0;
}

static void uart_callback(void)
{
	/*Read USART data register to key*/
	key = USART2->DR;

	/*if key is 1 then turn on LED*/
	if(key == '1')
	{
		GPIOA->ODR |= LED_PIN;
	}
	else
	{
		GPIOA->ODR &= ~LED_PIN;
	}

}

void USART2_IRQHandler(void)
{
	/*RM: Check if RXNE is set */
	if(USART2->SR & SR_RXNE)
	{
		/*LED blink when interrupt occurs*/
		uart_callback();
	}

}
