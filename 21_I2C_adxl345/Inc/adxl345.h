#ifndef ADXL345_H_
#define ADXL345_H_

#include <stdint.h>

#include "i2c.h"

/*DS: Device ID register address of adxl345 sensor*/
#define DEVID_R				(0x00)

/*DS: alternate Slave(adxl345 sensor) address to be pass in i2c functions*/
#define DEVID_ADDR			(0x53)

/*DS: Data format register address of adxl345 sensor*/
#define DATA_FORMAT_R		(0x31)

/*DS: Power control register address of adxl345 sensor*/
#define POWER_CTL_R			(0x2D)

/*DS: Data register address of adxl345 sensor*/
#define DATAX0              (0x32)

/*DS: Alias name for Data register address*/
#define DATA_START_ADDR     DATAX0

/*DS: Mask value to set the Range bits D1 = 0, D0 = 1 */
#define FOUR_G              (0x01)

/*DS: Mask value to set the wake up bits of power control register as D0 = 0, D1 = 0 to read data in frequency of 8Hz*/
#define RESET               (0x00)

/*DS: Mask value to set the measure bit of power control register to read the measured value from sensor */
#define SET_MEASURE_B       (0x08)



void adxl345_init(void);

void adxl_read_values(uint8_t reg);

void adxl_write(uint8_t reg, char value);

uint8_t adxl_read_address(uint8_t reg);

#endif /* ADXL345_H_ */

/*
 * DS - Data sheet of adxl345 sensor
 * */


