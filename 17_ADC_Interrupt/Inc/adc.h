#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

/*RM: Mask value for checking the End of conversion of Analog data and need to assign to ADC1_SR*/
#define SR_EOC              (1U<<1)

void pa1_adc_init(void);

void start_conversion(void);

uint32_t adc_read(void);

void pa1_adc_interrupt_init(void);

#endif /* ADC_H_ */
