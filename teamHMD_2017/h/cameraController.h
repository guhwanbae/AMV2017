/*
 * cameraController.h
 *
 *  Created on: 2017. 6. 26.
 *      Author: ghbae
 */

#ifndef CAMERACONTROLLER_H_
#define CAMERACONTROLLER_H_

/*******************************************************************************
 * Include
 *******************************************************************************/
#include "typedefinition.h"
#include "vehicleController.h"
#include "imageProcess.h"

/*******************************************************************************
 * Constant
 *******************************************************************************/
/*	CAMERA CLOCK	*/
#define CAM_PERIOD_CLK		78
#define CAM_DUTY_CLK		39
#define CAM_PERIOD_SI		11718
#define CAM_DUTY_SI			78

#define CAM_FREQ_CLK		20032.1
#define CAM_FREQ_SI			133.3
#define CAM_INV_FREQ_CLK	4.992e-5
#define CAM_INV_FREQ_SI		7.5e-3

/*	CAMERA PARAMETERS	*/
#define	CAM_PARAMETER_HEIGHT	30
//#define CAM_PARAMETER_TILT			not yet!
//#define CAM_PARAMETER_INTERVAL			not yet!
#define CAM_PARAMETER_DISTANCE		60
#define CAM_PARAMETER_LENGTH_VIEW	70




/*******************************************************************************
 * Define & MACRO
 *******************************************************************************/

/*	CAMERA PIN DEFINE	*/
#define DEFINE_PORT_CAMLEFT_SI			P00_IOCR8.B.PC8=0b10001	//P00.8 Camera1 SI   alt1   tom0_15      tout17
#define DEFINE_PORT_CAMLEFT_CLK			P00_IOCR4.B.PC4=0b10001	//P00.4 Camera1 CLK   alt1   tom0_11      tout13

#define DEFINE_PORT_CAMRIGHT_SI			P33_IOCR0.B.PC2=0b10001		//P33.2 Camera2 SI   alt1   tom0_6      tout24
#define DEFINE_PORT_CAMRIGHT_CLK		P15_IOCR4.B.PC6=0b10001		//P15.6 Camera2 CLK   alt1   tom0_0      tout77

/*	CAMERA PIN READ	*/
#define READ_LEFT_CAMERA_PIXEL_FROM_ADC		VADC_G0_RES11.B.RESULT //read from vadc
#define READ_RIGHT_CAMERA_PIXEL_FROM_ADC	VADC_G0_RES9.B.RESULT //read from vadc

/*	CAMERA INPUT PARAMETERS	*/
/*   CAMERA INPUT PIXEL LENGTH = IMAGE WIDTH
 *   = THRESHOLD_RIGHT_ONLY + SHIFT_PIXEL_INDEX + 1.   */
#define CAMERA_INPUT_LENGTH      157

#define THRESHOLD_LEFT_ONLY      30
#define THRESHOLD_BOTH         79
#define THRESHOLD_RIGHT_ONLY   117
#define SHIFT_PIXEL_INDEX      49

#define IMAGE_WIDTH            157
#define IMAGE_HEIGHT         10
#define RISING_TIME          9
#define FALLING_TIME          127
/*******************************************************************************
 * Type Definition
 *******************************************************************************/

/*******************************************************************************
 * Member
 *******************************************************************************/

/*	CAMERA DATA STRUCTURES	*/
/*	LINEAR	*/
typedef struct cameraInput {
	pixel inputPixelArr[CAMERA_INPUT_LENGTH];
	pixel pixelInputCnt;
}cameraInput;

typedef struct imageProcess {
	pixel resultProcessArr[CAMERA_INPUT_LENGTH];
}imageProcess;

typedef struct cameraStruct {
	cameraInput input;
	imageProcess result;
}cameraStruct;

/*	2D	*/
typedef struct imageInput2D {
	pixel inputImageArr[IMAGE_HEIGHT][IMAGE_WIDTH];
	int inputHeight;
	int inputWidthCnt;
}imageInput2D;

typedef struct imageProcess2D {
	pixel edgeVal[IMAGE_HEIGHT][IMAGE_WIDTH];
	edge edgeDir[IMAGE_HEIGHT][IMAGE_WIDTH];
	edge avgDir;
	int enQCnt;
	int Infraredval;
	int Infaredcnt;
}imageProcess2D;

typedef struct cameraStruct2D {
 imageInput2D input;
 imageProcess2D result;
}cameraStruct2D;

/*******************************************************************************
 * Member Functions
 *******************************************************************************/
void cameraInit(void);
void cameraStructInit(void);
void cameraStructInit2D(void);
void cameraClock(int periodSI, int dutySI, int periodCLK, int dutyCLK);

void getPixel(int dummy);
void checkLinearProcess(int dummy);

void buildFrame(int dummy);
void check2DProcess(int dummy);

void linearImgProcess(void);
void imgProcess2D(void);

void setCameraData(angleRadianType leftEdgeAngle, angleRadianType rightEdgeAngle, lengthType calcedWidth, lengthType calibratedWidth);

/*	view lcd	*/
unsigned short* getGrayFrame(unsigned short* lcd);

#endif /* CAMERACONTROLLER_H_ */
