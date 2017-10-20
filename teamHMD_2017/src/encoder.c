/*
 * encoder.c
 *
 *  Created on: 2017. 6. 28.
 *      Author: ghbae
 */

/*******************************************************************************
* Include
*******************************************************************************/
#include "global.h"
#include "typedefinition.h"
#include "systemController.h"
#include "vehicleController.h"

/*******************************************************************************
* Constant
*******************************************************************************/


/*******************************************************************************
* Define & MACRO
*******************************************************************************/
#define DEFINE_PORT_ENC_PHASEA			P02_IOCR4.B.PC6=0b00010 //INPUT : T3IN_A. PULL-UP
#define DEFINE_PORT_ENC_PHASEB			P02_IOCR4.B.PC7=0b00010 //INPUT : T3UDE_A. PULL-UP
#define DEFINE_PORT_ENC_PHASEZ			P02_IOCR8.B.PC8=0b00010 //INPUT : T4IN_A. PULL-UP


/*******************************************************************************
* Type Definition
*******************************************************************************/
 /* configuration of delay loop */

/*******************************************************************************
* Exported Global Variables
*******************************************************************************/


/*******************************************************************************
* Memeber Functions
*******************************************************************************/
void encoderInit(void) {

	UNLOCK_WDT_CON;
	GPT120_CLC.B.DISR = 0; //Module disable is not requested.
	while (GPT120_CLC.B.DISS == 1) { //Wait until enable.
		;
	}
	LOCK_WDT_CON;

	DEFINE_PORT_ENC_PHASEA;
	DEFINE_PORT_ENC_PHASEB;
	DEFINE_PORT_ENC_PHASEZ;

	//input select T3IN / T3EUD
	//set T3I, T3R, T3UD, T3UDE, T3M

	GPT120_PISEL.B.IST3IN = 0b00; //T3IN A to mapping P02.6 //manual 2686 page.
	GPT120_PISEL.B.IST3EUD = 0b00; //T3EUD_A to mapping P02.7
	GPT120_PISEL.B.IST4IN = 0b00; //T4IN_A to mapping P02.8

	GPT120_T3CON.B.T3I = 0b011; //MANUAL 2631PAGE. DETECT BOTH RISING AND FALLING EDGE
	GPT120_T3CON.B.T3M = 0b111; //Incremental Interface Mode (Edge Detection Mode)
	GPT120_T3CON.B.T3UD = 0; //Timer T3 counts up. Note : This bit only controls count direction of T3 if bit T3UDE = 0.
	GPT120_T3CON.B.T3UDE = 1; //Count direction is controlled by input T3EUD
	GPT120_T3CON.B.BPS1 = 00; //Default. fGPT/8

	GPT120_T3CON.B.T3R = 1; //Timer T3 runs.
}

