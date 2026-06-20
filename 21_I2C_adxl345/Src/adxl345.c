#include "adxl345.h"

char data;

uint8_t data_rec[6];

uint8_t adxl_read_address(uint8_t reg)
{
	I2C1_byteRead(DEVID_ADDR, reg, &data);
}

void adxl_write(uint8_t reg, char value)
{
	char data[1];
	data[0] = value;

	I2C1_burstWrite(DEVID_ADDR, reg, 1, data);
}

void adxl_read_values(uint8_t reg)
{
	I2C1_burstRead(DEVID_ADDR, reg, 6, (char*)data_rec);
}

void adxl345_init(void)
{
	uint8_t adxl_addr;

	/*Enable I2C*/
	I2C1_init();

    /*Read the DEVID, this should return 0xE5*/
	adxl_addr = adxl_read_address(DEVID_R);

    /*Set data format range to +-4g*/
	adxl_write(DATA_FORMAT_R, FOUR_G);

	/*Reset all bits*/
	adxl_write(POWER_CTL_R, RESET);

	/*Configure power control measure bit*/
	adxl_write(POWER_CTL_R, SET_MEASURE_B);
}

/*Here using I2C protocol we are configuring adxl345 sensor
 *
 * */
