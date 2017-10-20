/*
 * DMS.c
 *
 *  Created on: 2017. 7. 1.
 *      Author: User
 */

/*******************************************************************************
 * Include
 *******************************************************************************/
#include "DMS.h"
#include "PID.h"
#include "tc23xa/IfxVadc_reg.h"
#include "tc23xa/IfxPort_reg.h"
#include "cameraController.h"
/*******************************************************************************
 * Constant
 *******************************************************************************/
int DMS_input_bottom[9] = { 0, };
int DMS_input_top[9] = { 0, };
int DMS_input_right[9] = { 0, };
int DMS_input_left[9] = { 0, };
int a = 0;
int b = 0;
int empty = 0;

int DMS_BOTTOM = 0;
int DMS_TOP =0;
int DMS_RIGHT = 0;
int DMS_LEFT =0;


int count_DMS=0;
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

/*******************************************************************************
 * Exported Functions
 * Function name: Infrared_Data_Process
 * Description:	Infrared_data using median filter
 *******************************************************************************/


void AEB(void)
{
	if(DMS_TOP > 700)
		PID_control(0);
}


void DMS_filter(void){			//Median Filter

	DMS_input_bottom[count_DMS] = VADC_G1_RES3.B.RESULT;
	DMS_input_top[count_DMS] = VADC_G1_RES8.B.RESULT;
	DMS_input_left[count_DMS] = VADC_G1_RES9.B.RESULT;
	DMS_input_right[count_DMS] = VADC_G0_RES10.B.RESULT;

	/*camera2D.result.Infraredval = VADC_G1_RES4.B.RESULT;
		if(camera2D.result.Infraredval < 1000 )
			camera2D.result.Infaredcnt++;
		if(camera2D.result.Infraredval > 3000 && camera2D.result.Infraredval < 5000)
			camera2D.result.Infaredcnt =0;*/

		count_DMS++;

	if(count_DMS == 9){
		for(a = 0 ; a < 8 ; a++){

			for(b = a + 1 ; b < 9 ; b++){

				if(DMS_input_bottom[a] > DMS_input_bottom[b]){

					empty = DMS_input_bottom[a];

					DMS_input_bottom[a] = DMS_input_bottom[b];

					DMS_input_bottom[b] = empty;
				}
			}
		}

		for(a = 0 ; a < 8 ; a++){

			for(b = a + 1 ; b < 9 ; b++){

				if(DMS_input_top[a] > DMS_input_top[b]){

					empty = DMS_input_top[a];

					DMS_input_top[a] = DMS_input_top[b];

					DMS_input_top[b] = empty;
				}
			}
		}
				for(a = 0 ; a < 8 ; a++){

					for(b = a + 1 ; b < 9 ; b++){

						if(DMS_input_right[a] > DMS_input_right[b]){

							empty = DMS_input_right[a];

							DMS_input_right[a] = DMS_input_right[b];

							DMS_input_right[b] = empty;
						}
					}
				}

				for(a = 0 ; a < 8 ; a++){

					for(b = a + 1 ; b < 9 ; b++){

						if(DMS_input_left[a] > DMS_input_left[b]){

							empty = DMS_input_left[a];

							DMS_input_left[a] = DMS_input_left[b];

							DMS_input_left[b] = empty;
						}
					}
				}
		DMS_BOTTOM = DMS_input_bottom[4];
		DMS_TOP = DMS_input_top[4];
		DMS_RIGHT = DMS_input_right[4];
		DMS_LEFT = DMS_input_left[4];
		count_DMS=0;
	}

}


int getDMS_BOTTOM(void){
	return	DMS_BOTTOM;
}

int getDMS_TOP(void){
	return	DMS_TOP;
}

int getDMS_RIGHT(void){
	return	DMS_RIGHT;
}

int getDMS_LEFT(void){
	return	DMS_LEFT;
}
/*******************************************************************************
 * Local Functions
 * Function name:
 * Description:
 *******************************************************************************/


