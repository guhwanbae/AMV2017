/*
 * PID.c
 *
 *  Created on: 2017. 6. 29.
 *      Author: User
 */

#include "PID.h"
#include "encoder_set.h"
#include "tc23xa/IfxPort_reg.h"
#include "vehicleController.h"


/*******************************************************************************
 * Constant
 *******************************************************************************/

long double Before_error = 0;
long double integral = 0;
long double error;
long double derivative;
long double Motor_val;

long double Kp = 4.21;		//Gain for proportional 4
long double Ki = 0.000578;	//Gain for integration 0.0006
long double Kd = 0.00389;	//Gain for derivative 0.004
/*******************************************************************************
 * Global
 *******************************************************************************/

extern enccnt Car_rpm;
extern vehicleControl vehicle;
/*******************************************************************************
 * Exported Functions
 * Function name: PID_control
 * Description: PID_Control for AEB
 *******************************************************************************/

void PID_control(long double Expected_rpm){

	error = Expected_rpm - Car_rpm;

	integral = integral + (error * 10);
	derivative = (error - Before_error) / 10; // 3

	Motor_val = Kp * error + Ki * integral + Kd * derivative;

	Before_error = error;

	if(Motor_val < -90)
	{
		Motor_val = -90;
		pwmDcMotor(0,-Motor_val);
	}
	else if(Motor_val < 0)
	{
		pwmDcMotor(0, -Motor_val);
	}
	if(Car_rpm < 150)
	{
		pwmDcMotor(0, 0);
		vehicle.statusData.camData.AEB=1;
	}
}

