#include "uart.h"

/* RM: Macro to define Mask value to enable bit0 of AHB1ENR register */
#define GPIOAEN    (1U<<0)

/* RM: Macro to define Mask value to enable bit17 of APB1ENR register */
#define UART2EN    (1U<<17)

/* Default System frequency is 16MHZ i.e. 16 million cycles per second */
#define SYS_FREQ	16000000

/* Clock of APB1 Bus is same as System frequency as the device clock tree for APB1 is 1 */
#define APB1_CLK	SYS_FREQ

/* Baud rate of USART Module */
#define UART_BAUDRATE	115200

/* RM: Enable the Transmitter (TE bit) from USART_CR1 USART control register */
#define CR1_TE          (1U<<3)

/* RM: Enable the Receiver (RE bit) from USART_CR1 USART control register */
#define CR1_RE          (1U<<2)

/* RM: Enable the USART Enable (UE bit) from USART_CR1 USART control register */
#define CR1_UE          (1U<<13)

/* RM: Macro to define Mask value to enable bit7 of USART_SR register to check Transmission status */
#define SR_TXE          (1U<<7)

/* RM: Macro to define Mask value to enable bit5 of USART_SR register to check reception status */
#define SR_RXNE         (1U<<5)

/* Function Prototype of uart_set_baudrate API to set the baud rate for generic(any)
 * USART Module(our case USART2 Module)
 * */
static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);

/* Function Prototype of compute_uart_bd API to compute the Baud rate value for USART2 Module */
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);

/* Function to put sentence in the form of character by character */
int __io_putchar(int ch)
{
	uart2_write(ch);
	return ch;
}

void uart2_rxtx_init(void)
{
	/*******configure USART GPIO pin********/
	/*RM: Enable clock access to GPIO pin*/
	RCC->AHB1ENR |= GPIOAEN;
	/*RM: Set the PA2 mode to alternate function mode(1,0)*/
	/*RM: configure pin 2 in MODER Register of GPIOA by resetting bit 4 to 0 and bit 5 to 1*/
	GPIOA->MODER &=~ (1U<<4);
	GPIOA->MODER |=  (1U<<5);

	/*RM: Set the PA2 alternate function type to USART_TX (AF07)
	 *AFR[0] is for AFRL and AFR[1] is for AFRH but we have array AFR
	 *AFR[0] is need to be set with 0110 as per truth table 0111: AF7 in GPIOx_AFRL register
	 **/
	/*RM: Set the bit 8 of the pin 2 of AFRL*/
	GPIOA->AFR[0]  |= (1U<<8);
	/*RM: Set the bit 9 of the pin 2 of AFRL*/
	GPIOA->AFR[0]  |= (1U<<9);
	/*RM: Set the bit 10 of the pin 2 of AFRL*/
	GPIOA->AFR[0]  |= (1U<<10);
	/*RM: Reset the bit 11 of the pin 2 of AFRL*/
	GPIOA->AFR[0]  &= ~(1U<<11);

	/*RM: Set the PA3 mode to alternate function mode(1,0)*/
	/*RM: configure pin 3 in MODER Register of GPIOA by resetting bit 6 to 0 and bit 7 to 1*/
	GPIOA->MODER &=~(1U<<6);
	GPIOA->MODER |=  (1U<<7);

	/*RM: Set the PA3 alternate function type to USART_TX (AF07)
	 *AFR[0] is for AFRL and AFR[1] is for AFRH but we have array AFR
	 *AFR[0] is need to be set with 0110 as per truth table 0111: AF7 in GPIOx_AFRL register
	 **/
	/*RM: Set the bit 12 of the pin 3 of AFRL*/
	GPIOA->AFR[0]  |= (1U<<12);
	/*RM: Set the bit 13 of the pin 3 of AFRL*/
	GPIOA->AFR[0]  |= (1U<<13);
	/*RM: Set the bit 14 of the pin 3 of AFRL*/
	GPIOA->AFR[0]  |= (1U<<14);
	/*RM: Reset the bit 15 of the pin 3 of AFRL*/
	GPIOA->AFR[0]  &= ~(1U<<15);

	/*******configure USART module********/
	/* RM: Enable clock access to usart2 module */
	RCC->APB1ENR |= UART2EN;
	/* Configure baud rate */
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);
	/* RM: Configure the transfer direction and set all other bit to default by not using | operator */
	USART2->CR1 = (CR1_TE | CR1_RE);
	/* RM: Enable usart2 module ( use | operator to retain CR1_TE configured in USART2->CR1 register) */
	USART2->CR1 |= CR1_UE;

}

