/*
 * systemInit.h
 *
 *  Created on: 2017. 6. 28.
 *      Author: ghbae
 */

#ifndef SYSTEMINIT_H_
#define SYSTEMINIT_H_

#include "typedefinition.h"

void PLL200(void);

void BaseTick(uint32 nPeriodValue);
void Sys_Initialization(void);
void encoderTestInit(void);

#endif /* SYSTEMINIT_H_ */
