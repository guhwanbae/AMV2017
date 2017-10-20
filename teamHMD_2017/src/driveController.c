/*
 * driveController.c
 *
 *  Created on: 2017. 6. 29.
 *      Author: ghbae
 */

/*******************************************************************************
* Include
*******************************************************************************/
#include "global.h"
#include "vehicleController.h"
#include "cameraController.h"
#include "timerController.h"
#include "imageProcess.h"
#include "queueADT.h"
#include <math.h>
#include "Schedular.h"
/*******************************************************************************
* Constant
*******************************************************************************/

int Left;
int Right;

/*******************************************************************************
* Define & MACRO
*******************************************************************************/
#define THRESHOLD_JUDGE_CROSSING 10      //Degree

#define BOTH_LINE_DETECT      0
#define ONLY_RIGHT_LINE_DETECT   1
#define ONLY_LEFT_LINE_DETECT   2
#define NO_LINE_DETECT         3

#define UPPERCENTER            85
#define LOWERCENTER            76

/*******************************************************************************
* Type Definition
*******************************************************************************/


/*******************************************************************************
* Exported Global
*******************************************************************************/
extern queue* driveQ;
extern cameraStruct2D camera2D;
extern vehicleControl vehicle;
int line_right=0;
int line_left=0;
extern int line_state;
extern int schoolzone_state;
extern int schoolzone_position;
extern int Hill_state;
extern int Hill_position;
/*******************************************************************************
* Memeber Functions
*******************************************************************************/
void drivingControl(void) {
   /*   앞쪽에 장애물이 있는지 판단하고 차선데이터로 주행명령(pure pursuit)을 구한다.   */
//   judgeCrossCondition();

   /*   차선을 감지하고 모든 명령을 수행했으면 주행 로그를 남긴다.   */
   if(vehicle.statusData.log.isDetect == TRUE) {
      logUpdate();
   }

   /*   모든 명령을 수행하면 감지상태를 초기화해준다   */
   vehicle.statusData.log.isDetect = FALSE;
}

void setPurePursuit(edge edgeAngle) {
   double lengthWayPoint = (((double)CAM_PARAMETER_DISTANCE) / cos(edgeAngle));
   double curvature = (((double)2*sin(edgeAngle))/(lengthWayPoint));
   double servoArgument = atan(((double)VEHICLE_PARAMETER_LENGTH)*curvature);

   vehicle.purePursuit.servoArgument = servoArgument;
   vehicle.purePursuit.curCurvature = curvature;
   vehicle.purePursuit.lengthWayPoint = lengthWayPoint;
}



void setKineticStatus(velocityType mps, rpmType rpm, angleRadianType steeringAngle) {
   /*   서보명령각(servoArgu)이랑 현재 속도 등을 가지는 차량 운동 모델 값 업데이트   */
   vehicle.statusData.kinetic.curMps = mps;
   vehicle.statusData.kinetic.curRpm = rpm;
   vehicle.statusData.kinetic.curSteeringAngle = steeringAngle;
}

