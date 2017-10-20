/*
 * imageProcess.c
 *
 *  Created on: 2017. 6. 27.
 *      Author: ghbae
 */

/*******************************************************************************
* Include
*******************************************************************************/
#include "imageProcess.h"
#include "global.h"
#include "math.h"

/*******************************************************************************
* Constant
*******************************************************************************/


/*******************************************************************************
* Define & MACRO
*******************************************************************************/
#define SIZE_3BY3				9
#define SIZE_5BY5				25

#define MEDIAN_SIZE				5

/*	pixel scale ref = 255(8bit grayscale) or 4095(12bit adc)	*/
/*	ref scale : 255(8bit grayscale)	*/
#define EDGE_LOW_THRESHOLD		600
#define EDGE_HIGH_THRESHOLD		900

#define EDGE_STRONG				255
#define EDGE_WEAK				127
#define EDGE_REMOVED			0

#define LINEAR_EDGE_LOW_THRESHOLD		100
#define LINEAR_EDGE_HIGH_THRESHOLD		200

#define LINEAR_EDGE_STRONG				255
#define LINEAR_EDGE_WEAK				127
#define LINEAR_EDGE_REMOVED				0

#define THRESHOLD_ARC_TANGENT 10

/*	MEDIAN	*/
#define INPUT_MEDIAN_SIZE 14

/*	GAUSSIAN MASK	*/
/*	3x3 Mask	*/
const int gFilter[3][3] = { {  1,  2,  1},
							{  2,  4,  2},
							{  1,  2,  1} };
const int gaussianSize	= 16;
#define INV_GAUSSIAN_SIZE 0.0625

/*	5x5 Mask	*/
/*
 const int gFilter[5][5] = { {  2,  4,  5,  4,  2},
							 {  4,  9, 12,  9,  4},
							 {  5, 12, 15, 12,  5},
							 {  4,  9, 12,  9,  4},
							 {  2,  4,  5,  4,  2} };
const int gaussianSize	= 159;
#define INV_GAUSSIAN_SIZE = 0.006289;
 */

/*	SOBEL OPERATOR'S MASK	*/
/*	xFilter : find Vertical Edge	*/
const int xFilter[3][3] = { { -1,  0,  1},
							{ -2,  0,  2},
							{ -1,  0,  1} };
/*	yFilter : find Horizon Edge	*/
const int yFilter[3][3] = { {  1,  2,  1},
							{  0,  0,  0},
							{ -1, -2, -1} };
/*******************************************************************************
* Type Definition
*******************************************************************************/

/*******************************************************************************
* Exported Global Variables
*******************************************************************************/
//extern cameraStruct camera;
extern cameraStruct2D camera2D;
extern queue* driveQ;

/*******************************************************************************
* Member Functions
*******************************************************************************/


void median2D(int width, int height, pixel(*img)[width], pixel (*result)[width]) {
	int i,j,x,y;
	int index = 0;
	if(MEDIAN_SIZE == 3) {
		pixel arrTemp[SIZE_3BY3];
		for(i=0; i<height; ++i) {
			for(j=0; j<width; ++j) {
				/*	col 0, row 0 -> Exception Handling	*/

				for(y=-1; y<=1; ++y) {
					for(x=-1; x<=1; ++x) {
						arrTemp[index++] = img[i+y][j+x];
					}
				}
				result[i][j] = medianSort(arrTemp,0,SIZE_3BY3-1);
				index = 0;

				/*	col (last width), row (last row) -> Exception Handling	*/
			}
		}
		/*	col 0, row 0				->	Exception Handling	*/
		/*	col width-1, row height-1	->	Exception Handling	*/
		result[0][0]				= result[1][1];
		result[0][width-1]			= result[1][width-2];
		result[height-1][0]			= result[height-2][1];
		result[height-1][width-1]	= result[height-2][width-2];

		for(i=1; i<width-1; ++i) {
			result[0][i] = result[1][i];
			result[height-1][i] = result[height-2][i];
		}
		for(j=1;j<height-1; ++j) {
			result[j][0] = result[j][1];
			result[j][width-1] = result[j][width-2];
		}
	}
	else if(MEDIAN_SIZE == 5) {
		pixel arrTemp[SIZE_5BY5];
		for(i=2; i<height; ++i) {
			for(j=2; j<width; ++j) {
				for(x=-2; x<=2; ++x) {
					for(y=-2; y<=2; ++y) {
						arrTemp[index++] = img[i+y][j+x];
					}
				}
				result[i][j] = medianSort(arrTemp,0,SIZE_5BY5-1);
				index = 0;
			}
		}
		/*	col 0 and 1, row 0 and 1							->	Exception Handling	*/
		/*	col width-1 and width-2, row height-1 and height-2	->	Exception Handling	*/
		result[0][0]				= result[2][2];
		result[0][1]				= result[2][2];
		result[1][0]				= result[2][2];
		result[1][1]				= result[2][2];
		result[0][width-1]			= result[2][width-3];
		result[0][width-2]			= result[2][width-3];
		result[1][width-1]			= result[2][width-3];
		result[1][width-2]			= result[2][width-3];
		result[height-1][0]			= result[height-3][2];
		result[height-2][0]			= result[height-3][2];
		result[height-1][1]			= result[height-3][2];
		result[height-2][1]			= result[height-3][2];
		result[height-1][width-1]	= result[height-3][width-3];
		result[height-2][width-1]	= result[height-3][width-3];
		result[height-1][width-2]	= result[height-3][width-3];
		result[height-2][width-2]	= result[height-3][width-3];

		for(i=2; i<width-2; ++i) {
			result[0][i] = result[2][i];
			result[1][i] = result[2][i];
			result[height-1][i] = result[height-3][i];
			result[height-2][i] = result[height-3][i];
		}
		for(j=2;j<height-2; ++j) {
			result[j][0] = result[j][2];
			result[j][1] = result[j][2];
			result[j][width-1] = result[j][width-3];
			result[j][width-2] = result[j][width-3];
		}
	}
}

