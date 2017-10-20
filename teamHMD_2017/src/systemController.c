/*
 * systemController.c
 *
 *  Created on: 2017. 6. 26.
 *      Author: ghbae
 */

/*******************************************************************************
 * Exported Functions
 * Function name:
 * Description: function for initializing port output for LED
 *******************************************************************************/
extern void buildFrame(int dummy);
extern void check2DProcess(int dummy);

/*******************************************************************************
 * Include
 *******************************************************************************/
#include "global.h"
#include "systemController.h"
#include "cameraController.h"
#include "imageProcess.h"
#include "interrupts.h"
#include "encoder_set.h"
#include "Schedular.h"
#include "systemInit.h"
#include "DMS.h"
#include "vehicleController.h"

#define DMS_FREQ 10

enccnt Front_cnt = 0;
int DMS_cnt=0;

void systemInit(void) {
	systemTimerInit();
	phaseLockLoopInit();
}

void systemTimerInit(void) {

	/* Value for periodic event*/
	uint32 u32_ComparValue = BASE_PRD;

	/*InterruptInitalize*/
	InterruptInit();

	/*Registration of interrupt handler )*/

	/*	Linear
	 * InterruptInstall(SRC_ID_GTMTOM05, getPixel, 2, 0);
	 * InterruptInstall(SRC_ID_GTMTOM07, checkLinearProcess, 1, 0);
	*/


	InterruptInstall(SRC_ID_GTMTOM05, buildFrame, 2, 0);
	InterruptInstall(SRC_ID_GTMTOM07, check2DProcess, 1, 0);
	InterruptInstall(SRC_ID_GTMTIM07, rpm_Front, 3, 0);
	InterruptInstall(SRC_ID_STM0SR0, BaseTick, 4, u32_ComparValue);
	/*  2D

	*/

	//InterruptInstall(SRC_ID_GTMTIM00, rpm_1, 3, 0);
	//InterruptInstall(SRC_ID_GTMTIM07, rpm_2, 4, 0);

	//InterruptInstall(SRC_ID_STM0SR0, BaseTick, 5, u32_ComparValue);

	//InterruptInstall(SRC_ID_GPT120T3, encoderCountReset, 6, 0);

	/*System Timer Module Configuration */

	STM0_ISCR.B.CMP0IRR			= RESET_CMP_INT_FLAG;
	STM0_CMCON.B.MSIZE0			= CMP_REGISTER_SIZE;
	STM0_CMCON.B.MSTART0		= CMP_LSB_POSITION;
	STM0_CMP0.U					= STM0_TIM0.B.STM31_0 + u32_ComparValue;
	STM0_ICR.B.CMP0EN			= CMP_EN_EVENT;

	/************************************************
	 * SUMMARY										*
	 * 	System Timer	: Free Run					*
	 * 	Base Period		: 100us						*
	 * 					 							*
	 ************************************************/
}

void phaseLockLoopInit(void) {

	UNLOCK_SAFETY_WDT;
	while(SCU_CCUCON1.B.LCK != PLL_UNLOCK_STS) {
		;	}
	SCU_CCUCON1.B.GTMDIV		= PLL_GTM_DIV_VAL;
	LOCK_SAFETY_WDT;

	UNLOCK_SAFETY_WDT;
	SCU_OSCCON.B.MODE			= PLL_OSC_MODE_SEL;
	SCU_OSCCON.B.OSCVAL			= PLL_OSC_DIV;
	SCU_CCUCON1.B.INSEL			= PLL_OSC_SRC_SEL;
	SCU_CCUCON1.B.UP			= PLL_CCUCON_REQ;
	LOCK_SAFETY_WDT;

	while( (SCU_OSCCON.B.PLLLV != PLL_ENABLE_OSC) || (SCU_OSCCON.B.PLLHV != PLL_ENABLE_OSC) ) {
		;	}

	UNLOCK_SAFETY_WDT;
	while(SCU_CCUCON1.B.LCK != PLL_UNLOCK_STS) {
		;	}
	SCU_PLLCON0.B.VCOBYP		= PLL_VCO_MODE_BYPASS;
	SCU_PLLCON0.B.SETFINDIS		= PLL_BLOCK_OSC;
	SCU_PLLCON0.B.NDIV			= PLL_SYNTHESIZER_N_VAL;
	SCU_PLLCON0.B.PDIV			= PLL_DIV_P_VAL;
	SCU_PLLCON0.B.PLLPWD		= PLL_PSMODE;
	SCU_PLLCON1.B.K2DIV			= PLL_DIV_K_VAL;
	SCU_PLLCON0.B.CLRFINDIS 	= PLL_PASS_OSC;
	while(SCU_CCUCON0.B.LCK != PLL_UNLOCK_STS) {
		;	}
	SCU_CCUCON0.B.CLKSEL		= PLL_CLK_SEL;
	SCU_CCUCON0.B.UP			= PLL_CCU_CONFIG_REQ;
	LOCK_SAFETY_WDT;

	while(SCU_PLLSTAT.B.VCOLOCK == PLL_VCO_STS_UNLOCK) {
		;	}

	UNLOCK_SAFETY_WDT;
	SCU_PLLCON0.B.VCOBYP		= PLL_VCO_MODE_NORMAL;
	LOCK_SAFETY_WDT;

	/************************************************
	 * SUMMARY										*
	 * 						N						*
	 * 	Frequency PLL = --------- * Frequency OSC	*
	 * 					  P * K						*
	 * 					  							*
	 * 	System Clock  = 200 MHz				  		*
	 *												*
	 ************************************************/
}

/*void encoderCountReset(int dummy) {
	//if ( Core Timer 3(T3) == Overflow or Underflow)
	//	reset T3
	//endif
}*/

void rpm_Front(void) {
	Front_cnt = Front_cnt + 1;
}

enccnt getFrontCnt(void) {
	return Front_cnt;
}


void BaseTick(uint32 nPeriodValue) {
	STM0_CMP0.U = STM0_CMP0.U + nPeriodValue;
	Scheduler();
	DMS_cnt = DMS_cnt+1;// DMS_cnt가 1일때  0.1ms
	if(DMS_cnt>=DMS_FREQ) // DMS_count가 5ms 이상일때
	{
		DMS_filter();
		DMS_cnt=0;
	}
}
