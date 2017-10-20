/*
 * timerInputModule.c
 *
 *  Created on: 2017. 6. 26.
 *      Author: ghbae
 */

/*******************************************************************************
 * Include
 *******************************************************************************/
#include "global.h"
#include "timerController.h"
#include "systemController.h"

/*******************************************************************************
 * Constant
 *******************************************************************************/

/*******************************************************************************
 * Define & MACRO
 *******************************************************************************/

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

void timerInputModuleInit(void) {

	DEFINE_PORT_ENC_PHASEA;            //P14.5 TIM0_0
	DEFINE_PORT_ENC_PHASEB;            //P14.4 TIM0_7



	UNLOCK_WDT_CON;
	GTM_CLC.B.DISR = 0;
	while (GTM_CLC.B.DISS == 1) {
		;
	}
	LOCK_WDT_CON;

	GTM_CMU_CLK_EN.B.EN_CLK0 = 0b10;      //enable clock signal
	GTM_TBU_CHEN.B.ENDIS_CH0 = 0b10;

	GTM_TIM0INSEL.B.CH0SEL = 0b0100;    //channel select mode P14.5 TIN85
	GTM_TIM0INSEL.B.CH7SEL = 0b0010;        //channel select mode P14.6 TIN84
	//GTM_TIM0INSEL.B.CH1SEL = 0b0010;      //channel select mode P02.1 TIN1

	GTM_TIM0_CH0_CTRL.B.TIM_EN = 1;         //TIM channel enable
	GTM_TIM0_CH0_CTRL.B.TIM_MODE = 0b000;   //TIM mode TPWM
	GTM_TIM0_CH0_CTRL.B.OSM = 0b0;         //TIM continous operation mode
	GTM_TIM0_CH0_CTRL.B.CICTRL = 0b0;      //use signal x as input for channel x
	GTM_TIM0_CH0_CTRL.B.GPR0_SEL = 0b11;   //use CNTS as input
	GTM_TIM0_CH0_CTRL.B.GPR1_SEL = 0b11;   //use CNT as input
	GTM_TIM0_CH0_CTRL.B.CNTS_SEL = 0;      //use CNT register as input
	GTM_TIM0_CH0_CTRL.B.DSL = 1;         //Measurement starts with rising edge
	GTM_TIM0_CH0_CTRL.B.ISL = 0; //use DSL bit for selecting active signal level
	GTM_TIM0_CH0_CTRL.B.ECNT_RESET = 1; //ECNT counter is reset with periodic sampling
	GTM_TIM0_CH0_CTRL.B.FLT_EN = 0;         //Filter disabled
	GTM_TIM0_CH0_CTRL.B.CLK_SEL = 0b000;   //CMU_CLK0 selected
	GTM_TIM0_CH0_CTRL.B.TOCTRL = 0b01;  //Timeout feature enabled for both edges

	GTM_TIM0_CH7_CTRL.B.TIM_EN = 1;         //TIM channel enable
	GTM_TIM0_CH7_CTRL.B.TIM_MODE = 0b000;   //TIM mode TPWM
	GTM_TIM0_CH7_CTRL.B.OSM = 0b0;         //TIM continous operation mode
	GTM_TIM0_CH7_CTRL.B.CICTRL = 0b0;      //use signal x as input for channel x
	GTM_TIM0_CH7_CTRL.B.GPR0_SEL = 0b11;   //use CNTS as input
	GTM_TIM0_CH7_CTRL.B.GPR1_SEL = 0b11;   //use CNT as input
	GTM_TIM0_CH7_CTRL.B.CNTS_SEL = 0;      //use CNT register as input
	GTM_TIM0_CH7_CTRL.B.DSL = 1;         //Measurement starts with rising edge
	GTM_TIM0_CH7_CTRL.B.ISL = 0; //use DSL bit for selecting active signal level
	GTM_TIM0_CH7_CTRL.B.ECNT_RESET = 1; //ECNT counter is reset with periodic sampling
	GTM_TIM0_CH7_CTRL.B.FLT_EN = 0;         //Filter disabled
	GTM_TIM0_CH7_CTRL.B.CLK_SEL = 0b000;   //CMU_CLK0 selected
	GTM_TIM0_CH7_CTRL.B.TOCTRL = 0b01;  //Timeout feature enabled for both edges

	GTM_TIM0_CH0_IRQ_EN.B.NEWVAL_IRQ_EN = 1;
	GTM_TIM0_CH0_IRQ_MODE.B.IRQ_MODE = 0b10;

	GTM_TIM0_CH7_IRQ_EN.B.NEWVAL_IRQ_EN = 1;
	GTM_TIM0_CH7_IRQ_MODE.B.IRQ_MODE = 0b10;
}
