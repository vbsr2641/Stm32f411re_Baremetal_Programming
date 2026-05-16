#ifndef UART_H_
#define UART_H_

/* Include stdint header to use data types */
#include<stdint.h>

/* Include generic header file for the micro-controller then go this file add the name of the controller STM32F411xE to symbols of the properties of the project */
#include "stm32f4xx.h"

/* RM: Macro to define Mask value to enable bit5 of USART_SR register to check reception status */
#define SR_RXNE         (1U<<5)

/* Function Prototype of uart2_tx_init API to initialize USART2 Module */
void uart2_tx_init(void);

/* Function Prototype of uart2_rxtx_init API to initialize USART2 Module */
void uart2_rxtx_init(void);

/* Function Prototype of uart2_read API to receive data */
char uart2_read(void);

/* Function Prototype of uart2_write API to transmit data */
void uart2_write(int ch);

/* Function prototype of uart2_tx_interrupt_init */
void uart2_tx_interrupt_init(void);

/* Function prototype of uart2_rx_interrupt_init */
void uart2_rx_interrupt_init(void);

#endif /* UART_H_ */