void median(pixel* arr, int start, int end) {
	pixel medianVal = medianSort(arr,start,end);
	int index;
	for(index=start; index<=end; ++index) {
		arr[index] = medianVal;
	}
}

pixel medianSort(pixel* arr, int start, int end) {
	int i,j,temp;
	for(i=start; i<=end; ++i) {
		temp = arr[i];
		for(j=i;j>0 && temp<arr[j-1]; --j) {
			arr[j] = arr[j-1];
		}
		arr[j] = temp;
	}
	return arr[(start+end) / 2];
}
int medianSortStruct(int* arr,int start, int end) {
	int i,j,temp;
		for(i=start; i<=end; ++i) {
			temp = arr[i];
			for(j=i;j>0 && temp<arr[j-1]; --j) {
				arr[j] = arr[j-1];
			}
			arr[j] = temp;
		}
		return arr[(int)(start+end) / 2];
	}

void gaussian(int width, int height, pixel(*img)[width], pixel (*result)[width]) {
	/*	Using 3x3 Gaussian Mask	*/
	int i,j,x,y;
	pixel sum,val;

	for(i=1; i<height-1; ++i) {
		for(j=1; j<width-1; ++j) {
			sum = 0;
			for(y=0; y<3; ++y) {
				for(x=0; x<3; ++x) {
					val = img[i+y-1][j+x-1];
					sum = sum + (val * gFilter[y][x]);
				}
			}
			result[i][j] = (double)sum * (double)INV_GAUSSIAN_SIZE;
		}
	}

	/*	col 0, row 0				->	Exception Handling	*/
	/*	col width-1, row height-1	->	Exception Handling	*/
	result[0][0]				= result[1][1];
	result[0][width-1]			= result[1][width-2];
	result[height-1][0]			= result[height-2][1];
	result[height-1][width-1]	= result[height-2][width-2];

	for(i=1; i<width-1; ++i) {
		result[0][i] = result[1][i];
		result[height-1][i] = result[height-2][i];
	}
	for(j=1;j<height-1; ++j) {
		result[j][0] = result[j][1];
		result[j][width-1] = result[j][width-2];
	}
}

