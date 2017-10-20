/*
 * timerOutputModule.c
 *
 *  Created on: 2017. 6. 26.
 *      Author: ghbae
 */

/*******************************************************************************
 * Include
 *******************************************************************************/
#include "global.h"
#include "timerController.h"
#include "cameraController.h"
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

/*******************************************************************************
 * Exported Global Variables
 *******************************************************************************/

/*******************************************************************************
 * Memeber Functions
 *******************************************************************************/

void timerOutputModuleInit(void) {

	DEFINE_PORT_CAMLEFT_CLK;
	DEFINE_PORT_CAMLEFT_SI;
	DEFINE_PORT_CAMRIGHT_CLK;
	DEFINE_PORT_CAMRIGHT_SI;

	DEFINE_PORT_SERVO;		//Choose Port00.12 as alt 1		tom0_3 Servo
	DEFINE_PORT_MOTORDRIVE_AINH;	//motor
	DEFINE_PORT_MOTORDRIVE_AIN;	//motor
	DEFINE_PORT_MOTORDRIVE_BINH;	//tom 0_1 motor
	DEFINE_PORT_MOTORDRIVE_BIN;	//tom 0_2 motor

	P13_IOCR0.B.PC0=0b10000;
	P13_IOCR0.B.PC1=0b10000;
	P13_IOCR0.B.PC2=0b10000;
	P13_IOCR0.B.PC3=0b10000;
	P13_OMR.B.PCL0=0;
	P13_OMR.B.PS0=1;
	P13_OMR.B.PCL1=0;
	P13_OMR.B.PS1 = 1;
	P13_OMR.B.PCL2 = 0;
	P13_OMR.B.PS2 = 1;
	P13_OMR.B.PCL3 = 0;
	P13_OMR.B.PS3 = 1;


	UNLOCK_WDT_CON;
	GTM_CLC.B.DISR = 0;
	while (GTM_CLC.B.DISS == 1) {
		;
	}
	LOCK_WDT_CON;

	 //CMU Clock Enable Register, Enable clock source 0 and Enable Fixed Clock Generation
	      GTM_CMU_CLK_EN.B.EN_FXCLK = 0b01;
	      GTM_CMU_FXCLK_CTRL.B.FXCLK_SEL = 0b0001;
	      GTM_CMU_CLK_EN.B.EN_FXCLK = 0b10;
	      GTM_CMU_CLK_EN.B.EN_CLK0 = 0b10;

	      GTM_TOUTSEL1.B.SEL1 = 0b00;    //Timer Output Select Camera 1 SI TIMER A
	      GTM_TOUTSEL0.B.SEL13 = 0b00;    //Timer Output Select Camera 1 CLK TIMER A

	      GTM_TOUTSEL1.B.SEL8 = 0b00;      //Timer Output Select Camera 2 SI TIMER A
	      GTM_TOUTSEL4.B.SEL13 = 0b00;      //Timer Output Select Camera 2 CLK TIMER A

	      GTM_TOUTSEL2.B.SEL0 = 0b01;      //Timer Output Select Servo TIMER B

	      GTM_TOUTSEL0.B.SEL1 = 0b01;      //Timer Output Select No.1 Motor TIMER B
	      GTM_TOUTSEL0.B.SEL12 = 0b01;      //Timer Output Select No.2 Motor TIMER B

	      //TGCi global control register
	      GTM_TOM0_TGC1_GLB_CTRL.B.UPEN_CTRL7 = 0b10;         //Camera 1 SI
	      GTM_TOM0_TGC1_GLB_CTRL.B.UPEN_CTRL3 = 0b10;         //Camera 1 CLK

	      GTM_TOM0_TGC0_GLB_CTRL.B.UPEN_CTRL6 = 0b10;         //Camera 2 SI
	      GTM_TOM0_TGC0_GLB_CTRL.B.UPEN_CTRL0 = 0b10;         //Camera 2 CLK


	      GTM_TOM1_TGC0_GLB_CTRL.B.UPEN_CTRL0 = 0b10;         //Servo

	      GTM_TOM1_TGC1_GLB_CTRL.B.UPEN_CTRL1 = 0b10;         //No.1 Motor IN
	      GTM_TOM1_TGC0_GLB_CTRL.B.UPEN_CTRL2 = 0b10;         //No.2 Motor IN

	      //TGCi control/status register
	      GTM_TOM0_TGC1_ENDIS_CTRL.B.ENDIS_CTRL7 = 0b10; // Camera 1 SI
	      GTM_TOM0_TGC1_ENDIS_STAT.B.ENDIS_STAT7 = 0b10; // Camera 1 SI

	      GTM_TOM0_TGC1_OUTEN_CTRL.B.OUTEN_CTRL7 = 0b10; // Camera 1 SI
	      GTM_TOM0_TGC1_OUTEN_STAT.B.OUTEN_STAT7 = 0b10; // Camera 1 SI

	      GTM_TOM0_TGC1_ENDIS_CTRL.B.ENDIS_CTRL3 = 0b10; // Camera 1 CLK
	      GTM_TOM0_TGC1_ENDIS_STAT.B.ENDIS_STAT3 = 0b10; // Camera 1 CLK

	      GTM_TOM0_TGC1_OUTEN_CTRL.B.OUTEN_CTRL3 = 0b10; // Camera 1 CLK
	      GTM_TOM0_TGC1_OUTEN_STAT.B.OUTEN_STAT3 = 0b10; // Camera 1 CLK

	      GTM_TOM0_TGC0_ENDIS_CTRL.B.ENDIS_CTRL6 = 0b10; // Camera 2 SI
	      GTM_TOM0_TGC0_ENDIS_STAT.B.ENDIS_STAT6 = 0b10; // Camera 2 SI

	      GTM_TOM0_TGC0_OUTEN_CTRL.B.OUTEN_CTRL6 = 0b10; // Camera 2 SI
	      GTM_TOM0_TGC0_OUTEN_STAT.B.OUTEN_STAT6 = 0b10; // Camera 2 SI

	      GTM_TOM0_TGC0_ENDIS_CTRL.B.ENDIS_CTRL0 = 0b10; // Camera 2 CLK
	      GTM_TOM0_TGC0_ENDIS_STAT.B.ENDIS_STAT0 = 0b10; // Camera 2 CLK

	      GTM_TOM0_TGC0_OUTEN_CTRL.B.OUTEN_CTRL0 = 0b10; // Camera 2 CLK
	      GTM_TOM0_TGC0_OUTEN_STAT.B.OUTEN_STAT0 = 0b10; // Camera 2 CLK

	      GTM_TOM1_TGC0_ENDIS_CTRL.B.ENDIS_CTRL0 = 0b10;   //Servo
	      GTM_TOM1_TGC0_ENDIS_STAT.B.ENDIS_STAT0 = 0b10;   //Servo

	      GTM_TOM1_TGC0_OUTEN_CTRL.B.OUTEN_CTRL0 = 0b10;   //Servo
	      GTM_TOM1_TGC0_OUTEN_STAT.B.OUTEN_STAT0 = 0b10;   //Servo

	      GTM_TOM1_TGC1_ENDIS_CTRL.B.ENDIS_CTRL1 = 0b10;    //No.1 Motor IN
	      GTM_TOM1_TGC1_ENDIS_STAT.B.ENDIS_STAT1 = 0b10;   //No.1 Motor IN

	      GTM_TOM1_TGC1_OUTEN_CTRL.B.OUTEN_CTRL1 = 0b10;   //No.1 Motor IN
	      GTM_TOM1_TGC1_OUTEN_STAT.B.OUTEN_STAT1 = 0b10;    //No.1 Motor IN

	      GTM_TOM1_TGC0_ENDIS_CTRL.B.ENDIS_CTRL2 = 0b10;   //No.2 Motor IN
	      GTM_TOM1_TGC0_ENDIS_STAT.B.ENDIS_STAT2 = 0b10;    //No.2 Motor IN

	      GTM_TOM1_TGC0_OUTEN_CTRL.B.OUTEN_CTRL2 = 0b10;    //No.2 Motor IN
	      GTM_TOM1_TGC0_OUTEN_STAT.B.OUTEN_STAT2 = 0b10;    //No.2 Motor IN


	      // TOMi TGC0 Force Update Control Register
	      GTM_TOM0_TGC1_FUPD_CTRL.B.FUPD_CTRL7 = 0b10; // Camera 1 SI
	      GTM_TOM0_TGC1_FUPD_CTRL.B.RSTCN0_CH7 = 0b10; // Camera 1 SI

	      GTM_TOM0_TGC1_FUPD_CTRL.B.FUPD_CTRL3 = 0b10; // Camera 1 CLK
	      GTM_TOM0_TGC1_FUPD_CTRL.B.RSTCN0_CH3 = 0b10; // Camera 1 CLK

	      GTM_TOM0_TGC0_FUPD_CTRL.B.FUPD_CTRL6 = 0b10; // Camera 2 SI
	      GTM_TOM0_TGC0_FUPD_CTRL.B.RSTCN0_CH6 = 0b10; // Camera 2 SI

	      GTM_TOM0_TGC0_FUPD_CTRL.B.FUPD_CTRL0 = 0b10; // Camera 2 CLK
	      GTM_TOM0_TGC0_FUPD_CTRL.B.RSTCN0_CH0 = 0b10; // Camera 2 CLK

	      // TOMi TGC0 Force Update Control Register
	      GTM_TOM1_TGC0_FUPD_CTRL.B.RSTCN0_CH0 = 0b10;   //Servo
	      GTM_TOM1_TGC0_FUPD_CTRL.B.FUPD_CTRL0 = 0b10;   //Servo

	      GTM_TOM1_TGC1_FUPD_CTRL.B.RSTCN0_CH1 = 0b10;   //No.1 Motor IN
	      GTM_TOM1_TGC1_FUPD_CTRL.B.FUPD_CTRL1 = 0b10;   //No.1 Motor IN

	      GTM_TOM1_TGC0_FUPD_CTRL.B.RSTCN0_CH2 = 0b10;   //No.2 Motor IN
	      GTM_TOM1_TGC0_FUPD_CTRL.B.FUPD_CTRL2 = 0b10;   //No.2 Motor IN

	      //TOM0 Channel x Control Register ,000B CMU_FXCLK(0) selected: clock selected by FXCLKSEL
	      // Camera 1 SI
	      GTM_TOM0_CH15_CTRL.B.TRIGOUT = 1;
	      GTM_TOM0_CH15_CTRL.B.RST_CCU0 = 0;         //CM0이랑 같아지면 리셋
	      GTM_TOM0_CH15_CTRL.B.CLK_SRC_SR = 0b001;   //Clock source select  CMU_CLK0/16
	      GTM_TOM0_CH15_CTRL.B.SL = 1;

	      // Camera 1 CLK
	      GTM_TOM0_CH11_CTRL.B.TRIGOUT = 1;
	      GTM_TOM0_CH11_CTRL.B.RST_CCU0 = 0;
	      GTM_TOM0_CH11_CTRL.B.CLK_SRC_SR = 0b001;   // CMU_CLK0/16
	      GTM_TOM0_CH11_CTRL.B.SL = 1;

	      // Camera 2 SI
	      GTM_TOM0_CH6_CTRL.B.TRIGOUT = 1;
	      GTM_TOM0_CH6_CTRL.B.RST_CCU0 = 0;
	      GTM_TOM0_CH6_CTRL.B.CLK_SRC_SR = 0b001;      // CMU_CLK0/16
	      GTM_TOM0_CH6_CTRL.B.SL = 1;

	      // Camera 2 CLK
	      GTM_TOM0_CH0_CTRL.B.TRIGOUT = 1;
	      GTM_TOM0_CH0_CTRL.B.RST_CCU0 = 0;
	      GTM_TOM0_CH0_CTRL.B.CLK_SRC_SR = 0b001;      // CMU_CLK0/16
	      GTM_TOM0_CH0_CTRL.B.SL = 1;

	      //Servo
	      GTM_TOM1_CH0_CTRL.B.TRIGOUT = 1;
	      GTM_TOM1_CH0_CTRL.B.RST_CCU0 = 0;
	      GTM_TOM1_CH0_CTRL.B.CLK_SRC_SR = 0b001;      // CMU_CLK0/16
	      GTM_TOM1_CH0_CTRL.B.SL = 1;

	      ////No.1 Motor IN
	      GTM_TOM1_CH9_CTRL.B.TRIGOUT = 1;
	      GTM_TOM1_CH9_CTRL.B.RST_CCU0 = 0;
	      GTM_TOM1_CH9_CTRL.B.CLK_SRC_SR = 0b001;      // CMU_CLK0/16
	      GTM_TOM1_CH9_CTRL.B.SL = 1;

	      //No.2 Motor IN
	      GTM_TOM1_CH2_CTRL.B.TRIGOUT = 1;
	      GTM_TOM1_CH2_CTRL.B.RST_CCU0 = 0;
	      GTM_TOM1_CH2_CTRL.B.CLK_SRC_SR = 0b001;      // CMU_CLK0/16
	      GTM_TOM1_CH2_CTRL.B.SL = 1;

	      //Interrupt
	      GTM_TOM0_CH15_IRQ_MODE.B.IRQ_MODE = 0b10;   // Camera 1 SI

	      GTM_TOM0_CH11_IRQ_MODE.B.IRQ_MODE = 0b10;   // Camera 1 CLK

	      GTM_TOM0_CH15_IRQ_EN.B.CCU0TC_IRQ_EN = 1;   // Camera 1 SI

	      GTM_TOM0_CH11_IRQ_EN.B.CCU0TC_IRQ_EN = 1;   // Camera 1 CLK

	      GTM_TOM0_CH6_IRQ_MODE.B.IRQ_MODE = 0b10;   // Camera 2 SI

	      GTM_TOM0_CH0_IRQ_MODE.B.IRQ_MODE = 0b10;   // Camera 2 CLK

	      GTM_TOM0_CH6_IRQ_EN.B.CCU0TC_IRQ_EN = 1;   // Camera 2 SI

	      GTM_TOM0_CH0_IRQ_EN.B.CCU0TC_IRQ_EN = 1;   // Camera 2 CLK

}
