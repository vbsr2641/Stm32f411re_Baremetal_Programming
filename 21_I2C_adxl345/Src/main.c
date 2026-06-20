/* Include stdint header to use data types */
#include<stdint.h>

/* Include stdio header to use printf */
#include<stdio.h>

/* Include generic header file for the micro-controller then go this file add the name of the controller STM32F411xE to symbols of the properties of the project */
#include "stm32f4xx.h"

#include "uart.h"

#include "adxl345.h"

int16_t x;
int16_t y;
int16_t z;

float xg;
float yg;
float zg;

/*Buffer to store the x,y,z values from data register address 0x32 to 0x37 in the register map*/
extern uint8_t data_rec[6];

int main(void)
{

	adxl345_init();


	while(1)
	{
	    adxl_read_values(DATA_START_ADDR);

	    x = ((data_rec[1] << 8) | data_rec[0]);
	    y = ((data_rec[3] << 8) | data_rec[2]);
	    z = ((data_rec[5] << 8) | data_rec[4]);

	    /*DS: Constant value(7.8mg/LSB) to be multiplied to get full resolution value for +-4g in page 3 */
	    xg = (x * 0.0078);
	    yg = (y * 0.0078);
	    zg = (z * 0.0078);

	}

	return 0;
}