void uart2_tx_init(void)
{
	/*******configure USART GPIO pin********/
	/*RM: Enable clock access to GPIO pin*/
	RCC->AHB1ENR |= GPIOAEN;
	/*RM: Set the PA2 mode to alternate function mode(1,0)*/
	/*RM: configure pin 2 in MODER Register of GPIOA by resetting bit 4 to 0 and bit 5 to 1*/
	GPIOA->MODER &=~ (1U<<4);
	GPIOA->MODER |=  (1U<<5);

	/*RM: Set the PA2 alternate function type to USART_TX (AF07)
	 *AFR[0] is for AFRL and AFR[1] is for AFRH but we have array AFR
	 *AFR[0] is need to be set with 0110 as per truth table 0111: AF7 in GPIOx_AFRL register
	 **/
	/*RM: Set the bit 8 of the pin 2 of AFRL*/
	GPIOA->AFR[0]  |= (1U<<8);
	/*RM: Set the bit 9 of the pin 2 of AFRL*/
	GPIOA->AFR[0]  |= (1U<<9);
	/*RM: Set the bit 10 of the pin 2 of AFRL*/
	GPIOA->AFR[0]  |= (1U<<10);
	/*RM: Reset the bit 11 of the pin 2 of AFRL*/
	GPIOA->AFR[0]  &= ~(1U<<11);

	/*******configure USART module********/
	/* RM: Enable clock access to usart2 module */
	RCC->APB1ENR |= UART2EN;
	/* Configure baud rate */
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);
	/* RM: Configure the transfer direction and set all other bit to default by not using | operator */
	USART2->CR1 = CR1_TE;
	/* RM: Enable usart2 module ( use | operator to retain CR1_TE configured in USART2->CR1 register) */
	USART2->CR1 |= CR1_UE;

}

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate)
{
	/* RM: Store the computed divisor to Baud rate register of USART Module */
	USARTx->BRR = compute_uart_bd(PeriphClk, BaudRate);
}

static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate)
{
	/* Equation to calculate the divisor for the Baud Rate Register (BRR) when using 16x over sampling */
	return ((PeriphClk+(BaudRate/2))/BaudRate);
}

char uart2_read(void)
{
	/* RM: Make sure the receive data register not empty */
	while(!(USART2->SR & SR_RXNE)){}

	/* RM: Return Read data */
	return USART2->DR;
}

void uart2_write(int ch)
{
	/* RM: Read the status register of USART2 and wait till the register value is set by hardware */
	while(!(USART2->SR & SR_TXE)){}

	/* RM: Write the data to data register of USART2 */
	USART2->DR = (ch & 0xFF);
}

/* APB1 Bus supplies clock access to USART2 Module.
 * 17th bit of RCC_APB1ENR register enables clock access to USART2 Module.
 * Why USART2 is selected means it has connection of USB which can communication with my PC otherwise
 * additional board and USART to USB converter is required.
 * we need GPIO pins as Tx and Rx lines of UART is GPIO Pins.
 * Search for alternate functions in data sheet to check to which alternate function
 * the USART Tx and Rx is connected.
 * In our case USART2 Tx is mapped to PA2 under AF07 alternate function
 * USART2 Rx is mapped to PA3 under AF07 in the Alternate function mapping table.
 * Search for Alternate function registers AFRL and AFRH.
 * In our case we are dealing with Pin 2 it is mapped to AFRL2.
 * one AFRL(Alternate function low) is for one pin e.g. AFRL0 is for pin 0
 * one AFRH(Alternate function high) is for one pin e.g. AFRH8 is for pin 8
 * pin0 to pin7 is mapped to AFRL0 to AFRL7 and pin8 to pin15 is mapped for AFRH8 to AFRH15
 * To configure baud rate for Transmission we write 2 functions uart_set_baudrate() and compute_uart_bd().
 * The default system peripheral clock is 16MHZ
 * compute_uart_bd() API Takes system frequency 16MHz and popular baud rate 115200 as input and gives the rate
 * at which USART need to communicate.
 * uart_set_baudrate() API is general API used for all USART Module and store the computed value by
 * compute_uart_bd() API to baud rate register USARTx->BRR of USART Module.
 * invoke uart_set_baudrate() inside uart2_tx_init to calculate Baud rate by taking inputs
 * USART2(USART2 Module name), APB1_CLK(system frequency), UART_BAUDRATE(Popular baud rate 115200)
 *  */

