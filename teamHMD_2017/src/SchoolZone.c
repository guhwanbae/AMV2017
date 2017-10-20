/*
 * SchoolZone.c
 *
 *  Created on: 2017. 7. 7.
 *      Author: User
 */

/*******************************************************************************
* Include
*******************************************************************************/
#include "vehicleController.h"
#include "DMS.h"
#include "global.h"
#include "Schedular.h"
#include "cameraController.h"
#include "timerController.h"
#include "imageProcess.h"


void Leftcross_obstacle(void);
void Rightcross_obstacle(void);

/*******************************************************************************
* Constant
*******************************************************************************/
int schoolzone_state=0;
int schoolzone_position=0;

int line_state=0; //0으로바꿔야해제발꼭

/*******************************************************************************
* Define & MACRO
*******************************************************************************/

/*******************************************************************************
* Exported Global
*******************************************************************************/
extern int Left;
extern int Right;
extern int DMS_BOTTOM;
//extern int DMS_TOP;
extern int DMS_RIGHT;
extern int DMS_LEFT;
extern int school_cnt_in;
extern int school_cnt_out;
//extern cameraStruct camera;
//extern cameraStruct2D camera2D;

bool isFrontObstacle(void) {
	if(DMS_BOTTOM >650)
		return TRUE;
	else
		return FALSE;
}

void schoolZoneStatus(void)
{
	if(school_cnt_in==1)// 적외선이 검은라인 인식 했을 때
	{
	      schoolzone_state = 1;
	      school_cnt_in=0;
	}
	while(schoolzone_state ==1)
   {
		pwmDcMotor(38,0); // 스쿨존 내 속도 다시살려야해제발
      while(line_state == 1) // 1차선 주행의 모든경우를 여기 다넣어보자
      {

         switch(schoolzone_position)
         {
            case 0 : // 장애물 안 본 경우(직진)
            	//if(isFrontObstacle() == TRUE || DMS_LEFT > 500 || DMS_RIGHT > 500 )
            	//if(isFrontObstacle() == TRUE)
            	if(isFrontObstacle() == TRUE ||  DMS_LEFT > 550 || DMS_RIGHT > 550 )
            		schoolzone_position = 1;// 장애물 인식
            	else
            		schoolzone_position = 0;// 장애물 미인식
            	break;
            case 1 :             	// 장애물 본 경우
            	Rightcross_obstacle();// rightcross라는 함수안에 장애물피하고 다시 라인인식까지 하는 내용 포함
            	break;
         }
        if(school_cnt_out==1)// 적외선이 인식했을 때 count
         {
            schoolzone_state=0; // 스쿨존 빠져 나온다.
            break;
         }
         else if(line_state ==2)// 2차선에 들어왔을경우 while문 종료
            break;
      }
      while(line_state == 2) // 2차선 주행의 모든경우를 여기 다넣어보자
      {
         switch(schoolzone_position)
         {
            case 0 : // 장애물 안 본 경우(직진)
            	//if(isFrontObstacle() == TRUE || DMS_LEFT > 500 || DMS_RIGHT > 500)
            	//if(isFrontObstacle() == TRUE)
            	if(isFrontObstacle() == TRUE || DMS_LEFT > 550 || DMS_RIGHT > 550 )
            		schoolzone_position = 1;
            	else
            		schoolzone_position = 0;
            	break;

            case 1 :   //장애물을 본 경우
            	Leftcross_obstacle();// leftcross라는 함수안에 장애물피하고 다시 라인인식까지 하는 내용 포함
            	break;
         }
         if(school_cnt_out==1)// 적외선이 인식했을 때 count
         {
            schoolzone_state=0; // 스쿨존 빠져나온다
            break;
         }
         else if(line_state ==1)// 2차선에 들어왔을경우 while문 종료
            break;
      }
      if(schoolzone_state==0) // 전체 스쿨존 while문 종료

    	  break;
   }

}

void Leftcross_obstacle(void)
{
   while(1)
   {
	  pwmServo(31250, -20); //장애물 인식하면 서보 좌측으로 최대한 꺽기
	  if (Left> 45)// 바퀴가 점선쯤 오면 빠져나온다
		 break;
   }
   while(1)
   {
	  pwmServo(31250, -17);
	  if (Left < 20)     //
		 break;
   }
   while(1)
     {
	   pwmServo(31250, -14);
	   if (Left > 25)     // 반대편라인 인식할때 쯤 다시 서보 우측으로돌린다
           break;
     }
   while(1)
   {
	   pwmServo(31250, 21);// 바퀴 우측으로 확돌게 한다

	   if (Left < 25 && Left > 5)
		   break;
   }
   while(1)
   {
	   pwmServo(31250, 5);// 바퀴 우측으로 확돌게 한다
	   if (Left > 5)
	   {
		   line_state =1;
		   schoolzone_position=0;
            break;
	   }
   }
   // 다시 왼쪽차선으로만 서보 잡기
}
void Rightcross_obstacle(void)
{
	while(1)
   {
		/*P13_OMR.B.PCL0=1;
		P13_OMR.B.PS0=0;*/
      pwmServo(31250, 21); //장애물 인식하면 서보 우측으로 최대 꺽기
      if(Right < 120)     // 바퀴가 점선 쯤에서 빠져나온다
         break;
   }
   while(1)
   {
	   /*P13_OMR.B.PCL1=1;
	   P13_OMR.B.PS1=0;*/
      pwmServo(31250, 15);
      if(Right > 130)     //
         break;
   }
   while(1)
   {
	   /*P13_OMR.B.PCL2=1;
	   	P13_OMR.B.PS2=0;*/
	   pwmServo(31250, 10);
      if(Right < 130)    //반대편라인 인식할 때 쯤 빠져나온다
            break;
   }
   while (1)
   {
      pwmServo(31250, -21); // 바퀴를 왼쪽으로 최대 꺽기
      if (Right > 120 && Right < 140 )//
         break;
   }
   while (1)
   {
	   pwmServo(31250, -5); // 정상주행쯤으로 만들어준다
	   if (Right > 120)
	   {
		   line_state =2;
		   schoolzone_position=0;
		   break;
	   }
   }
   // 다시 카메라로 우측라인 찍으면서 센터잡아서 직진
}









