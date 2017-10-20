/*
 * Hll.c
 *
 *  Created on: 2017. 7. 1.
 *      Author: User
 */

/*******************************************************************************
 * Include
 *******************************************************************************/
#include "DMS.h"
#include "vehicleController.h"
#include "Schedular.h"

#include "tc23xa/IfxPort_reg.h"
#include "tc23xa/IfxStm_reg.h"
#include "tc23xa/IfxVadc_reg.h"
#include "tc23xa/IfxGtm_reg.h"
#include "tc23xa/IfxScu_reg.h"
/*******************************************************************************
 * Constant
 *******************************************************************************/
int Hill_state = 0;
int Hill_position = 0;
int hill_check=0;
int Hill_cnt1=0;
int Hill_cnt2=0;
/*******************************************************************************
 * Define & MACRO
 *******************************************************************************/

/*******************************************************************************
 * Type Definition
 *******************************************************************************/

/*******************************************************************************
 * Prototypes of Local Functions
 *******************************************************************************/

/*******************************************************************************
 * Static Variable
 *******************************************************************************/

/*******************************************************************************
 * Exported Global Variables
 *******************************************************************************/


extern int DMS_BOTTOM;
extern int DMS_TOP;

//extern int Pure_Pursuit_state;

/*******************************************************************************
 * Exported Functions
 * Function name: Hill_detect.c
 * Description: algorithm for detecting hill
 *******************************************************************************/
void Hill(void) {

	if (DMS_BOTTOM > 700 && DMS_TOP < 200 && hill_check==0) {
		Hill_state = 1;
		Hill_position = 0;
	}
	while (Hill_state == 1)
	{
		hill_check=1;
		switch (Hill_position)
		{
		case 0:
			//속도 증가
			pwmDcMotor(105,0);
			//서버 고정
			pwmServo(31250,0);
			if (DMS_BOTTOM < 100 )  //둘다 인식x
				Hill_position = 1;
			break;

		case 1:   //완만
			//속도 조금 감소
			pwmDcMotor(100,0);
			//서버 조정은?
			if (DMS_BOTTOM > 450)  //내리막길 시작
				Hill_state = 0;
				//Hill_position = 2;
			break;

		case 2:  //내리막길
			pwmDcMotor(50,0);
			if (DMS_BOTTOM > 700)  //30cm 내리막길 끝
				Hill_position = 3;
			break;

		case 3:
			pwmDcMotor(70,0);
			Hill_cnt1 =1;
			if (DMS_BOTTOM < 200 )
				Hill_state = 0;
			break;
		}
		if (Hill_state == 0)
			break;

	}
}
