#include "stm32f4xx.h"

#include "i2c.h"

/*RM: Mask value to Enable GPIOB register and assign to AHB1ENR register*/
#define GPIOBEN   (1U<<1)

/*RM: Mask value to Enable I2C1 peripheral and assign to APB1ENR register*/
#define I2C1EN    (1U<<21)

/*RM: Mask value to Enable I2C1 peripheral in standard mode and assign to CCR register(0b 0101 0000 = 80(decimal))*/
#define I2C_100KHZ 80

/*RM: Mask value to set the Max rise time for standard mode of I2C and assign to TRISE register*/
#define SD_MODE_MAX_RISE_TIME 17

/*RM: Mask value to Enable I2C peripheral and assign to CR1 register*/
#define CR1_PE    (1U<<0)

/*RM: Mask value to check the status of I2C peripheral for Busy or not and check for SR2 register*/
#define SR2_BUSY  (1U<<1)

/*RM: Mask value to start generation of the I2C peripheral and assign to CR1 register*/
#define CR1_START (1U<<8)

/*RM: Mask value to set start bit(controller mode) the I2C peripheral and assign to SR1 register*/
#define SR1_SB    (1U<<0)

/*RM: Mask value to set Data register empty Transmitter the I2C peripheral and assign to SR1 register*/
#define SR1_TXE   (1U<<7)

/*RM: Mask value to reset ACK bit the I2C peripheral and assign to CR1 register*/
#define CR1_ACK   (1U<<10)

/*RM: Mask value to stop condition of the I2C peripheral and assign to CR1 register*/
#define CR1_STOP  (1U<<9)

/*RM: Mask value to check Data register not empty bit of the I2C peripheral and assign to CR1 register*/
#define SR1_RXNE  (1U<<6)

/*RM: Mask value to check Data register not empty bit of the I2C peripheral and assign to CR1 register*/
#define SR1_BTF   (1U<<2)

/*RM: Mask value to check target address is matched or not in ADDR bit of the I2C peripheral and assign to SR1 register*/
#define SR1_ADDR  (1U<<1)

/*Pin-out
 *PB8 - SCL
 *PB9 - SDA
 * */

void I2C1_init(void)
{

	/*Enable clock access to GPIOB*/
	RCC->AHB1ENR |= GPIOBEN;

	/*Set PB8 mode to alternate function as it is SCL for I2C1*/
	GPIOB->MODER &=~ (1U<<16);
	GPIOB->MODER |= (1U<<17);

	/*Set PB9 mode to alternate function as it is SDA for I2C1*/
	GPIOB->MODER &=~ (1U<<18);
	GPIOB->MODER |= (1U<<19);

	/*Set PB8 output type to open drain for SCL*/
	GPIOB->OTYPER |= (1U<<8);

    /*Set PB9 output type to open drain for SDA*/
	GPIOB->OTYPER |= (1U<<9);

	/*Enable pull up for PB8 for SCL*/
	GPIOB->PUPDR |= (1U<<16);
	GPIOB->PUPDR &=~ (1U<<17);

	/*Enable pull up for PB9 for SDA*/
	GPIOB->PUPDR |= (1U<<18);
	GPIOB->PUPDR &=~ (1U<<19);

	/*Set PB8 alternate function type to I2C AF4 in AFRL High register*/
	GPIOB->AFR[1] &=~ (1U<<0);
	GPIOB->AFR[1] &=~ (1U<<1);
	GPIOB->AFR[1] |=  (1U<<2);
	GPIOB->AFR[1] &=~ (1U<<3);

	/*Set PB9 alternate function type to I2C AF4 in AFRL High register*/
	GPIOB->AFR[1] &=~ (1U<<4);
	GPIOB->AFR[1] &=~ (1U<<5);
	GPIOB->AFR[1] |=  (1U<<6);
	GPIOB->AFR[1] &=~ (1U<<7);

	/*Enable clock access to I2C1*/
	RCC->APB1ENR |= I2C1EN;

	/*Enter reset mode*/
	I2C1->CR1 |= (1U<<15);

	/*Come out of reset mode*/
	I2C1->CR1 &=~ (1U<<15);

	/*Set peripheral clock frequency to 16MHz*/
	I2C1->CR2 = (1U<<4);

	/*Set the I2C peripheral in standard mode frequency to 100KHZ*/
	I2C1->CCR = I2C_100KHZ;

	/*Set the I2C peripheral rise time as 17*/
	I2C1->TRISE = SD_MODE_MAX_RISE_TIME;

	/*Enable the I2C peripheral*/
	I2C1->CR1 |= CR1_PE;
}

