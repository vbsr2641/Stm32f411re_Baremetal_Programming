/* Include generic header file for the micro-controller then go this file add the name of the controller STM32F411xE to symbols of the properties of the project */
#include "stm32f4xx.h"

/* Include stdint header to use data types */
#include<stdint.h>

/* Include stdio header to use printf */
#include<stdio.h>

#include "uart.h"

int main(void)
{
	/* Initialize the USART Transmission */
	uart2_tx_init();

	while(1)
	{
		/* Transmit data in the form of sentence */
		printf("Hello from STM32F411...\n\r");
	}

	return 0;
}

