/* where the led is connected */
/* port: A */
/* pin:  5 */
/* (UL - stands for Unsigned Linked) */
/* (U - stands for Unsigned) */
/* We need GPIOAP5 to enable LED from GPIOA port */
/* DS : Data Sheet, UM : User Manual, RM : Reference Manual */
/* Here in macros for register definitions _R is added to register name for easy identification */


/* Why we need Bus?
 * Because each peripheral need access to clock. Busses carry clock signal
 * hence peripherals are connected to buses. In other words bus is a transporter of clock to various peripherals.
 */


/* AHB-Advance High Performance Bus
 * APB-Advance Performance Bus
 * AHB is faster than APB
 * RCC(Reset and Clock Control) - is the peripheral module which enables the bus to carry clock to peripherals in our case it enables GPIOA to access clock
 * Offset - Difference between two addresses
 * Address - 0x0000 0000 (higher nibble is base(start) address and lower nibble is offset from base address)
 * Base address - starting address of the peripheral
 * User Manual : getting started document contains information about peripherals like LED, Push buttons etc.
 * Data Sheet : chip related information like memory mapping, circuit of I/O etc
 * Reference Manual : Registers related information, complete functionality of peripherals present in board is explained.
 * Every Peripheral like GPIOA has 2 types of registers(Minimum registers need to be configured for GPIOs)
 * 1. Direction(Mode) register : Used to set i/p or o/p pin.
 * 2. Data Register : Used to store i/p data to be transmit or o/p data to be receive from peripheral of Micro-controller.
 * Direction register is having name as Mode Register from STM manufacturer
 */


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
/* RM: Difference between the RCC base address and the start address of register responsible for enable RCC AHB1 peripheral clock signal */
#define AHB1EN_R_OFFSET     (0X30UL)
/* RM: Start address of RCC AHB1 peripheral clock enable register */
#define RCC_AHB1EN_R        (*(volatile unsigned int*) (RCC_BASE + AHB1EN_R_OFFSET))
/* RM: Bit 0 is set to enable GPIOA by moving value 1 by 0 position as mentioned in reference manual*/
#define GPIOAEN    			(1UL << 0) /* value = 0b 0000 0000 0000 0000 0000 0000 0000 0001 */
/* RM: Difference between GPIOA base address and start address of Direction(Mode) register for GPIOA */
#define MODE_R_OFFSET       (0x00UL)
/* RM: Start address of the GPIOA Mode register */
#define GPIOA_MODE_R        (*(volatile unsigned int*) (GPIOA_BASE + MODE_R_OFFSET))
/* RM: Difference between start address of Output data register and base address of the GPIOA */
#define OD_R_OFFSET         (0x14UL)
/* RM: Start address of the Output data register */
#define GPIOA_OD_R          (*(volatile unsigned int*) (GPIOA_BASE + OD_R_OFFSET))
/* RM: Macro to set the pin 5 of GPIOA Mode and Output data register because Pin5 is connected to LED as mentioned in Reference manual */
#define PIN5                (1U<<5)
/* RM: User readable macro is created for PIN5 */
#define LED_PIN             PIN5

/**
 * (1U<<10) to set the 10th bit of GPIOA_MODE_R because we need to configure LED as output
 * In this register 2 bits are required to configure one pin that is PA5 as mentioned in user manual
 * 01: General purpose output mode--> 11th bit as 0 and 10th bit as 1
 * &=~(1U<<11) to reset 11th bit only
 *
 **/

/**
 * For E.g.
 * Initial value of RCC Register = 0b 0000 0000 0000 0000 0010 1111 0000 0000
 *
 * Need to set bit 0 only to enable clock access to GPIOA through bus AHB1 using RCC Module as mentioned below
 * RCC Register = 0b 0000 0000 0000 0000 0010 1111 0000 0001
 *
 * To set only required bit, in our case bit 0 and not to hamper other bits we need to use in RCC Register (|= - bitwise or operator)
 *
 * To reset only required bit, and not to hamper other bits we need to use in register (&=~ - bitwise and, ~ complement operator)
 **/

int main(void)
{
	/* 1. Enable clock access to GPIOA using RCC_AHB1EN register */
	RCC_AHB1EN_R |= GPIOAEN;
	/* 2. Set PA.5 as Output pin, PA.5 has 2 bits bit 10 and bit 11 */
	/* Set Pin5 bit10 as 1 for GPIOA Mode register */
	GPIOA_MODE_R |= (1U<<10);
	/* Reset Pin5 bit11 as 0 for GPIOA Mode register */
	GPIOA_MODE_R &= ~(1U<<11);

	while(1)
	{
		/* 3. Set PA.5 as high to switch on LED */
		GPIOA_OD_R |= LED_PIN;

		/* 4. Toggle PA.5 to switch on and off LED */
		//GPIOA_OD_R ^= LED_PIN;
		//for(int delay = 100000; delay > 0 ; delay--){}
	}

	return 0;
}


