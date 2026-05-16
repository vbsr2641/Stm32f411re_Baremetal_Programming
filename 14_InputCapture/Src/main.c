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

/*Variable to read captured timer value of Timer 2 from CCR1 register*/
int timestamp = 0;

int main(void)
{

	/* Configuration of the Timer 2 Module in output compare mode */
	tim2_pa5_output_compare();

	/* Configuration of the Timer 3 Module in input capture mode */
	tim3_pa6_input_capture();

	while(1)
	{
	   /*Wait until edge is captured*/
       while(!(TIM3->SR & SR_CC1IF)){}

       /*Read Value from Channel 1 CCR1 register*/
       timestamp = TIM3->CCR1;

	}

	return 0;
}

/*General Test to check whether driver is working or not.
 *
 * 1. Test1: LED is toggled every 1 second by using output compare feature of Timer 2 Module.
 *
 * 2. Test2: Connect PA5 and PA6 using jumper cable to capture the timer value from Timer 2 using Timer 3 to
 * check the toggle rate around 1000 will be the difference in the value every second in variable timestamp.
 *
 * */
