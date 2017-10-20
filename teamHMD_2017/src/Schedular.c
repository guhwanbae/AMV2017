/*
 * Schedular.c
 *
 *  Created on: 2017. 6. 28.
 *      Author: ghbae
 */


/*
 * Schedular.c
 *
 *  Created on: 2017. 6. 27.
 *      Author: User
 */



/*******************************************************************************
* Include
*******************************************************************************/
#include "Schedular.h"

/*******************************************************************************
* Constant
*******************************************************************************/


/*******************************************************************************
* Define & MACRO
*******************************************************************************/
#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE  0
#endif

/*******************************************************************************
* Type Definition
*******************************************************************************/


/*******************************************************************************
* Prototypes of Local Functions
*******************************************************************************/



/*******************************************************************************
* Static Variable
*******************************************************************************/
static uint16 u16_BaseCounter = 0;


static bool b_1msSchedule=FALSE;
static bool b_3msSchedule=FALSE;
static bool b_5msSchedule=FALSE;
static bool b_10msSchedule=FALSE;
static bool b_20msSchedule=FALSE;
static bool b_50msSchedule=FALSE;
static bool b_100msSchedule=FALSE;
static bool b_500msSchedule=FALSE;
static bool b_1sSchedule=FALSE;
static bool b_3sSchedule=FALSE;


/*******************************************************************************
* Exported Global Variables
*******************************************************************************/


/*******************************************************************************
* Exported Functions
* Function name:

*******************************************************************************/
uint16 getBaseTick(void) {
	return u16_BaseCounter;
}

void Scheduler(void)
{
	if(u16_BaseCounter % 10 == 0) b_1msSchedule = TRUE;
	if(u16_BaseCounter % 30 == 0) b_3msSchedule = TRUE;
	if(u16_BaseCounter % 50 == 0) b_5msSchedule = TRUE;
	if(u16_BaseCounter % 100 == 0) b_10msSchedule = TRUE;
	if(u16_BaseCounter % 200 == 0) b_20msSchedule = TRUE;
	if(u16_BaseCounter % 500 == 0) b_50msSchedule = TRUE;
	if(u16_BaseCounter % 1000 == 0) b_100msSchedule = TRUE;
	if(u16_BaseCounter % 5000 == 0) b_500msSchedule = TRUE;
	if(u16_BaseCounter % 10000 == 0) b_1sSchedule = TRUE;
	if(u16_BaseCounter % 30000 == 0) b_3sSchedule = TRUE;

	u16_BaseCounter++;
	if (u16_BaseCounter>= 60000) u16_BaseCounter = 0 ;
}

bool Scheduler_1ms(void)
{
	if (b_1msSchedule)
	{
		b_1msSchedule = FALSE;

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

bool Scheduler_3ms(void)
{
	if (b_3msSchedule)
	{
		b_3msSchedule = FALSE;

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


bool Scheduler_5ms(void)
{
	if (b_5msSchedule)
	{
		b_5msSchedule = FALSE;

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


bool Scheduler_10ms(void)
{
	if (b_10msSchedule)
	{
		b_10msSchedule = FALSE;

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}




bool Scheduler_20ms(void)
{
	if (b_20msSchedule)
	{
		b_20msSchedule = FALSE;

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}



bool Scheduler_50ms(void)
{
	if (b_50msSchedule)
	{
		b_50msSchedule = FALSE;

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}






bool Scheduler_100ms(void)
{
	if (b_100msSchedule)
	{
		b_100msSchedule = FALSE;

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


bool Scheduler_500ms(void)
{
	if (b_500msSchedule)
	{
		b_500msSchedule = FALSE;

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

bool Scheduler_1s(void)
{
	if (b_1sSchedule)
	{
		b_1sSchedule = FALSE;

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

bool Scheduler_3s(void)
{
	if (b_3sSchedule)
	{
		b_3sSchedule = FALSE;

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*******************************************************************************
* Exported Functions
* Function name:
* Description:
*******************************************************************************/


/*******************************************************************************
* Local Functions
* Function name:
* Description:
*******************************************************************************/



