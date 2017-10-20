/*
 * cameraController.c
 *
 *  Created on: 2017. 6. 26.
 *      Author: ghbae
 */

/*******************************************************************************
 * Include
 *******************************************************************************/
#include "global.h"
#include "typedefinition.h"
#include "cameraController.h"
#include "vehicleController.h"
#include "timerController.h"
#include "imageProcess.h"
#include "Schedular.h"
/*******************************************************************************
 * Constant
 *******************************************************************************/

/*******************************************************************************
 * Define & MACRO
 *******************************************************************************/
#define SET_VALUE_LEFT_CAM_SI_PERIOD	GTM_TOM0_CH15_SR0.U
#define SET_VALUE_LEFT_CAM_SI_DUTY		GTM_TOM0_CH15_SR1.U
#define SET_VALUE_LEFT_CAM_CLK_PERIOD	GTM_TOM0_CH11_SR0.U
#define SET_VALUE_LEFT_CAM_CLK_DUTY		GTM_TOM0_CH11_SR1.U

#define SET_VALUE_RIGHT_CAM_SI_PERIOD	GTM_TOM0_CH6_SR0.U
#define SET_VALUE_RIGHT_CAM_SI_DUTY		GTM_TOM0_CH6_SR1.U
#define SET_VALUE_RIGHT_CAM_CLK_PERIOD	GTM_TOM0_CH0_SR0.U
#define SET_VALUE_RIGHT_CAM_CLK_DUTY	GTM_TOM0_CH0_SR1.U
/*******************************************************************************
 * Type Definition
 *******************************************************************************/

/*******************************************************************************
 * Global
 *******************************************************************************/
cameraStruct camera;
cameraStruct2D camera2D;

int school_cnt_in=0;
int school_cnt_out=0;
int whiteline_check=0;
int school_cnt_out2=0;
extern int schoolzone_state;
extern int Hill_state;
extern int Hill_cnt1;
extern int Hill_cnt2;

//extern vehicleControl vehicle;
/*******************************************************************************
 * Member Functions
 *******************************************************************************/
void cameraInit(void) {
	//cameraStructInit();
	cameraStructInit2D();
}

void cameraStructInit(void) {
	camera.input.pixelInputCnt = 0;
	int index;
	for(index=0; index<CAMERA_INPUT_LENGTH; ++index) {
		camera.input.inputPixelArr[index] = 0;
	}
}

void cameraStructInit2D(void) {
	camera2D.input.inputHeight = IMAGE_HEIGHT - 1;
	camera2D.input.inputWidthCnt = 0;
	camera2D.result.avgDir = 0;
	int h,w;
	for(h=0; h<IMAGE_HEIGHT; ++h) {
		for(w=0; w<IMAGE_WIDTH; ++w) {
			camera2D.input.inputImageArr[h][w] = 0;
		}
	}
}

void cameraClock(int periodSI, int dutySI, int periodCLK, int dutyCLK) {
	// Camera 1 SI (CAM LEFT)
	SET_VALUE_LEFT_CAM_SI_PERIOD	= periodSI; //Camera1_SI_Period
	SET_VALUE_LEFT_CAM_SI_DUTY		= dutySI;	//Camera1_SI_Duty

	// Camera 1 CLK (CAM LEFT)
	SET_VALUE_LEFT_CAM_CLK_PERIOD	= periodCLK;	//Camera1_CLK_Period
	SET_VALUE_LEFT_CAM_CLK_DUTY		= dutyCLK;		//Camera1_CLK_Duty

	// Camera 2 SI  (CAM RIGHT)
	SET_VALUE_RIGHT_CAM_SI_PERIOD	= periodSI;	//Camera2_SI_Period
	SET_VALUE_RIGHT_CAM_SI_DUTY		= dutySI;	//Camera2_SI_Duty

	// Camera 2 CLK (CAM RIGHT)
	SET_VALUE_RIGHT_CAM_CLK_PERIOD	= periodCLK;	//Camera2_CLK_Period
	SET_VALUE_RIGHT_CAM_CLK_DUTY	= dutyCLK;		//Camera2_CLK_Duty
}

