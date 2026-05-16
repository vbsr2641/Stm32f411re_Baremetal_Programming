#include <stdint.h>

/* DS: Base(start) address for all peripherals as mentioned in data sheet */
#define PERIPH_BASE			(0X40000000UL)
/* DS: Difference between base address of peripherals and base address of AHB1(0X40020000UL) bus gives the offset */
#define AHB1PERIPH_OFFSET	(0X00020000UL)
/* DS: Base address of AHB1 bus is the sum of base address of peripherals and offset of AHB1 bus */
#define AHB1PERIPH_BASE 	(PERIPH_BASE + AHB1PERIPH_OFFSET)
/* DS: Start address of AHB1 bus and GPIOA is same as mentioned in memory mapping of data sheet hence the offset of GPIOA from AHB1 bus is zero */
#define GPIOA_OFFSET        (0x0000U)
/* DS: Base address of GPIOA port from AHB1 bus */
#define GPIOA_BASE  		(AHB1PERIPH_BASE + GPIOA_OFFSET)
/* DS: Difference between RCC start address and AHB1 base address */
#define RCC_OFFSET			(0x3800U)
/* DS: Base address of RCC module peripheral */
#define RCC_BASE			(AHB1PERIPH_BASE + RCC_OFFSET)
/* RM: Bit 0 is set to enable GPIOA by moving value 1 by 0 position as mentioned in reference manual*/
#define GPIOAEN    			(1UL << 0) /* value = 0b 0000 0000 0000 0000 0000 0000 0000 0001 */
/* RM: Macro to set the pin 5 of GPIOA Mode and Output data register because Pin5 is connected to LED as mentioned in Reference manual */
#define PIN5                (1U<<5)
/* RM: User readable macro is created for PIN5 */
#define LED_PIN             PIN5
/* As per MISRA C standard this macro is created */
#define __IO                volatile

/* This law is applicable for both GPIO and RCC module
 * starting member address of structure is same as starting address of GPIO. The registers
 * should be in order based on the offset starts from 0x00 to 0x24 for registers
 * MODER to AFRH. The offset order should not change in the any peripheral. To maintain
 * the address offset of ODR register 16 bytes of array having 4 elements is created.
 * we can remove the register address below the required register
 * */

typedef struct
{
	volatile uint32_t DUMMY[12]; /* Dummy Array created to fulfill the vacant address of (12 elements x 4 bytes = 48 bytes) between CR and DCKCFGR registers */
	volatile uint32_t AHB1ENR;/* GPIO port mode register, address offset = 0x00 */
}RCC_TypeDef;

typedef struct
{
	volatile uint32_t MODER;   /* GPIO port mode register, address offset = 0x00 */
	volatile uint32_t DUMMY[4];/* Dummy Array created to fulfill the vacant address between MODER and ODR registers */
	volatile uint32_t ODR;     /* GPIO port output data register address offset = 0x14 */
}GPIO_TypeDef;

/* Defining the RCC as the address for RCC_BASE using RCC_TypeDef structure */
#define RCC      ((RCC_TypeDef*)RCC_BASE)
/* Defining the GPIOA as the address for RCC_BASE using GPIO_TypeDef structure */
#define GPIOA    ((GPIO_TypeDef*)GPIOA_BASE)

int main(void)
{
	/* 1. Enable clock access to GPIOA using RCC_AHB1EN register */
	RCC->AHB1ENR |= GPIOAEN;
	/* 2. Set PA.5 as Output pin, PA.5 has 2 bits bit 10 and bit 11 */
	/* Set Pin5 bit10 as 1 for GPIOA Mode register */
	GPIOA->MODER |= (1U<<10);
	/* Reset Pin5 bit11 as 0 for GPIOA Mode register */
	GPIOA->MODER &= ~(1U<<11);

	while(1)
	{
		/* 3. Set PA.5 as high to switch on LED */
		GPIOA->ODR |= LED_PIN;

		/* 4. Toggle PA.5 to switch on and off LED */
		//GPIOA->ODR ^= LED_PIN;
		//for(int delay = 100000; delay > 0 ; delay--){}
	}

	return 0;
}


