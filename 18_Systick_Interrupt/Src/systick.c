#include "stm32f4xx.h"

/*Reload value to assign to register SYST_RVR*/
#define SYSTICK_LOAD_VAL		16000

/*GUG: Mask value to enable Systick timer in SYST_CSR register*/
#define CTRL_ENABLE				(1U<<0)

/*GUG: Mask value to set the clock source in SYST_CSR register*/
#define CTRL_CLKSRC             (1U<<2)

/*GUG: Mask value to set the count flag in SYST_CSR register*/
#define CTRL_COUNTFLAG          (1U<<16)

/*RM: 16 million scycles per second that is 16MHz*/
#define ONE_SEC_LOAD            16000000U

/*GUG: Mask value to enable systick timer interrupt bit */
#define CTRL_TICKINT            (1U<<1)

void systickDelayMS(int delay)
{
	/*Reload with number of clocks per milliseconds*/
	SysTick->LOAD = SYSTICK_LOAD_VAL;

	/*Clear Systick current value register*/
	SysTick->VAL = 0;

	/*Enable Systick and select internal clock source*/
	SysTick->CTRL = CTRL_ENABLE | CTRL_CLKSRC;

	for(int i = 0; i < delay; i++)
	{
		/*Wait for Count Flag to set*/
		while((SysTick->CTRL & CTRL_COUNTFLAG) == 0){};
	}

	/*Stop the Systick timer*/
	SysTick->CTRL = 0;

}

/*Function for 1 second systick dealy in interrupt mode*/
void systick_1hz_interrupt(void)
{
	/*GUG: Reload with number of clocks per second*/
	SysTick->LOAD = ONE_SEC_LOAD - 1;

	/*GUG: Clear Systick current value register*/
	SysTick->VAL = 0;

	/*GUG: Enable Systick and select internal clock source*/
	SysTick->CTRL |= CTRL_ENABLE | CTRL_CLKSRC;

	/*GUG: Enable systick timer in interrupt mode*/
	SysTick->CTRL |= CTRL_TICKINT;

}

/* Systick registers information is present in cortex M devices generic user guide not in usermanual,
 * datasheet, reference manual because systick is core peripheral it will be there in all the microcontroller
 * irrespective of manufacture of board hence information is present in generic userguide of Arm.
 *
 * SYSTICK_LOAD_VAL is defined as 16000 by doing below calculation.
 * For 1 ms delay how much reload value is required is calculated using below formula
 * Delay = N/SYSCLK
 * N = Delay * SYSCLK
 * where,
 * Delay = time required to achieve delay
 * N = Reload value
 * SYSCLK = System Clock frequency
 *
 * N = 0.001s * 16000000Hz
 * N = 16000
 *
 * GUG: Generic User Guide
 *
 * Here the control executes for loop 1000 times, at each iteration the while loop waits for counter
 * to count from 16000 to 0 then count flag sets to 1 and 1ms delay is achieved.
 * if 1000 times for loop executes time taken by controller is 1 second in this way
 * precision delay is achieved. LED will blink every 1 second and in port 0 of UART in realterm tool
 * sentence is printed every one second.
 *
 * */
