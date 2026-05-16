/*Include generic header file for the micro-controller then go this file add the name
 *of the controller STM32F411xE to symbols of the properties of the project
 * */
#include "stm32f4xx.h"

/*Inclusion of ADC header file*/
#include "adc.h"

/*RM: Mask value to enable clock access to GPIOA port and need to assign to AHB1ENR register*/
#define GPIOAEN             (1U<<0)

/*RM: Mask value enable to clock access to ADC1 module and need to assign to APB2ENR register*/
#define ADC1EN				(1U<<8)

/*RM: Mask value to enable sequence for channel 1 of ADC Module and need to assign to ADC_SQR3*/
#define ADC_CH1             (1U<<0)

/*RM: Mask value for Length of the Sequence of ADC Module. Here, entire ADC_SQR1 register is assigned with 0 instead of just L(bit:23 to 20)*/
#define ADC_SEQ_LEN_1       0x00

/*RM: Mask value for Control register 2 to turn on ADC and need to assign to ADC1_CR2*/
#define CR2_ADON            (1U<<0)

/*RM: Mask value for Control register 2 to start conversion in ADC and to assign to ADC1_CR2*/
#define CR2_SWSTART         (1U<<30)

/*RM: Mask value for checking the End of conversion of Analog data and need to assign to ADC1_SR*/
#define SR_EOC              (1U<<1)

/*RM: Mask value for continuous conversion and need to assign to ADC1_CR2*/
#define CR2_CONT            (1U<<1)

void pa1_adc_init(void)
{

	/***Configure the ADC GPIOA Pin***/

	/*RM: Enable clock access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/*RM: Set the PA1 mode to analog*/
    GPIOA->MODER |= (1U<<2);
    GPIOA->MODER |= (1U<<3);

	/***Configure the ADC Module***/

	/*RM: Enable clock access to ADC1*/
    RCC->APB2ENR |= ADC1EN;

	/***Configure ADC Parameters***/

    /*Conversion sequence start of channel 1 of ADC*/
    ADC1->SQR3 = ADC_CH1;

    /*Conversion sequence length of channel 1 of ADC*/
    ADC1->SQR1 = ADC_SEQ_LEN_1;

    /*Enable ADC1 Module*/
    ADC1->CR2 |= CR2_ADON;
}

void start_conversion(void)
{
	/*RM: Enable Continuous conversion*/
	ADC1->CR2 |= CR2_CONT;

    /*RM: Software Start the conversion of channel of ADC*/
	ADC1->CR2 |= CR2_SWSTART;
}

uint32_t adc_read(void)
{
	/*RM: Wait for the conversion to be complete*/
	while(!(ADC1->SR & SR_EOC)){}

	/*RM: Read the converted Result*/
	return ADC1->DR;
}

/*
 * STM32F411RE micro-controller has 1 ADC module connected to APB2 Bus with 16 channels.
 * Here PA1 is considered for ADC1_1 channel to develop driver as per Table 8. STM32F411xC/xE pin definitions (continued).
 * SQR(Sequence registers) registers of ADC need to be checked to configure conversion sequence.
 * Here To configure sequence for Channel 1 of ADC module we need to check for ADC_SQR3 register as
 * it contains pins for channels like from channel 1 to 6 as we are dealing with channel 1.
 * SQ - > It represents sequence number or order in which channel need to be configured here we
 * need to channel 1 at SQ1 hence ADC_CH1 is used.
 * Length of the sequence -> how many channels we are going to do sampling and conversion, In our case it is 1
 * so we need to use value 0000 in L(bit:23 to 20) of ADC_SQR1 to configure the length as 1.
 * */