void updateDrivingStatus(void) {
   /*   SI 인터럽트 이후에 checkProcess2D 함수에서
    * 이미지 프로세스가 끝나고 호출되는 함수
    *  이미지 처리로 얻은 차선 정보들을
    *  vehicle 구조체에 업데이트한다.   */
   edgeNode* curNode;
      int leftCnt = 0, rightCnt = 0;
      int left_m=0,right_m=0;
      int left[camera2D.result.enQCnt], right[camera2D.result.enQCnt];
      int ref_center=0;
      /*   driveQ는 main.c에 선언된 글로벌 변수
       *  큐 구조체의 포인터이다.
       *  구조체의 포인터는 .이 아닌 ->로 호출한다.
       *  ex) driveQ->
       *  ex) edgeNode->
       *  큐 설명은 queueADT.h*/
      //double leftang=0,rightang=0;
      while( (curNode = deQueue(driveQ)) != NULL ) {
         /*
          // 큐에 저장된 엣지 정보들을 담은 노드(행 위치, 열 위치, 엣지 각도)를 불러와서 해당 노드가 left인지 right인지 판단한다.
          // 노드의 col 정보 (가로 위치 정보)를
          // log의 old값을 불러와서 비교하던가  -> (ex. (oldLeft + oldRight)/2 하면 대략 중간 지점이라고 생각)
          // 찍히는 중간 지점을 비교하던가
          // 조건문 여러개를 쓰던가 해서 left인지 right인지 판단한다.

         if( curNode->col 와  curNode->row 값 비교해서 left면) {
            leftAvgDir = leftAvgDir + curNode->dir;
            left = left + curNode->row;
            leftCnt++;
         }
         else if( curNode->col 와 curNode->row 값 비교해서 right면) {
            rightAvgDir = rightAvgDir + curNode->dir;
            right = right + curNode->row;
            rightCnt++;
         }
         */
         if (vehicle.statusData.log.oldleft == 0 && vehicle.statusData.log.oldright == 0) {
            ref_center = 80;
         }
         else {
            ref_center = vehicle.statusData.log.oldcenter;
         }
         if (curNode->col <= ref_center) {
            left[leftCnt] = curNode->col;
            leftCnt++;
         } else if (curNode->col > ref_center) {
            right[rightCnt] = curNode->col;
            rightCnt++;
         }

         /*   left나 rigth 정보들을 제외한 나머지 값들은 버린다.
          *  deQueue는 큐가 완전히 빈 상태라면 널 포인터를 리턴한다.
          *  이 작업이 끝나면 이미지 데이터 처리로 얻은 정보들이 left와 right로 각각 분류된다. */

      }
      if(leftCnt>=10){
                  leftCnt=0;
               }
               if(rightCnt>=10){
                  rightCnt=0;
               }
      int CAM_SITUATION=0;
         if(leftCnt != 0 && rightCnt !=0)
         {
            CAM_SITUATION = BOTH_LINE_DETECT;
         }
         else if(leftCnt == 0  && rightCnt !=0)
         {
            CAM_SITUATION = ONLY_RIGHT_LINE_DETECT;
         }
         else if(leftCnt != 0 && rightCnt == 0)
         {
            CAM_SITUATION = ONLY_LEFT_LINE_DETECT;
         }
         else {
            CAM_SITUATION = NO_LINE_DETECT;
         }


      /*   카운트 값을 나눠 평균값을 구한다. 카운트 값이 0이면 나눌 때 에러가 발생하므로 조건문으로 예외처리해준다.   */
      //if(leftCnt != 0) {
      //   leftAvgDir = (leftAvgDir) / ( (double)leftCnt );
      //   left = (left) / ( (double)leftCnt );

         /*   감지에 성공하면  주행 로그를 남기기 위해 감지성공으로 기록한다.   */
      //   vehicle.statusData.log.isDetect = TRUE;
      //}
      //else {
         /*   leftCnt = 0(왼쪽에서 엣지 감지 하나도 안되었을 때
          * 예외처리 해준다.
          * 만약 오른쪽이 찍혔는데 왼쪽이 안찍힌거면 오른쪽 차선을 기준으로
          * 오른쪽 차선 정보만 가지고 제어 명령을 내려준다.*/
      //}

      //if(rightCnt != 0) {
      //   rightAvgDir = (rightAvgDir) / ( (double)rightCnt );
      //   right = right / ( (double)rightCnt );

         /*   감지에 성공하면  주행 로그를 남기기 위해 감지성공으로 기록한다.   */
      //   vehicle.statusData.log.isDetect = TRUE;
      //}
      //else {
         /*   rightCnt = 0(오른쪽에서 엣지 감지 하나도 안되었을 때
         * 예외처리 해준다.
         * 만약 왼쪽이 찍혔는데 오른쪽이 안찍힌거면 왼쪽 차선 정보만으로 제어명령을 내려준다.
         * 왼쪽 오른쪽 둘 다 안찍혔으면 주행 log의 old값을 불러와서 쓰거나
         * old값도 없다면 속도를 낮추거나 직진한다.
         * 언덕구간에서 차선이 안찍히는 경우나
         * 장애물에 가리는 경우를 대비하여 코딩하면 될듯.*/
      //}


         //if(schoolzone_state==1 && CAM_SITUATION == ONLY_LEFT_LINE_DETECT && line_state==0 && vehicle.statusData.lineStatus.curTurnState==Straight)
         if(schoolzone_state==1 && CAM_SITUATION == ONLY_LEFT_LINE_DETECT && line_state==0)
         {
        	 line_left++;
        	 if(line_left>2)
        		 line_state = 1;
         }
        //else if(schoolzone_state==1 && CAM_SITUATION == ONLY_RIGHT_LINE_DETECT && line_state==0 && vehicle.statusData.lineStatus.curTurnState==Straight)
         else if(schoolzone_state==1 && CAM_SITUATION == ONLY_RIGHT_LINE_DETECT && line_state==0)
         {
        	line_right++;
        	if(line_right>2)
        		line_state = 2;
        }
         switch(CAM_SITUATION)
            {

            case BOTH_LINE_DETECT ://둘다 찍히면
               left_m= medianSortStruct(left,0,leftCnt-1);
               right_m=medianSortStruct(right,0,rightCnt-1);
               vehicle.statusData.log.isDetect = TRUE;
               break;


            case ONLY_RIGHT_LINE_DETECT : //레프트없고 라이트찍힘


               right_m=medianSortStruct(right,0,rightCnt-1);
               left_m = right_m-120;
               vehicle.statusData.log.isDetect = TRUE;
               break;


            case ONLY_LEFT_LINE_DETECT : //레프트찍고 라이트없고


               left_m= medianSortStruct(left,0,leftCnt-1);
               right_m = left_m+120;
               vehicle.statusData.log.isDetect = TRUE;
               break;


            default ://NO_LINE_DETECT

               vehicle.statusData.log.isDetect = FALSE;
               break;
            }
         if(right_m>UPPERCENTER+120){
            right_m=UPPERCENTER+120;
         }
         else if(right_m<LOWERCENTER){
            right_m=LOWERCENTER;
         }
         if(left_m<LOWERCENTER-120){
            left_m=LOWERCENTER-120;
         }
         else if(left_m>UPPERCENTER)
         {
            left_m=UPPERCENTER;
         }

      //      vehicle.statusData.camData.curLeftEdgeAngle = (vehicle.statusData.camData.curLeftEdgeAngle/PHI)*180;
         //   vehicle.statusData.camData.curRightEdgeAngle = (vehicle.statusData.camData.curRightEdgeAngle/PHI)*180;
           //servoset(vehicle.statusData.camData.curcenter);
            if(vehicle.statusData.log.isDetect==TRUE)
			{
				vehicle.statusData.camData.curleft = (int)left_m;
				vehicle.statusData.camData.curright =(int)right_m;
				vehicle.statusData.camData.curcenter = (int)(left_m+right_m)/2;
				if(schoolzone_state==1)
				{
				Left = vehicle.statusData.camData.curleft;
				Right = vehicle.statusData.camData.curright;
				}
		//		vehicle.statusData.camData.curLeftEdgeAngle = (vehicle.statusData.camData.curLeftEdgeAngle/PHI)*180;
			//	vehicle.statusData.camData.curRightEdgeAngle = (vehicle.statusData.camData.curRightEdgeAngle/PHI)*180;
				//if((schoolzone_state==1 && schoolzone_position==1) || (Hill_state == 1))
				if((schoolzone_state==1 && schoolzone_position==1) || (Hill_state == 1 && (Hill_position==0 || Hill_position==1)))
				{
				}
				else
					servoset(vehicle.statusData.camData.curcenter);
			}




      //      double calcedWidth = 0, calibratedWidth = 0;

         //   calcedWidth = ABS(left_m - right_m);
            /*   calcedWidth : 왼쪽 오른쪽 엣지 픽셀간 간격차이로 찾은 거리.
             * 1차선일때는 도로폭이   60cm로 일정하기때문에 큰 변동이 없다.
             * 급커브 구간이나 분기점에서 calcedWidth가 크게 바뀐다.*/

         //   calibratedWidth = (calcedWidth / 2)   * (cos(leftAvgDir_m) + cos(rightAvgDir_m));
            /*   도로 곡률이나 노이즈로인한 차선폭 보정 */

            /*   calcedWidth는 차량 수직방향으로 찍힌 차선간격   */
      //      vehicle.statusData.camData.curLaneWidth = calcedWidth;
            /*   calibratedWidth는 도로 곡률을 고려해서 보정한 차선간격. 아마 1차선 구간에서는 일정하게 측정될 것 같다.   */
         //   vehicle.statusData.camData.curCompWidth = calibratedWidth;

         else {
            ;
         }
}

