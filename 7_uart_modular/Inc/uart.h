#ifndef UART_H_
#define UART_H_

/* Include stdint header to use data types */
#include<stdint.h>

/* Include generic header file for the micro-controller then go this file add the name of the controller STM32F411xE to symbols of the properties of the project */
#include "stm32f4xx.h"

/* Function Prototype of uart2_tx_init API to initialize USART2 Module */
void uart2_tx_init(void);

/* Function Prototype of uart2_write API to transmit data */
void uart2_write(int ch);

#endif /* UART_H_ */
