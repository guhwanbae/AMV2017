/*
 * encoder_set.h
 *
 *  Created on: 2017. 6. 27.
 *      Author: User
 */

#ifndef ENCODER_SET_H_
#define ENCODER_SET_H_

#include "typedefinition.h"
#include "systemInit.h"

typedef int enccnt;

void Encoder_set(void);
void rpm_Front(void);
//void rpm_Back();
enccnt Car_rpm_find(void);
int Car_angle_find(void);
enccnt getFrontCnt(void);

#endif /* 0_SRC_0_APPSW_APP_INC_ENCODER_SET_H_ */