void getPixel(int dummy) {
   if(camera.input.pixelInputCnt <= THRESHOLD_LEFT_ONLY) { // left only
      camera.input.inputPixelArr[camera.input.pixelInputCnt++] = READ_LEFT_CAMERA_PIXEL_FROM_ADC;
   }
   else if(camera.input.pixelInputCnt <= THRESHOLD_BOTH) // both
   {
      camera.input.inputPixelArr[camera.input.pixelInputCnt] = READ_LEFT_CAMERA_PIXEL_FROM_ADC;
      camera.input.inputPixelArr[(camera.input.pixelInputCnt)+SHIFT_PIXEL_INDEX] = READ_RIGHT_CAMERA_PIXEL_FROM_ADC;
      camera.input.pixelInputCnt++;
   }
   else if(camera.input.pixelInputCnt <= THRESHOLD_RIGHT_ONLY) {//right only
      camera.input.inputPixelArr[(camera.input.pixelInputCnt)+SHIFT_PIXEL_INDEX] = READ_RIGHT_CAMERA_PIXEL_FROM_ADC;
      camera.input.pixelInputCnt++;
   }
   else {
      return;
   }
}

void checkLinearProcess(int dummy) {
	linearImgProcess();

	/*	RESET CAMERA INPUT COUNT	*/
	camera.input.pixelInputCnt = 0;
}

void linearImgProcess(void) {
	pixel* inputArr = camera.input.inputPixelArr;
	pixel* resultArr = camera.result.resultProcessArr;

	/*	SMOOTHING? : noise removal algorithm
	 *	MEDIAN? : remove impulse noise.
	 * 	GAUSSIAN? : remove random noise. */
	linearSmoothing(inputArr,resultArr,CAMERA_INPUT_LENGTH-1);

	inputArr = resultArr;
	resultArr = camera.input.inputPixelArr;

	/*	GRADIENT? : differentiate signal. find edge.
	 *	SOBEL MASK? : differential operator in discrete. */
	linearGradient(inputArr,resultArr,CAMERA_INPUT_LENGTH-1);

	inputArr = resultArr;
	resultArr = camera.result.resultProcessArr;

	/*	NON-MAXIMUM-SURPRESSION? : 	*/
	linearNonMaximumSurpression(inputArr,resultArr,CAMERA_INPUT_LENGTH-1);

	inputArr = resultArr;
	resultArr = camera.result.resultProcessArr;

	/*	DOUBLE-THRESHOLDING? :	*/
	//linearDoubleThresholding(inputArr,resultArr,CAMERA_INPUT_LENGTH-1);
}

void buildFrame(int dummy) {
	if(camera2D.input.inputHeight >= 0 && camera2D.input.inputWidthCnt < FALLING_TIME) {

			if(camera2D.input.inputWidthCnt <= RISING_TIME){
				camera2D.input.inputWidthCnt++;
			}
		else if(camera2D.input.inputWidthCnt <= THRESHOLD_LEFT_ONLY) {
				camera2D.input.inputImageArr[camera2D.input.inputHeight][(camera2D.input.inputWidthCnt++)-(RISING_TIME+1)] = READ_LEFT_CAMERA_PIXEL_FROM_ADC;
			}
			else if(camera2D.input.inputWidthCnt <= THRESHOLD_BOTH) {
				camera2D.input.inputImageArr[camera2D.input.inputHeight][camera2D.input.inputWidthCnt-(RISING_TIME+1)] = READ_LEFT_CAMERA_PIXEL_FROM_ADC;
				camera2D.input.inputImageArr[camera2D.input.inputHeight][(camera2D.input.inputWidthCnt)+SHIFT_PIXEL_INDEX-(RISING_TIME+1)] = READ_RIGHT_CAMERA_PIXEL_FROM_ADC;
				camera2D.input.inputWidthCnt++;
			}
			else if(camera2D.input.inputWidthCnt <= THRESHOLD_RIGHT_ONLY) {
				camera2D.input.inputImageArr[camera2D.input.inputHeight][(camera2D.input.inputWidthCnt)+SHIFT_PIXEL_INDEX-(RISING_TIME+1)] = READ_RIGHT_CAMERA_PIXEL_FROM_ADC;
				camera2D.input.inputWidthCnt++;
			}
			else if(camera2D.input.inputWidthCnt <= FALLING_TIME)
			{
				camera2D.input.inputWidthCnt++;
			}
		}
}

