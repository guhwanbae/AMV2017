/*
 * imageProcess.h
 *
 *  Created on: 2017. 6. 27.
 *      Author: ghbae
 */

#ifndef IMAGEPROCESS_H_
#define IMAGEPROCESS_H_

/*******************************************************************************
* Include
*******************************************************************************/
#include "typedefinition.h"
#include "cameraController.h"
#include "queueADT.h"

/*******************************************************************************
* Constant
*******************************************************************************/


/*******************************************************************************
* Define & MACRO
*******************************************************************************/
#define ABS(x)						(((x)<0)?-(x):(x))
#define DISTANCE_MANHATTAN(x,y)		(((x)<0)?(-(x)):(x))+(((y)<0)?(-(y)):(y))

/*******************************************************************************
* Type Definition
*******************************************************************************/

/*******************************************************************************
* Exported Global Variables
*******************************************************************************/


/*******************************************************************************
* Member Functions
*******************************************************************************/

/*SMOOTHING*/
void median2D(int width, int height, pixel (*img)[width], pixel (*result)[width]);
void gaussian(int width, int height, pixel (*img)[width], pixel (*result)[width]);

/*GRADIENT*/
void gradient(int width, int height, pixel (*img)[width], pixel (*result)[width], edge (*direction)[width]);

/*FIND EXTREMA*/
void nonMaximumSurpression(int width, int height, pixel (*img)[width], pixel (*result)[width]);
void hysteresisThresholding(int width, int height, pixel (*img)[width], pixel (*result)[width], edge (*direction)[width], int enQCnt);

/*VERSATILE FUNCTION*/
void median(pixel* arr, int start, int end);
pixel medianSort(pixel* arr, int start, int end);
int medianSortStruct(int* arr,int start, int end);
/*LINEAR PROCESS*/
void linearSmoothing(pixel* arr, pixel* result, int end);
pixel* linearMedian(pixel* arr, pixel* result, int end);
pixel* linearGaussian(pixel* arr, pixel* result, int end);
pixel* linearGradient(pixel* arr, pixel* result, int end);
pixel* linearNonMaximumSurpression(pixel* arr, pixel* result, int end);
pixel* linearDoubleThresholding(pixel* arr, pixel* result, int end);

/*UNIT CONV*/
double raidanToDegree(double degree);
double degreeToRadian(double radian);

#endif /* IMAGEPROCESS_H_ */