void gradient(int width, int height, pixel (*img)[width], pixel (*result)[width], edge (*direction)[width]) {
	/*	Sobel Operation	*/
	pixel val, sumX, sumY;
	int i,j,x,y;
	double dir;

	for(i=1; i<height-1; ++i) {
		for(j=1; j<width-1; ++j) {
			sumX = 0;
			sumY = 0;
			for(y=0; y<3; ++y) {
				for(x=0; x<3; ++x) {
					val = img[i+y-1][j+x-1];
					sumX = sumX + (val * xFilter[y][x]);
					sumY = sumY + (val * yFilter[y][x]);
				}
			}
			/*	"val(result)" is gradient magnitude.	*/
			val = ABS(sumX) + ABS(sumY); //Use "Manhattan distance" or "Euclidean distacne".
			result[i][j] = val;

			/*	"direction" is angle of the gradient.	*/
			if( ABS(sumX) == 0 ) {
				direction[i][j] = 0;
			}
			else {
				dir = ( (double)sumY ) / ( (double)sumX );
				/*	funcion double atan(double) -> return type : double. (Unit : radian)	*/
				if( dir > 0 ) {
					direction[i][j] = -atan( dir );
				}
				else {
					direction[i][j] = -atan( dir );
				}
				//direction[i][j] = ABS( atan2( (double)ABS(sumX) , (double)ABS(sumY) ) );
			}
		}
	}

	/*	col 0, row 0				->	Exception Handling	*/
	/*	col width-1, row height-1	->	Exception Handling	*/
	result[0][0]				= result[1][1];
	result[0][width-1]			= result[1][width-2];
	result[height-1][0]			= result[height-2][1];
	result[height-1][width-1]	= result[height-2][width-2];

	direction[0][0]					= direction[1][1];
	direction[0][width-1]			= direction[1][width-2];
	direction[height-1][0]			= direction[height-2][1];
	direction[height-1][width-1]	= direction[height-2][width-2];

	for(i=1; i<width-1; ++i) {
		result[0][i] = result[1][i];
		result[height-1][i] = result[height-2][i];
		direction[0][i] = direction[1][i];
		direction[height-1][i] = direction[height-2][i];
	}
	for(j=1;j<height-1; ++j) {
		result[j][0] = result[j][1];
		result[j][width-1] = result[j][width-2];
		direction[j][0] = direction[j][1];
		direction[j][width-1] = direction[j][width-2];
	}
}

void nonMaximumSurpression(int width, int height, pixel (*img)[width], pixel (*result)[width]) {
	pixel central, val;
	int i,j,x,y;

	for(i=1; i<height-1; ++i) {
		for(j=1; j<width-1; ++j) {
			central = img[i][j];
			for(y=-1; y<=1; ++y) {
				for(x=-1; x<=1; ++x) {
					val = img[i+y][j+x];
					if(central < val) {
						central = 0;
						break;
					}
				}
				if(central == 0) {
					break;
				}
			}
			result[i][j] = central;
		}
	}

	/*	col 0, row 0				->	Exception Handling	*/
	/*	col width-1, row height-1	->	Exception Handling	*/
	result[0][0]				= 0;
	result[0][width-1]			= 0;
	result[height-1][0]			= 0;
	result[height-1][width-1]	= 0;

	for(i=1; i<width-1; ++i) {
		result[0][i] = 0;
		result[height-1][i] = 0;
	}
	for(j=1;j<height-1; ++j) {
		result[j][0] = 0;
		result[j][width-1] = 0;
	}
}

void hysteresisThresholding(int width, int height, pixel (*img)[width], pixel (*result)[width], edge (*direction)[width], int enQCnt) {
	int i,j;
	enQCnt=0;
	for(i=0; i<height; ++i) {
		for(j=0; j<width; ++j) {
			if(img[i][j] < EDGE_LOW_THRESHOLD) {
				result[i][j] = EDGE_REMOVED;
				direction[i][j] = 0;
			}
			else if(img[i][j] < EDGE_HIGH_THRESHOLD) {
				result[i][j] = EDGE_WEAK;
			}
			else {
				result[i][j] = EDGE_STRONG;
				enQueue(driveQ, j, i, direction[i][j]);
				enQCnt++;
			}
		}
	}
	camera2D.result.enQCnt=enQCnt;
}

void linearSmoothing(pixel* arr, pixel* result, int end) {
	//linearMedian(arr,result,end);
	linearGaussian(arr,result,end);
}

/*	MEDIAN FILTER */
/*  array = arr[0], arr[1], ... , arr[n-2], arr[n-1], arr[n].
 *  'n' is (L-1).
 *  'L' is the length of array.	*/
