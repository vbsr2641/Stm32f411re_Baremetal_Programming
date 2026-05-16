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

int main(void)
{

	/* Configuration of the Timer 2 Module in output compare mode */
	tim2_pa5_output_compare();

	while(1)
	{


	}

	return 0;
}

/*General Test to check whether driver is working or not.
 *
 * 1. Test1: LED is toggled every 1 second by using output compare feature of Timer 2 Module.
 *
 * */
