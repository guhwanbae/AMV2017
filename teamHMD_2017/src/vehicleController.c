/*
 * vehicleController.c
 *
 *  Created on: 2017. 6. 26.
 *      Author: ghbae
 */

/*******************************************************************************
 * Include
 *******************************************************************************/
#include "global.h"
#include "vehicleController.h"
#include "cameraController.h"
#include "timerController.h"
#include "encoder_set.h"

/*******************************************************************************
 * Constant
 *******************************************************************************/

/*******************************************************************************
 * Define & MACRO
 *******************************************************************************/

/*******************************************************************************
 * Type Definition
 *******************************************************************************/

/*******************************************************************************
 * Global Variables
 *******************************************************************************/
vehicleControl vehicle;
extern cameraStruct2D camera2D;
/*******************************************************************************
 * Memeber Functions
 *******************************************************************************/

void peripheralInit(void) {
	adcInit();
	timerOutputModuleInit();
	timerInputModuleInit();
	Encoder_set();

	vehicle.statusData.log.oldleft =0;
	vehicle.statusData.log.oldright=0;
	vehicle.statusData.lineStatus.oldTurnState=Straight;
	vehicle.statusData.lineStatus.oldangle =0;
	vehicle.statusData.lineStatus.turnCnt=0;
	vehicle.statusData.camData.AEB=0;

	camera2D.result.Infraredval=0;
	camera2D.result.Infaredcnt=0;

	cameraInit();
}

void peripheralOn(void) {
	cameraClock(CAM_PERIOD_SI, CAM_DUTY_SI, CAM_PERIOD_CLK, CAM_DUTY_CLK);
	pwmServo(31250, 0);
}

void pwmServo(int servo0, double servo_angle) {
	//servo_angle = ( (servo_angle / PHI) * 180 );
	GTM_TOM1_CH0_SR0.U = servo0; //31250
	//2332.5       + left
	GTM_TOM1_CH0_SR1.U = 2376.5 - servo_angle * 16; // 인터넷상에서 나온대로 1도가 10 usec  //2812.5      2652.5
} //2295

void pwmDcMotor(long double motor0, long double motor1) {
	P02_OUT.B.P0 = 1;
	P00_OUT.B.P2 = 1;

	GTM_TOM1_CH9_SR0.U = 300;
	GTM_TOM1_CH9_SR1.U = motor0;

	GTM_TOM1_CH2_SR0.U = 300;
	GTM_TOM1_CH2_SR1.U = motor1;
}