void logUpdate(void) {
   /*   감지 실패할 경우를 대비해서 주행 데이터 로그를 남긴다.
    * 감지실패한 경우 로그값(old)를 불러와서 주행명령을 내린다.   */
   vehicle.statusData.log.oldCompWidth = vehicle.statusData.camData.curCompWidth;
   vehicle.statusData.log.oldLaneWidth = vehicle.statusData.camData.curLaneWidth;
   vehicle.statusData.log.oldLeftEdgeAngle = vehicle.statusData.camData.curLeftEdgeAngle;
   vehicle.statusData.log.oldRightEdgeAngle = vehicle.statusData.camData.curRightEdgeAngle;
   vehicle.statusData.log.oldLengthWayPoint = vehicle.purePursuit.lengthWayPoint;
   vehicle.statusData.log.oldCurCurvature = vehicle.purePursuit.curCurvature;
   vehicle.statusData.log.oldServoArgument = vehicle.purePursuit.servoArgument;
   vehicle.statusData.log.oldleft = vehicle.statusData.camData.curleft;
   vehicle.statusData.log.oldright = vehicle.statusData.camData.curright;
   vehicle.statusData.log.oldcenter = vehicle.statusData.camData.curcenter;
   vehicle.statusData.lineStatus.olddiff = vehicle.statusData.lineStatus.curdiff;
   vehicle.statusData.lineStatus.oldTurnState = vehicle.statusData.lineStatus.curTurnState;
   /*(모든 데이터를 현재 명령으로 업데이트)*/
}


