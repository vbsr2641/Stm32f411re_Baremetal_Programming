#ifndef EXTI_H_
#define EXTI_H_

#include "stm32f4xx.h"

void pc13_exti_init(void);

/*RM:Mask value to check line 13 of interrupt in EXTI*/
#define LINE13 (1U<<13)

#endif /* EXTI_H_ */
