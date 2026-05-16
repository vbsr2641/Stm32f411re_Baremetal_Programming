/* Include generic header file for the micro-controller then go this file add the name of the controller STM32F411xE to symbols of the properties of the project */
#include "stm32f4xx.h"

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

	while(1)
	{
		/* RM: Output data register of GPIOA is XOR with LED_PIN to toggle LED */
		GPIOA->ODR ^= LED_PIN;
		/* Delay to see the toggle of LED */
		for(int i=0; i<100000; i++){}
	}
}
