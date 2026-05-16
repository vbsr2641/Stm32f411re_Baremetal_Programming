/* Include generic header file for the micro-controller then go this file add the name of the controller STM32F411xE to symbols of the properties of the project */
#include "stm32f4xx.h"

/* Include stdint header to use data types */
#include<stdint.h>

/* Include stdio header to use printf */
#include<stdio.h>

/* Inclusion of uart header file */
#include "uart.h"

/* Inclusion of adc header file */
#include "adc.h"

/* Variable to store the sensor value */
uint32_t sensor_value;

int main(void)
{

	/* Initialize the USART Transmission */
	uart2_tx_init();

	/* Initialize the ADC1 module */
	pa1_adc_init();

	while(1)
	{
		/* Start ADC conversion in single conversion mode */
		start_conversion();

		/* Read the converted value by ADC */
		sensor_value = adc_read();

		/* print the converted sensor value */
		printf("Sensor Value: %d\n\r", (int)sensor_value);
	}

	return 0;
}

/*General Test to check whether driver is working or not.
 *
 * 1. Test1: Take jumper wire and keep one end at A1 where PA1 is connected and other end at gnd(ground)
 *    as per hardware configuration layout and result is expected to be around 0.
 *
 * 2. Test2: Take jumper wire and keep one end at A1 and other end at 3.3V as per hardware configuration
 *    layout and result is expected to be around 4000.
 *
 * 3. Test3: If nothing is connected and then random value will be coming in variable sensor_value when it is
 *    added to live expression in debugging mode.
 *
 * */
