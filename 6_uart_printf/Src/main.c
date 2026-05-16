/* Include generic header file for the micro-controller then go this file add the name of the controller STM32F411xE to symbols of the properties of the project */
#include "stm32f4xx.h"

/* Include stdint header to use data types */
#include<stdint.h>

/* Include stdio header to use printf */
#include<stdio.h>

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

/* RM: Enable the USART Enable (UE bit) from USART_CR1 USART control register */
#define CR1_UE          (1U<<13)

/* RM: RM: Macro to define Mask value to enable bit7 of USART_SR register */
#define SR_TXE          (1U<<7)

/* Function Prototype of uart2_tx_init API to initialize USART2 Module */
void uart2_tx_init(void);

/* Function Prototype of uart_set_baudrate API to set the baud rate for generic(any)
 * USART Module(our case USART2 Module)
 * */
static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);

/* Function Prototype of compute_uart_bd API to compute the Baud rate value for USART2 Module */
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);

/* Function Prototype of uart2_write API to transmit data */
void uart2_write(int ch);

/* Function to put sentence in the form of character by character */
int __io_putchar(int ch)
{
	uart2_write(ch);
	return ch;
}

int main(void)
{
	/* Initialize the USART Transmission */
	uart2_tx_init();

	while(1)
	{
		/* Transmit data in the form of sentence */
		printf("Hello from STM32F411...\n\r");
	}
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
	/*RM: Store the computed Baud rate to Baud rate register of USART Module*/
	USARTx->BRR = compute_uart_bd(PeriphClk, BaudRate);
}

static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate)
{
	/*Equation to calculate baud rate for UART*/
	return ((PeriphClk+(BaudRate/2))/BaudRate);
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