void servoset(int center) {
	int curstate = vehicle.statusData.lineStatus.curTurnState;
	int oldstate = vehicle.statusData.lineStatus.oldTurnState;
	double cur_diff = vehicle.statusData.lineStatus.curdiff;
	double old_diff = vehicle.statusData.lineStatus.olddiff;

	if (center > UPPERCENTER) {
		cur_diff = center - (UPPERCENTER);
		if (cur_diff >= 6)
			curstate = TurningRight;
		else {
			curstate = TurnRight;
		}
	} else if (center < LOWERCENTER) {
		cur_diff = center - (LOWERCENTER);
		if (cur_diff <= -6) {
			curstate = TurningLeft;
		} else {
			curstate = TurnLeft;
		}
	} else {
		cur_diff = 0;
		curstate = Straight;
	}

	switch (curstate) {
	case TurningLeft:
		pwmServo(31250, (cur_diff - 2) * 0.5729);
		break;
	case TurnLeft:
		pwmServo(31250, (cur_diff - 2) * 0.5729);
		break;
	case TurningRight:
		pwmServo(31250, (cur_diff + 4) * 0.5729);
		break;
	case TurnRight:
		pwmServo(31250, (cur_diff + 4) * 0.5729);
		break;
	default:
		pwmServo(31250, cur_diff * 0.5729);
		break;
	}

	if (schoolzone_state == 1 || Hill_state == 1 || vehicle.statusData.camData.AEB == 1) {
	} else {
		if (oldstate == Straight) {
			if (cur_diff >= 5 || cur_diff <= -5) {
				pwmDcMotor(0, 70);
				while (1) {
					if (Scheduler_10ms()) {
						break;
					}
					while (1) {
						if (Scheduler_10ms()) {
							break;
						}
					}
				}
			}
		}

		if (curstate == TurningLeft || curstate == TurningRight) {
			pwmDcMotor(50, 0);
		} else {

			if (oldstate == TurningLeft || oldstate == TurningRight) {
				vehicle.statusData.lineStatus.turnCnt++;
				if (oldstate == TurningLeft) {
					curstate = TurningLeft;
				} else if (oldstate == TurningRight) {
					curstate = TurningRight;
				}
				pwmDcMotor(50, 0);
				if (vehicle.statusData.lineStatus.turnCnt >= 4) {
					pwmDcMotor(50, 0);
					vehicle.statusData.lineStatus.turnCnt=0;
				}
			} else {
				vehicle.statusData.lineStatus.turnCnt=0;
				if(curstate==TurnRight ||curstate==TurnLeft)
					pwmDcMotor(70,0);
				else
					pwmDcMotor(90, 0);
			}
		}
	}
}