/*The below function can be used to calculate decimal value required to set 16 bits of BRR register
 *as per reference manual instead of using uart_set_baudrate and compute_uart_bd used above.
 *But in the function compute_uart_bd some formula is used which is not
 *in reference manual.
 *
 *
1. I now understand WHAT the USART_BRR is, what it's for, and how/why to program it. It took TONS of reading on different sites and numerous passes thru the debugger until it all FINALLY became clear!!!

2. I now FULLY understand that the formula Israel used in the compute_uart_bd(..) function IS NOT BASED, AT ALL, ON THE formula presented in the Reference Manual on PG:518 Section 19.3.4. However he did derive/create that formula -->what it is doing is calculating the decimal number required to set ALL 16 bits of the USART_BRR. But it's a "MAGIC" formula since it is not actually doing the calculation(s) required to derive the USART:Mantissa and the USART:Fraction which are the 2 values programmed into the USART_BRR register.

2a. Isreal's formula, of course, works...but we (the class audience) have NO IDEA how he derived that formula in order to come up with a value that sets ALL 16 BITS [15:0] of the USART_BRR ALL AT ONCE!!!

3. The following is a replacement to the uart_set_baudrate() function that Isreal presented in class. It does away with the call to the "MAGIC" function compute_uart_bd(..)

3a. It is fully commented so ANYBODY can understand how/why the USART_BRR is being programmed as it is.

3b. I'm sure the steps presented can be consolidated into a more efficient function but I programmed it the way I did so it's a "show your work" kind of function --- allowing anybody reading it to both understand the algorithm presented from start to finish...with all intermediate steps.

3c. For clarity all actual code lines are in bold and all comments are non-bold but start with //



=============================================================================

// functions to calculate and then set the USART_BRR (baud rate register)
// RefMan Pg:547 Sec:19.6.3 USART_BRR (Baud Rate Register)

static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate)
{

double mantissa;
double fraction;

uint16_t USARTDIV_MANT;
uint16_t USARTDIV_FRAC;



// RefMan Pg:518 Sec:19.3.4 (Fractional Baud rate generation)
// Using a value of OVER8 = 0 the formula posed in Equation #1 reduces to:
// USARTDIV = Fck / 16 * baud
// This reduced formula results in the following (for our project):
// USARTDIV = 16000000 / 16 * 115200 (16Mhz and baudrate of 115200)
// USARTDIV = 8.6806
// This makes the mantissa (whole portion) portion of USARTDIV = 8
// This makes the fractional portion of USARTDIV .6806
// Turning .6806 into a 16 bit number: .6806 * 16 = 10.8896 which
// rounded to the next whole number is = 11
// The mantissa binary value (8) is: 0000 0000 1000 (bits 11:0 of the USART_BRR)
// The fractional binary value (11) is: 1011 (bits 3:0 of the USART_BRR
// Bottom Line: We need to set the USART_BRR to:
// 0b 0000 0000 0000 0000 0000 0000 1000 1011 or 0x8b
// See: RefMan Pg 547 Sec:19.6.3 (Baud Rate Register USART_BRR)
// Doing this programatically as follows:

// Step #1: Calculate the mantissa using the above formula and store in a temp double
mantissa = (double)periph_clk / (double)(baudrate * 16);

// Step #2: Calculate the fraction using the above formula and store in a temp double|
fraction = ((mantissa - ((long)mantissa))) * 16;

// Step #3: Round the fraction double up if fractional portion of fraction is > .50
if (fraction - ((long)fraction) >= .5)
fraction++;

// Step #4: store the mantissa into a uint16_t value (USARTDIV_MANT)
// which stores only the whole portion of the mantissa double
USARTDIV_MANT = mantissa;

// Step #5: store the fraction into a unit16_t value (USARTDIV_FRACT
// which stores only the whole portion of the fraction double
USARTDIV_FRAC = fraction;

// Step #6: set the 1st 4 bits [3:0] of the USART_BRR register to USARTDIV's fraction
USART2->BRR = (USARTDIV_FRAC << 0);

// Step #7: set the next 12 bits [11:0] of the USART_BRR to USARTDIV's mantissa
USART2->BRR |= (USARTDIV_MANT << 4);

}
============================================================================

I have fully tested the above function in the 1-uart_driver project in the class and it works perfectly...setting the USART2 peripheral up for communications with a serial terminal!!!

WRAPUP:

I would highly recommend that the above be added to EVERY SINGLY CLASS where Israel uses his two functions: uart_set_baudrate(..) which calls the mysterious compute_uart_bd(..) function. Since he NEVER, EVER, explains what the USART_BRR does or how/why it's being programmed.....the above function, with the detailed comments, will provide clarity to all the students that are "mystified" by the formula presented in compute_uart_bd(..).

I would still like to know/understand how Israel came up with the formula in compute_uart_bd(..) so that it derives the full 16 bit number, in one pass, required to set the USART_BRR. It's pretty cool how it works but IT IS NOT DERIVED FROM THE FORMULA IN THE REFMANUAL!!!
 * */
