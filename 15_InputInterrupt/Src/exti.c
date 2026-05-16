#include "exti.h"

/*RM:Mask value to enable clock access to GPIO port C and assign 1 value to 2nd bit*/
#define GPIOCEN	(1U<<2)

/*RM:Mask value to enable SYSCFG peripheral to use external interrupts and assign
 *value 1 to 14th bit for register APB2ENR
 */
#define SYSCFGEN (1U<<14)

void pc13_exti_init(void)
{
	/*BSP:Disable global interrupts*/
	__disable_irq();

	/*RM:Enable clock access to GPIOC*/
	RCC->AHB1ENR |= GPIOCEN;

	/*RM:Set PC13 as input pin*/
	GPIOC->MODER &= ~(1U<<26);
	GPIOC->MODER &= ~(1U<<27);

	/*RM:Enable clock access to SYSCFG*/
	RCC->APB2ENR |= SYSCFGEN;

	/*RM:Select PORTC for EXTI13*/
	SYSCFG->EXTICR[3] |= (1U<<5);

	/*RM:Unmask EXTI13*/
	EXTI->IMR |= (1U<<13);

	/*RM:Select falling edge trigger*/
	EXTI->FTSR |= (1U<<13);

	/*BSP:Enable EXTI13 line in NVIC*/
	NVIC_EnableIRQ(EXTI15_10_IRQn);

	/*BSP:Enable global interrupts*/
	__enable_irq();

}

/*1.BSP: Board support package(APIs, Macros given by the manufacturer)
 *2.Here in this file we are configuring external hardware interrupt by pressing push button with falling edge
 *3.These 2 APIs are invoked to enable and disable interrupts due to best practices __enable_irq() and __disable_irq()
 *4.Push button is connected to Port C 13 pin as mentioned in user manual and Port C is connected to
 *5.AHB1 Bus to enable clock access we are assigning GPIOCEN.
 *6.To set the GPIOC 13 as input direction/ mode MODER register are assigned with reset mode(0,0 - input(reset) as mentioned in reference manual)
 *7.SYSCFG is a peripheral which need to be configured for to use External interrupt and It is connected to
 *APB2 Bus and enabling clock access to SYSCFG module by assigning SYSCFGEN.
 *8.we need to select the Port C 13 pin for EXTI module so EXICR is in the form of array EXTI13 is present
 *in 4th element of array EXTICR and for bits (0010 shall be assigned to 4th element of array to
 *enable PortC 13 pin for EXTI line)
 *9.Interrupt shall be unmaskable hence IMR register is assigned with value 1 in 13th bit
 *10.FTSR register is configured for falling edge trigger action since we are using push button
 *and it is assigned with value 1 at 13th bit
 *11.As we know NVIC handles all the form of interrupts in Arm cortex microcontroller we need to
 *enable EXTI 13 line in NVIC using BSP API NVIC_EnableIRQ with Interrupt number EXTI15_10_IRQn for EXTI 13 line as mentioned in core_cm4.h file
 * */
