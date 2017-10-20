/*
 * vehicleController.h
 *
 *  Created on: 2017. 6. 26.
 *      Author: ghbae
 */

#ifndef VEHICLECONTROLLER_H_
#define VEHICLECONTROLLER_H_

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
#define SINGLE_LANE 1
#define FROM_SINGLE_LANE_TO_DOUBLE -1
#define	DOUBLE_LANE 2
#define FROM_DOUBLE_LNAE_TO_SINGLE -2
#define NO_WAY_ON_AEB 8411

#define VEHICLE_ON_LEFT_LANE -2
#define VEHICLE_FROM_LEFT_TO_RIGHT_LANE -1
#define VEHICLE_ON_SINGLE_LANE 0
#define VEHICLE_FROM_RIGHT_TO_LEFT_LANE 1
#define VEHICLE_ON_RIGHT_LANE 2

#define CURVED_LEFT_LANE -1
#define STRAIGHT_LALE 0
#define CURVED_RIGHT_LANE 1
#define ZIGZAG_LANE 2
#define NO_WAY_STOP 8411

#define DRIVE_DYNAMIC 1
#define DRIVE_SCHOOLZONE 0

#define VEHICLE_PARAMETER_LENGTH 40
#define Straight		0
#define TurnLeft		1
#define TurnRight		3
#define TurningLeft		2
#define TurningRight	4
/*******************************************************************************
* Type Definition
*******************************************************************************/
typedef double rpmType;
typedef double lengthType;
typedef double angleRadianType;
typedef double velocityType;
typedef int turnState;
typedef int schoolzoneState;
/*******************************************************************************
* Exported Global Variables
*******************************************************************************/

/*******************************************************************************
* Memeber Structures
*******************************************************************************/
/*	VEHICLE DATA STRUCTURES	*/
typedef struct purePursuit {
	lengthType lengthWayPoint;
	angleRadianType curCurvature;
	angleRadianType servoArgument;
}purePursuit;

typedef struct kineticStatus {
	rpmType curRpm;	/* Unit : rpm	*/
	velocityType curMps; /*	Unit : m/s */
	angleRadianType curSteeringAngle; /* Unit : radian	*/
}kineticStatus;

typedef struct cameraData {
	angleRadianType curLeftEdgeAngle;
	angleRadianType curRightEdgeAngle;
	lengthType curLaneWidth;
	lengthType curCompWidth;	/*	Calibrated	Width	*/
	int curleft;
	int curright;
	int curcenter;
	int AEB;
}cameraData;

typedef struct drivingLog {
	bool isDetect;
	angleRadianType oldLeftEdgeAngle;
	angleRadianType oldRightEdgeAngle;
	lengthType oldLaneWidth;
	lengthType oldCompWidth;	/*	Calibrated	Width	*/
	lengthType oldLengthWayPoint;
	angleRadianType oldCurCurvature;
	angleRadianType oldServoArgument;
	int oldleft;
	int oldright;
	int oldcenter;
}drivingLog;
typedef struct lineStatus {
	turnState oldTurnState;
	turnState oldangle;
	turnState curTurnState;
	turnState curangle;
	turnState angleStatus;
	schoolzoneState SchoolZoneState;
	schoolzoneState SchoolZoneStateCnt;
	lengthType	curdiff;
	lengthType	olddiff;
	turnState	turnCnt;
}lineStatus;
typedef struct vehicleStatus {
	cameraData camData;
	kineticStatus kinetic;
	drivingLog log;
	lineStatus lineStatus;
}vehicleStatus;

typedef struct vehicleControl {
	vehicleStatus statusData;
	purePursuit purePursuit;
}vehicleControl;


/*******************************************************************************
* Memeber Functions
*******************************************************************************/
void peripheralInit(void);

void adcInit(void);
void pwmServo(int servo0, double servo_angle);
void pwmDcMotor(long double motor0, long double motor1);
void peripheralOn(void);

void drivingControl(void);
void updateDrivingStatus(void);

void judgeCrossCondition(void);
void setPurePursuit(edge edgeAngle);

bool isFrontObstacle(void);

void logUpdate(void);
void servoset(int center);

void schoolZoneStatus(void);

#endif /* VEHICLECONTROLLER_H_ */