pixel* linearMedian(pixel* arr, pixel* result, int end) {
	pixel medianVal;
	pixel tempArr[MEDIAN_SIZE];
	const int shiftIndex = (MEDIAN_SIZE/2);
	int i,j;

	/* result[0], result[1], ... , result[MEDIAN_SIZE/2] */
	for(i=0; i<shiftIndex; ++i) {
		for(j=0; j<=i+shiftIndex; ++j) {
			tempArr[j] = arr[j];
		}
		medianVal = medianSort(tempArr,0,i+shiftIndex);
		result[i] = medianVal;
	}

	/* result[MEDIAN_SIZE/2+1] , ... , result[length-MEDIAN_SIZE/2-1] */
	for(i=shiftIndex; i<end-shiftIndex; ++i) {
		for(j=0; j<MEDIAN_SIZE; ++j) {
			tempArr[j] = arr[i+j-shiftIndex];
		}
		medianVal = medianSort(tempArr,0,MEDIAN_SIZE-1);
		result[i] = medianVal;
	}

	/*	result[n], result[n-1], ... , result[n-MEDIAN/2]	*/
	for(i=end; i>end-shiftIndex; --i) {
		for(j=0; j<=end-i+shiftIndex; ++j) {
			tempArr[j] = arr[i-j];
		}
		medianVal = medianSort(tempArr,0,end-i+shiftIndex);
		result[i] = medianVal;
	}

	return result;
}

pixel* linearGaussian(pixel* arr, pixel* result, int end) {
	const int gaussianSize = 49;
	const double invGaussianSize = 0.0204;
	const int gaussianMaskSize = 5;
	const int gM[5] = {5, 12, 15, 12, 5};
	const int shiftIndex = (gaussianMaskSize/2);

	result[0] = invGaussianSize * (double)(arr[0]*gM[2] + arr[1]*gM[3] + arr[2]*gM[4]);
	result[1] = invGaussianSize * (double)(arr[0]*gM[1] + arr[1]*gM[2] + arr[2]*gM[3] + arr[3]*gM[4]);

	int i,j;
	pixel sum;

	for(i=shiftIndex; i<end-shiftIndex; ++i) {
		sum = 0;
		for(j=0; j<gaussianMaskSize; ++j) {
			sum = sum + ( arr[i+j-2] * gM[j] );
		}
		result[i] = sum;
	}

	result[end-1] = invGaussianSize * (double)(arr[end-3]*gM[0] + arr[end-2]*gM[1] + arr[end-1]*gM[2] + arr[end]*gM[3]);
	result[end] = invGaussianSize * (double)(arr[end-2]*gM[0] + arr[end-1]*gM[1] + arr[end]*gM[2]);

	return result;
}

pixel* linearGradient(pixel* arr, pixel* result, int end) {
	const int sobelMaskSize = 5;
	const int sM[5] = {-1, -1, 0 , 1, 1};
	const int shiftIndex = (sobelMaskSize/2);

	result[0] = ABS((arr[0]*sM[2] + arr[1]*sM[3] + arr[2]*sM[4]));
	result[1] = ABS((arr[0]*sM[1] + arr[1]*sM[2] + arr[2]*sM[3] + arr[3]*sM[4]));

	int i,j;
	pixel sum;

	for(i=shiftIndex; i<end-shiftIndex; ++i) {
		sum = 0;
		for(j=0; j<sobelMaskSize; ++j) {
			sum = sum + ( arr[i+j-2] * sM[j] );
		}
		result[i] = ABS(sum);
	}

	result[end-1] = ABS((arr[end-3]*sM[0] + arr[end-2]*sM[1] + arr[end-1]*sM[2] + arr[end]*sM[3]));
	result[end] = ABS((arr[end-2]*sM[0] + arr[end-1]*sM[1] + arr[end]*sM[2]));

	return result;
}

pixel* linearNonMaximumSurpression(pixel* arr, pixel* result, int end) {
	pixel central;
	int i,j;
	/*	find Extrema Size : 3	*/
	/*	shiftIndex = findExtremaSize/2 : 1	*/

	result[0] = 0;

	for(i=1; i<end-1; ++i) {
		central = arr[i];
		for(j=-1; j<=1; ++j) {
			if(arr[i+j] > central) {
				central = 0;
				break;
			}
		}
		result[i] = central;
	}

	result[end] = 0;

	return result;
}

pixel* linearDoubleThresholding(pixel* arr, pixel* result, int end) {
	int i;

	for(i=0; i<end; ++i) {
		if(arr[i] < LINEAR_EDGE_LOW_THRESHOLD) {
			result[i] = LINEAR_EDGE_REMOVED;
		}
		else if(arr[i] < LINEAR_EDGE_HIGH_THRESHOLD) {
			result[i] = LINEAR_EDGE_WEAK;
		}
		else {
			result[i] = LINEAR_EDGE_STRONG;
		}
	}

	return result;
}
