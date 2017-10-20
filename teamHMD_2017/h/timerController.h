/*
 * timerController.h
 *
 *  Created on: 2017. 6. 26.
 *      Author: ghbae
 */

#ifndef TIMERCONTROLLER_H_
#define TIMERCONTROLLER_H_

/*******************************************************************************
* Include
*******************************************************************************/
#include "typedefinition.h"

/*******************************************************************************
* Constant
*******************************************************************************/

/*******************************************************************************
* Define & MACRO
*******************************************************************************/

/*	PORT DEFINE	*/
#define DEFINE_PORT_SERVO				P33_IOCR8.B.PC10=0b10001	//Servo   P33.10   alt1 tom1_0   tout32

#define DEFINE_PORT_MOTORDRIVE_AINH		P02_IOCR0.B.PC0=0b10000	//Motor1 INH   P02.0
#define DEFINE_PORT_MOTORDRIVE_AIN		P02_IOCR0.B.PC1=0b10001	//Motor1 IN    P02.1
#define DEFINE_PORT_MOTORDRIVE_BINH		P00_IOCR0.B.PC2=0b10000	//INH   P00.2
#define DEFINE_PORT_MOTORDRIVE_BIN		P00_IOCR0.B.PC3=0b10001	//IN   P00.3

#define DEFINE_PORT_ENC_PHASEA			P14_IOCR4.B.PC5=0b00000	//P14.5 TIM0_0
#define DEFINE_PORT_ENC_PHASEB			P14_IOCR4.B.PC4=0b00000	//P14.4 TIM0_7

/************************************
 *	CAMLEFT			: CAMERA 1		*
 *	CAMRIGHT		: CAMERA 2		*
 *	MOTORDRIVE A	: DRIVE 1		*
 *	MOTORDRIVE B	: DRIVE 2		*
 ************************************/

/*******************************************************************************
* Type Definition
*******************************************************************************/

/*******************************************************************************
* Member
*******************************************************************************/

/*******************************************************************************
* Member Functions
*******************************************************************************/
void timerInputModuleInit(void);
void timerOutputModuleInit(void);

#endif /* TIMERCONTROLLER_H_ */