void I2C1_byteRead(char slaveaddr, char memoryaddr, char* data)
{
	/*Temporary variable to read memory of SR2*/
	volatile int tmp;

	/*RM: Check whether I2C peripheral is busy or not*/
	while(I2C1->SR2 & (SR2_BUSY)){}

	/*RM: Generate the start condition*/
	I2C1->CR1 = CR1_START;

	/*RM: Check whether I2C peripheral start bit is set or not*/
	while(!(I2C1->SR1 & (SR1_SB))){}

	/*RM: Transmit the slave address by master(controller mode) and write*/
	I2C1->DR = slaveaddr << 1;

	/*RM: Wait until addr flag is set*/
	while(!(I2C1->SR1 & (SR1_ADDR))){}

	/*RM: Clear the addr flag*/
	tmp = I2C1->SR2;

	/*RM: Send memory address in slave from where data to be read*/
	I2C1->DR = memoryaddr;

	/*RM: Wait until transmitter empty*/
	while(!(I2C1->SR1 & SR1_TXE)){}

	/*RM: Generate the restart condition*/
	I2C1->CR1 = CR1_START;

	/*RM: Check whether I2C peripheral start bit is set or not*/
	while(!(I2C1->SR1 & (SR1_SB))){}

	/*RM: Transmit the slave address by master(controller mode) and read*/
	I2C1->DR = slaveaddr << 1 | 1;

	/*RM: Wait until addr flag is set*/
	while(!(I2C1->SR1 & (SR1_ADDR))){}

	/*RM: Disable the acknowledge bit*/
	I2C1->CR1 &=~ CR1_ACK;

	/*RM: Clear the addr flag*/
	tmp = I2C1->SR2;

	/*RM: Generate Stop condition after data received*/
	I2C1->CR1 |= CR1_STOP;

	/*RM: Wait until RXNE flag is set*/
	while(!(I2C1->SR1 & (SR1_RXNE))){}

	/*RM: Read data from DR register*/
	*data++ = I2C1->DR;

}

void I2C1_burstRead(char slaveaddr, char memoryaddr, int n, char* data)
{
	/*Temporary variable to read memory of SR2*/
	volatile int tmp;

	/*RM: Check whether I2C peripheral is busy or not*/
	while(I2C1->SR2 & (SR2_BUSY)){}

	/*RM: Generate the start condition*/
	I2C1->CR1 = CR1_START;

	/*RM: Check whether I2C peripheral start bit is set or not*/
	while(!(I2C1->SR1 & (SR1_SB))){}

	/*RM: Transmit the slave address(7 bit) by master(controller mode) and write*/
	I2C1->DR = slaveaddr << 1;

	/*RM: Wait until addr flag is set*/
	while(!(I2C1->SR1 & (SR1_ADDR))){}

	/*RM: Clear the addr flag*/
	tmp = I2C1->SR2;

	/*RM: Wait until transmitter empty*/
	while(!(I2C1->SR1 & SR1_TXE)){}

	/*RM: Send memory address in slave from where data to be read*/
	I2C1->DR = memoryaddr;

	/*RM: Generate the restart condition*/
	I2C1->CR1 = CR1_START;

	/*RM: Check whether I2C peripheral start bit is set or not*/
	while(!(I2C1->SR1 & (SR1_SB))){}

	/*RM: Transmit the slave address by master(controller mode) and read*/
	I2C1->DR = slaveaddr << 1 | 1;

	/*RM: Wait until addr flag is set*/
	while(!(I2C1->SR1 & (SR1_ADDR))){}

	/*RM: Clear the addr flag*/
	tmp = I2C1->SR2;

	/*RM: Disable the acknowledge bit*/
	I2C1->CR1 |= CR1_ACK;

	while(n > 0U)
	{
		/*If one byte left*/
		if(n == 1U)
		{
			/*RM: Disable the acknowledge bit*/
			I2C1->CR1 &=~ CR1_ACK;

			/*RM: Generate Stop condition after data received*/
			I2C1->CR1 |= CR1_STOP;

			/*RM: Wait until RXNE flag is set*/
			while(!(I2C1->SR1 & (SR1_RXNE))){}

			/*RM: Read data from DR register*/
			*data++ = I2C1->DR;
		}
		else
		{
			/*RM: Wait until RXNE flag is set*/
			while(!(I2C1->SR1 & (SR1_RXNE))){}

			/*RM: Read data from DR register*/
			*data++ = I2C1->DR;

			n--;

		}
	}

}

void I2C1_burstWrite(char slaveaddr, char memoryaddr, int n, char* data)
{
	/*Temporary variable to read memory of SR2*/
	volatile int tmp;

	/*RM: Check whether I2C peripheral is busy or not*/
	while(I2C1->SR2 & (SR2_BUSY)){}

	/*RM: Generate the start condition*/
	I2C1->CR1 = CR1_START;

	/*RM: Check whether I2C peripheral start bit is set or not*/
	while(!(I2C1->SR1 & (SR1_SB))){}

	/*RM: Transmit the slave address(7 bit) by master(controller mode) and write*/
	I2C1->DR = slaveaddr << 1;

	/*RM: Wait until addr flag is set*/
	while(!(I2C1->SR1 & (SR1_ADDR))){}

	/*RM: Clear the addr flag*/
	tmp = I2C1->SR2;

	/*RM: Wait until transmitter empty*/
	while(!(I2C1->SR1 & SR1_TXE)){}

	/*RM: Send memory address in slave from where data to be read*/
	I2C1->DR = memoryaddr;

	for(int i = 0; i < n; i++)
	{
		/*RM: Wait until transmitter empty*/
		while(!(I2C1->SR1 & SR1_TXE)){}

		/*RM: Transmit memory address*/
		I2C1->DR = *data++;

	}

	/*RM: Wait until transfer is finished*/
	while(!(I2C1->SR1 & SR1_BTF)){}

	/*RM: Generate stop condition*/
	I2C1->CR1 |= CR1_STOP;
}
