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

/* Prototype of Function dma_callback */
static void dma_callback(void);

int main(void)
{
	char message[31] = "Hello from stm32 DMA transfer\n\r";

	/* RM: Enable Clock access to GPIOA */
	RCC->AHB1ENR |= GPIOAEN;

	/* RM: Set PA5 as output pin */
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &= ~(1U<<11);


	/* Initialize the USART Transmission */
	uart2_tx_init();

	/* Initialize the USART DMA Transmission */
	dma1_stream6_init((uint32_t)message, (uint32_t)&USART2->DR, 31);


	while(1)
	{

	}

	return 0;
}

static void dma_callback(void)
{
	/*RM: Turn ON LED*/
	GPIOA->ODR |= LED_PIN;
}

void DMA1_Stream6_IRQHandler(void)
{

	/*RM: Check if Transfer complete interrupt flag is set*/
	if(DMA1->HISR & HISR_TCIF6)
	{
		/*RM: Clear the Transfer complete interrupt flag*/
        DMA1->HIFCR |= HIFCR_CTCIF6;

		/*Do Something*/
        dma_callback();

	}
}