void check2DProcess(int dummy) {

	//Infrared sensor check
	camera2D.result.Infraredval = VADC_G1_RES4.B.RESULT;
	if(camera2D.result.Infraredval < 1000 && Hill_state==0 && Hill_cnt1==0)  //°ËÀº
		camera2D.result.Infaredcnt++;

	if(camera2D.result.Infraredval > 3000 && camera2D.result.Infraredval < 5000)  //Èò»ö
	{
		camera2D.result.Infaredcnt =0;
		whiteline_check=1;
	}
	if(camera2D.result.Infaredcnt!=0 && schoolzone_state==0 && school_cnt_out2==0)
	{
		school_cnt_in=1;
		whiteline_check=0;
	}
	if(camera2D.result.Infaredcnt>10 && schoolzone_state==1 && whiteline_check==1)
	{
		school_cnt_out=1;
		school_cnt_out2=1;
	}

	if (camera2D.input.inputHeight == 0) {
		imgProcess2D();

		updateDrivingStatus();

		drivingControl();
		camera2D.input.inputHeight = IMAGE_HEIGHT - 1;
		camera2D.input.inputWidthCnt = 0;

		if (Hill_cnt1 == 1) {
			Hill_cnt2++;
			if (Hill_cnt2 >= 25) {
				Hill_cnt2 = 0;
				Hill_cnt1 = 0;
			}
		}
	}
	else {
		camera2D.input.inputHeight--;
		camera2D.input.inputWidthCnt = 0;
	}

}

void imgProcess2D(void) {
	/*	SMOOTHING? : noise removal algorithm
	 *	MEDIAN? : remove impulse noise.
	 * 	GAUSSIAN? : remove random noise. */
	//input(temp) : inputImageArr, result(temp) : edgeVal
	median2D(IMAGE_WIDTH,IMAGE_HEIGHT,camera2D.input.inputImageArr,camera2D.result.edgeVal);
	//gaussian(IMAGE_WIDTH,IMAGE_HEIGHT,camera2D.input.inputImageArr,camera2D.result.edgeVal);

	/*	GRADIENT? : differentiate signal. find edge.
	 *	SOBEL MASK? : differential operator in discrete. */
	//input(temp) : edgeVal, result(temp) : inputImageArr
	gradient(IMAGE_WIDTH,IMAGE_HEIGHT,camera2D.result.edgeVal,camera2D.input.inputImageArr,camera2D.result.edgeDir);

	/*	NON-MAXIMUM-SURPRESSION? : 	*/
	//input(temp) : inputImageArr, result(temp) : edgeVal
	nonMaximumSurpression(IMAGE_WIDTH,IMAGE_HEIGHT,camera2D.input.inputImageArr,camera2D.result.edgeVal);

	/*	DOUBLE-THRESHOLDING? :	*/
	//input(temp) : edgeVal, result(complete) : edgeVal
	//hysteresisThresholding(IMAGE_WIDTH,IMAGE_HEIGHT,camera2D.result.edgeVal,camera2D.result.edgeVal);//input(temp) : edgeVal, result(temp) : inputImageArr
	hysteresisThresholding(IMAGE_WIDTH,IMAGE_HEIGHT,camera2D.result.edgeVal,camera2D.result.edgeVal,camera2D.result.edgeDir, camera2D.result.enQCnt);//input(temp) : edgeVal, result(temp) : inputImageArr
}

extern unsigned short lcdImage[IMAGE_HEIGHT][IMAGE_WIDTH];

unsigned short* getGrayFrame(unsigned short* lcdImage) {

	int h,w;
	pixel gray;
	const int adcScale = 1000;
	const int grayscale = 255;

	for(h=0; h<IMAGE_HEIGHT; ++h) {
		for(w=0; w<IMAGE_WIDTH; ++w) {
			/*	scaling	*/
			gray = ( (camera2D.input.inputImageArr[h][w]) / adcScale ) * grayscale;
			if(gray >= 255) {
				gray = 255;
			}
			else if(gray <= 0) {
				gray = 0;
			}
			/*	end scaling	*/
			lcdImage[(h*IMAGE_WIDTH) + w] = gray;
		}
	}

	/*
	 *	GLCD_clear(COLOR_BLUE);
	 *	GLCD_bitmap( 25, 25, IMAGE_WIDTH, IMAGE_HEIGHT, lcdImage);
	 *
	 * 	*/

	return lcdImage;
}
